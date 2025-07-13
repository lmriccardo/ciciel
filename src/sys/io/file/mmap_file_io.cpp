#include "mmap_file_io.hpp"

using namespace ccl::sys::io;

FLAG_T ccl::sys::io::__make_flags(protf flags)
{
#ifndef _WIN32

    FLAG_T result = 0;

    // Guard for checking NONE mutual exclusion
    if ( to_bool(flags & protf::None) && to_bool(flags & ~protf::None) )
    {
        throw std::runtime_error( "NONE cannot appear with other flags." );
    }

    if ( to_bool( flags & protf::None  ) ) result |= PROT_NONE;
    if ( to_bool( flags & protf::Read  ) ) result |= PROT_READ;
    if ( to_bool( flags & protf::Write ) ) result |= PROT_WRITE;
    if ( to_bool( flags & protf::Exec  ) ) result |= PROT_EXEC;

    return result;

#else

    if ( flags == protf::ReadOnly  ) return PAGE_READONLY;
    if ( flags == protf::ReadWrite ) return PAGE_READWRITE;
    if ( flags == protf::ExecRead  ) return PAGE_EXECUTE_READ;
    if ( flags == protf::ExecRW    ) return PAGE_EXECUTE_READWRITE;

#endif
}

FLAG_T ccl::sys::io::__make_flags(mapf flags)
{
    FLAG_T result = 0;

#ifndef _WIN32

    // Guard check for mutually exclusion of both PRIVATE and SHARED
    if ( to_bool(flags & mapf::Private) && to_bool(flags & mapf::Shared) )
    {
        throw std::runtime_error( "Cannot use map PRIVATE and SHARED together" );
    }

    if ( to_bool( flags & mapf::Private   ) ) result |= MAP_PRIVATE;
    if ( to_bool( flags & mapf::Shared    ) ) result |= MAP_SHARED;
    if ( to_bool( flags & mapf::Populate  ) ) result |= MAP_POPULATE;
    if ( to_bool( flags & mapf::Fixed     ) ) result |= MAP_FIXED;

#else

    if ( to_bool( flags & mapf::All   ) ) return FILE_MAP_ALL_ACCESS;
    if ( to_bool( flags & mapf::Read  ) ) result |= FILE_MAP_READ;
    if ( to_bool( flags & mapf::Write ) ) result |= FILE_MAP_WRITE;
    if ( to_bool( flags & mapf::Exec  ) ) result |= FILE_MAP_EXECUTE;

#endif

    return result;
}

void *ccl::sys::io::MappedFileIO::mmap(OFF_T fsize, FLAG_T pfl, FLAG_T mfl)
{
    void* addr = nullptr;

#ifndef _WIN32 // POSIX specific implementation

    addr = ::mmap( nullptr, fsize, pfl, mfl, m_handler.get(), 0 );
    if ( addr == MAP_FAILED ) addr = nullptr;

#else // WINDOWS Specific implementation

    m_hMap = CreateFileMapping( m_handler.get(), NULL, pfl, 0, 0, NULL );
    addr = MapViewOfFile( hMap, mfl, 0, 0, 0 );

#endif

    if ( addr == nullptr )
    {
        std::stringstream ss;
        ss << "Unable to Memory map file " << m_filePath << " [Error]: "
           << get_last_error_string()
           << std::endl;

        throw std::runtime_error( ss.str() );
    }

    return addr;
}

void MappedFileIO::remap( OFF_T size, bool keep_size )
{
    // Close the previous allocated memory map
    free();
    
    // Creates a new map
    // Get the actual flags using the overload function, platform specific
    FLAG_T prot_f = __make_flags( m_protFlags );
    FLAG_T map_f  = __make_flags( m_mapFlags );

    void* addr = mmap( size, prot_f, map_f );
    unsigned char* data = static_cast<unsigned char*>( addr );

    m_mapBuffer.setBuffer( data, size, keep_size );
}

OFF_T MappedFileIO::compute_position(OFF_T offset, iop position) const
{
    OFF_T result = 0;

    switch ( position )
    {
        case iop::Beg: result = offset; break;
        case iop::Cur: result = m_mapBuffer.position() + offset; break;
        case iop::End: result = m_mapBuffer.getBufferSize() + offset; break;
    }

    return result;
}

