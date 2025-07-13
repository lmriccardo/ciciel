#include "file_io.hpp"

using namespace ccl::sys::io;

bool ccl::sys::io::iom2bool(iom a)
{
    return a != iom::None;
}

FileIOBase::FileIOBase(FileIOBase &&other) : StreamIO( std::move(other) )
{
    m_filePath = std::move( other.m_filePath );
    m_openMode = other.m_openMode;
    m_readIdx  = other.m_readIdx;
    m_writeIdx = other.m_writeIdx;

    other.m_readIdx = -1;
    other.m_writeIdx = -1;
}

FileIOBase &ccl::sys::io::FileIOBase::operator=(FileIOBase &&other)
{
    if ( this != &other )
    {
        StreamIO::operator=( std::move(other) );

        m_filePath = std::move( other.m_filePath );
        m_openMode = other.m_openMode;
        m_readIdx  = other.m_readIdx;
        m_writeIdx = other.m_writeIdx;

        other.m_readIdx = -1;
        other.m_writeIdx = -1;
    }

    return *this;
}

bool FileIOBase::hasFlag(iom flag) const
{
    return static_cast<bool>(flag & m_openMode);
}

bool FileIOBase::isOpen() const
{
    return m_handler.isValid();
}

size_t FileIOBase::getNofLines()
{
    // Compute the total number of lines
    OFF_T prev_read_idx = m_readIdx;

    std::string line;
    size_t nof_lines = 0;
    while ( readLine(line) != 0 ) { nof_lines++; }
    
    m_readIdx = prev_read_idx;
    return nof_lines;
}

ssize_t FileIOBase::readLine(std::string &line)
{
    if ( m_handler.isValid() )
    {
        ssize_t total_bytes = 0; // Initialize the total bytes result
        ssize_t curr_bytes; // The current number of bytes read
        char curr_ch; // Current char
        line.clear(); // Clear the destination before reading

        do
        {
            // Read a single character from the current position and
            // check for any potential error (-1 is returned).
            if ( ( curr_bytes = read( &curr_ch, (size_t)1 ) ) < 0 )
            {
                line.clear();
                return -1;
            }

            // Check for EOF (End Of File)
            if ( curr_bytes == 0 ) break;
            
            // Updates the line and the total count
            line += curr_ch;
            total_bytes++;

        } while (curr_ch != '\n');

        return total_bytes;
    }

    return -1;
}

void FileIOBase::read(std::string &dest)
{
    seekg( 0, iop::Beg );
    std::string line;
    while ( readLine( line ) != 0 ) { dest += line; }
}

OFF_T FileIOBase::getSize() const
{
    OFF_T fileSize = -1;

#ifndef _WIN32

    // First we need to take the entire file size
    struct stat st;
    if ( ::fstat( m_handler.get(), &st ) == 0 )
    {
        fileSize = static_cast<OFF_T>( st.st_size );
    }

#else

    LARGE_INTEGER _filesize;
    if (GetFileSizeEx( m_handler.get(), &_filesize ))
    {
        fileSize = static_cast<OFF_T>( _filesize.QuadPart );
    }

#endif

    if ( fileSize == -1 )
    {
        std::stringstream ss;
        ss << "Failed to get file size for " << m_filePath << " [Error]: " 
           << get_last_error_string() 
           << std::endl;

        throw std::runtime_error( ss.str() );
    }

    return fileSize;
}

OFF_T FileIOBase::tellp() const
{
    return m_writeIdx;
}

OFF_T FileIOBase::tellg() const
{
    return m_readIdx;
}

FileIOBase &ccl::sys::io::operator<<(FileIOBase &stream, const std::string &data)
{
    stream.write( data ); // Write the data into the stream
    return stream;
}

FileIOBase &ccl::sys::io::operator<<(FileIOBase &stream, const char *data)
{
    stream.write( data, strlen(data) );
    return stream;
}

std::ostream &ccl::sys::io::operator<<(std::ostream &ostream, FileIOBase &stream)
{
    // Get the entire content of the file
    std::string content;
    stream.read( content );

    // Put the content into the output stream
    ostream << content;
    return ostream;
}

std::string &ccl::sys::io::operator>>(FileIOBase &stream, std::string &dest)
{
    stream.read( dest );
    return dest;
}

