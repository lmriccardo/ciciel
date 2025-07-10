#pragma once

#include <cstddef>
#include <cstdlib>
#include <sstream>
#include "io_handle.hpp"

#include <data_structures/buffers/byte_buffer.hpp>

#ifdef _WIN32
    #include <windows.h>
    typedef HANDLE NativeHandleType;
#else
    typedef int NativeHandleType;
#endif

namespace ccl::sys::io
{
    /**
     * @brief A base class for stream-based IO operations.
     * This class provides a common interface for reading and writing to streams,
     * such as files or network sockets, using a native handle.
     * It is designed to be extended by specific stream implementations.
     */
    class StreamIO
    {
    protected:
        StreamHandler<NativeHandleType> m_handler;
        
        size_t m_bytes_read = 0;
        size_t m_bytes_write = 0;

        explicit StreamIO( NativeHandleType handle );

    public:
        StreamIO() = default;

        explicit StreamIO( StreamHandler<NativeHandleType>&& handler );
        virtual ~StreamIO() = default;

        StreamIO( const StreamIO& other ) = delete;
        StreamIO& operator=( const StreamIO& other ) = delete;

        StreamIO( StreamIO&& other );
        StreamIO& operator=( StreamIO&& other );

        /**
         * Returns the native handle associated with this StreamIO object.
         * @return The native handle (file descriptor or Windows HANDLE).
         * @note The handle is not guaranteed to be valid; use isOpen() to check
         */
        NativeHandleType getNativeHandle() const;

        /**
         * Closes the stream and releases the associated handle.
         * After calling this method, the StreamIO object should not be used.
         */
        void close();

        virtual ssize_t write( const char* buff, size_t nbytes ) = 0;
        virtual ssize_t read( char* dst, size_t rsize ) = 0;
        
        /**
         * Reads the input number of bytes into the destination string.
         * Returns -1 on error, >= 0 otherwise.
         * 
         * @param dst The destination string
         * @param rsize The number of bytes to read
         * @return The actual number of read bytes
         */
        ssize_t read( std::string& dst, size_t rsize );

        /**
         * Reads the input number of bytes into the ByteBuffer. Returns -1
         * on error, >= 0 otherwise. If the capacity of the input ByteBuffer
         * is less than the number of bytes to read, then new space will be
         * allocated. After the read, the pointer inside the ByteBuffer will
         * points at a position after the read section.
         * 
         * @param buff The ByteBuffer destination
         * @param rsize The number of bytes to read
         * @return The total number of bytes read
         */
        ssize_t read( ds::buffers::ByteBuffer& buff, size_t rsize );

        /**
         * Writes the input string into the file and returns the total number of
         * bytes actually written. Returns -1 for any error, >= 0 otherwise.
         * 
         * @param content The line to be written
         * @return The total number of bytes written
         */
        ssize_t write( std::string& content );
    };
}