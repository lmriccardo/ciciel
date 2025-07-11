#include "buffered_io.hpp"

using namespace ccl::sys::io;

BufferedFileIO::BufferedFileIO(const std::string &path, iom mode, size_t buff_size)
    : FileIO( path, mode ), m_buffer( buff_size )
{
    m_buffer.position(0);
    m_buffer.clear();
}

BufferedFileIO::BufferedFileIO(BufferedFileIO &&other)
    : FileIO( std::move(other) )
{
    m_buffer = std::move( other.m_buffer );
}

BufferedFileIO &BufferedFileIO::operator=(BufferedFileIO &&other)
{
    FileIO::operator=( std::move(other) );

    if ( this != &other )
    {
        m_buffer = std::move( other.m_buffer );
    }

    return *this;
}

void BufferedFileIO::setBufferCapacity(size_t capacity)
{
    m_buffer.allocate( capacity );
}

ssize_t BufferedFileIO::write(const char *buff, size_t nbytes)
{
    size_t total_written_bytes = 0;

    while ( total_written_bytes < nbytes )
    {
        size_t rem_bytes = nbytes - total_written_bytes;
        size_t n_bytes_to_write = std::min( rem_bytes, m_buffer.getRemainingCapacity() );
        const char* next_data = buff + total_written_bytes;
        
        m_buffer.putBuffer( 
            reinterpret_cast<const unsigned char*>(next_data), 
            n_bytes_to_write );

        if ( m_buffer.getRemainingCapacity() == 0 )
        {
            flush();
        }

        total_written_bytes += n_bytes_to_write;
    }

    return total_written_bytes;
}

ssize_t BufferedFileIO::flush()
{
    // Write the entire buffer into the file
    const char* buff = (const char*)m_buffer.getBuffer();
    ssize_t r = FileIO::write( buff, m_buffer.getBufferSize() );
    m_buffer.clear();
    return r;
}