void MappedFileIO::set_position(OFF_T position)
{
    size_t n_position = static_cast<size_t>(position);

    if ( n_position >= m_mapBuffer.getBufferCapacity() )
    {
        // This means that we would like to access data outside the current capacity
        size_t size = m_growFactor * static_cast<size_t>(getSize());

        // Create a new mem map with the new file size increased 
        // using the grow factor.
        remap( size, true );
    }

    // Finally, set the buffer position
    m_mapBuffer.position( n_position );
}

void MappedFileIO::free()
{
#ifndef _WIN32

    munmap( static_cast<void*>(m_mapBuffer.getBuffer()), 
            m_mapBuffer.getBufferCapacity() );

#else

    UnmapViewOfFile( static_cast<void*>(m_mapBuffer.getBuffer()) );
    CloseHandle( m_hMap );

#endif
}

MappedFileIO::MappedFileIO(const std::string &filePath, iom mode, protf pfl, mapf mfl)
    : FileIOBase(filePath, mode), m_protFlags( pfl ), m_mapFlags( mfl )
{
    // First open the file, platform specific implementation
    m_handler.reset( __open_file( filePath, mode ) );

    // Get the actual flags using the overload function, platform specific
    FLAG_T prot_f = __make_flags( pfl );
    FLAG_T map_f  = __make_flags( mfl );

    // Get the actual file size
    OFF_T fileSize = getSize();

    // Create the memory mapping of the entire file
    void* addr = mmap( fileSize, prot_f, map_f );
    unsigned char* data = static_cast<unsigned char*>( addr );
    m_mapBuffer.setBuffer( data, fileSize, false );

    // Here we need to set the read and write index. The read index
    // always starts at the beginning of the file.
    seekg( 0, iop::Beg );

    // The write index depends on the open mode. If it is append then
    // we need to put it at the end of the file, otherwise at the start
    // of the file.
    iop pos = iom2bool( mode & iom::App ) ? iop::End : iop::Beg;
    seekp( 0, pos );
}

MappedFileIO::~MappedFileIO()
{
    free();

    if ( m_policy == MSyncPolicy::SYNC_ON_CLOSE )
    {
        msync( m_mapBuffer.getBuffer(), m_mapBuffer.getBufferCapacity(), MS_SYNC );
    }
}

void MappedFileIO::seekg( OFF_T offset, iop position )
{
    m_readIdx = compute_position( offset, position );
    set_position( m_readIdx );
}

void MappedFileIO::seekp( OFF_T offset, iop position )
{
    m_writeIdx = compute_position( offset, position );
    set_position( m_writeIdx );
}

ssize_t MappedFileIO::read(char *dst, size_t rsize)
{
    m_mapBuffer.position( m_readIdx );

    // Check the total number of remaining bytes
    if ( rsize > m_mapBuffer.getRemainingSize() )
    {
        rsize = m_mapBuffer.getRemainingSize();
    }
    
    m_mapBuffer.getBuffer( reinterpret_cast<unsigned char*>(dst), rsize );
    m_readIdx = m_mapBuffer.position();

    return rsize;
}

ssize_t MappedFileIO::write(const char *src, size_t nbytes)
{   
    // Check if we can even write into the buffer
    if ( !to_bool( m_protFlags & protf::Write ) ) return 0;

    // Check the total number of remaining bytes
    if ( m_writeIdx + nbytes > m_mapBuffer.getBufferCapacity() )
    {
        size_t nsize = m_mapBuffer.getBufferCapacity() * m_growFactor;
        int result = 0;

#ifndef _WIN32

        // First, we need to resize the file
        result = ftruncate( m_handler.get(), nsize );

#else

        // Do the same for Windows

#endif

        if ( result < 0 )
        {
            std::cerr << "Unable to resize the file " << m_filePath << " [Error]: "
                      << get_last_error_string()
                      << std::endl;
            
            return -1;
        }

        remap( nsize, true );
    }

    m_mapBuffer.position( m_writeIdx );
    m_mapBuffer.putBuffer( reinterpret_cast<const unsigned char*>(src), nbytes );
    m_writeIdx = m_mapBuffer.position();

    if ( m_policy == MSyncPolicy::SYNC_ON_WRITE )
    {
#ifndef _WIN32
        int res = msync( m_mapBuffer.getBuffer(), m_mapBuffer.getBufferSize(), MS_SYNC );
        std::cout << res << std::endl;
#else
        // Do something for Windows
#endif
    }

    return nbytes;
}

void MappedFileIO::setCapacityGrowFactor(size_t factor)
{
    m_growFactor = factor;
}

void MappedFileIO::setSyncPolicy(MSyncPolicy policy)
{
    m_policy = policy;
}
