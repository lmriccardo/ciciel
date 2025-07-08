#pragma once

#include <cstddef>
#include <cstdlib>
#include "io_handle.hpp"

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
         * Checks if the stream is open.
         * @return true if the stream is open, false otherwise.
         */
        bool isOpen() const;

        /**
         * Closes the stream and releases the associated handle.
         * After calling this method, the StreamIO object should not be used.
         */
        void close();

        virtual ssize_t write( const char* buff, size_t nbytes ) const = 0;
        virtual ssize_t read( char* dst, size_t rsize ) const = 0;
    };
}