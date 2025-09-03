#include "stream_io.hpp"

using namespace ccl::sys::io;

std::string ccl::sys::io::get_last_error_string()
{
#ifndef _WIN32

    if ( errno == 0 ) return "No Error Message";
    return std::strerror( errno );

#else

    DWORD err = GetLastError();
    if ( err == 0 ) return "No Error Message";

    // Otherwise construct the error string
    LPVOID lpMsgBuf = nullptr;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&lpMsgBuf,
        0, NULL);

    std::string message;
    if ( lpMsgBuf )
    {
        message = static_cast<char*>( lpMsgBuf );
        LocalFree( lpMsgBuf );
    }
    else
    {
        message = "Unknown Error Code: " + std::to_string(err);
    }

    return message;

#endif
}

StreamIO::StreamIO(NativeHandleType handle)
    : m_handler(handle)
{
    if (!m_handler.isValid())
    {
        throw std::runtime_error("Failed to create StreamIO with invalid handle.");
    }
}

StreamIO::StreamIO(StreamHandler<NativeHandleType>&& handler)
    : m_handler(std::move(handler))
{}

StreamIO::StreamIO(StreamIO&& other)
    : m_handler(std::move(other.m_handler))
{}

StreamIO& StreamIO::operator=(StreamIO&& other)
{
    if (this != &other)
    {
        m_handler = std::move(other.m_handler);
    }
    return *this;
}

NativeHandleType StreamIO::getNativeHandle() const
{
    return m_handler.get();
}

void StreamIO::close()
{
    if (m_handler.isValid())
    {
        m_handler.reset();
    }
}

ssize_t StreamIO::read(std::string &dst, size_t rsize)
{
    dst.resize( rsize ); // Resize the string to contain the input bytes
    ssize_t bytes = read( dst.data(), rsize ); // read into the string

    // Resize the string to the actual number of read bytes
    if ( bytes >= 0 && static_cast<size_t>(bytes) < rsize )
    {
        dst.resize( bytes );
    }

    return bytes;
}

ssize_t StreamIO::read(ds::buffers::ByteBuffer &buff, size_t rsize)
{
    // Allocate enough space (it does not have any effect if the buffer
    // already has a capacity >= of rsize)
    buff.allocate( rsize );

    // Read the bytes into the buffer data array
    ssize_t nbytes;
    if ( ( nbytes = read( (char*)buff.getBuffer(), rsize ) ) < 0 )
    {
        return -1;
    }

    // Position the pointer after the read section
    buff.position( nbytes );
    return nbytes;
}

ssize_t StreamIO::write(const std::string &content)
{
    // Take the underlying buffer and write its content
    return write( content.c_str(), content.size() );
}