file_iterator::file_iterator(FileIOBase *file_io, size_t size)
    : Base( file_io, size ), m_fileSize( size )
{
}

file_iterator::reference file_iterator::operator*() const
{
    m_iterable->readLine( m_currLine );
    return m_currLine;
}

file_iterator::pointer file_iterator::operator->() const
{
    m_iterable->readLine( m_currLine );
    return &m_currLine;
}

FileIOIterable::iterator FileIOIterable::begin()
{
    return iterator( this, 0 );
}

FileIOIterable::iterator FileIOIterable::end()
{
    return iterator( this, getNofLines() );
}

#ifndef _WIN32

FLAG_T ccl::sys::io::__make_flags(iom mode)
{
    FLAG_T flags = 0; // initialize the result variable

    if ( iom2bool(mode & iom::Read) && iom2bool(mode & iom::Write) )
    {
        flags |= O_RDWR;
    }
    else if ( iom2bool( mode & iom::Read ) )
    {
        flags |= O_RDONLY;
    }
    else if ( iom2bool( mode & iom::Write ) )
    {
        flags |= O_WRONLY;
    }

    if ( iom2bool( mode & iom::App ) )    flags |= O_APPEND;
    if ( iom2bool( mode & iom::Trunc ) )  flags |= O_TRUNC;
    if ( iom2bool( mode & iom::Create ) ) flags |= O_CREAT;

    return flags;
}

NativeHandleType ccl::sys::io::__open_file(const std::string &path, iom mode)
{
    // Create the correct list of flags and open the file
    FLAG_T o_flags = __make_flags( mode );

    NativeHandleType fd;
    if ( ( fd = open( path.c_str(), o_flags, 0644 ) ) < 0 )
    {
        std::stringstream ss;
        ss << "Unable to open file: " << path << " "
           << "[Error]: " << std::strerror(errno)
           << std::endl;

        throw std::runtime_error( ss.str() );
    }

    return fd;
}

FileIO::FileIO(const std::string &path, iom flags)
    : FileIOIterable( path, flags )
{
    m_handler.reset( __open_file( path, flags ) );

    // Here we need to set the read and write index. The read index
    // always starts at the beginning of the file.
    seekg( 0, iop::Beg );

    // The write index depends on the open mode. If it is append then
    // we need to put it at the end of the file, otherwise at the start
    // of the file.
    iop pos = iom2bool( flags & iom::App ) ? iop::End : iop::Beg;
    seekp( 0, pos );
}

void FileIO::seekg(OFF_T offset, iop position)
{
    m_readIdx = lseek( m_handler.get(), offset, static_cast<int>(position) );
}

void FileIO::seekp(OFF_T offset, iop position)
{
    m_writeIdx = lseek( m_handler.get(), offset, static_cast<int>(position) );
}

ssize_t FileIO::read(char *dst, size_t rsize)
{
    if ( m_handler.isValid() )
    {
        lseek( m_handler.get(), m_readIdx, static_cast<int>( iop::Beg ) );
        
        ssize_t nbytes;
        if ( ( nbytes = ::read( m_handler.get(), dst, rsize ) ) < 0 )
        {
            std::cerr << "Unable to read bytes from " << m_filePath
                      << " [Error]: " << std::strerror(errno)
                      << std::endl;
            
            return -1;
        }

        m_readIdx += nbytes; // Update the read index
        return nbytes;
    }

    return -1;
}

ssize_t FileIO::write(const char *src, size_t nbytes)
{
    if ( m_handler.isValid() )
    {
        lseek( m_handler.get(), m_writeIdx, static_cast<int>(iop::Beg) );

        ssize_t nw_bytes;
        if ( ( nw_bytes = ::write( m_handler.get(), src, nbytes ) ) < 0 )
        {
            std::cerr << "Unable to write bytes into " << m_filePath
                      << " [Error]: " << std::strerror(errno)
                      << std::endl;

            return -1;
        }

        m_writeIdx += nw_bytes;
        return nw_bytes;
    }

    return -1;
}

#else

// TODO: Do something for windows version

#endif

const iom FileIO::DEFAULT_OPEN_FLAGS = iom::Read | iom::Write | iom::App;

