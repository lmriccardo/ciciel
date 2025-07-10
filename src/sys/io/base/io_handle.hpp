#pragma once

#include <utility>
#include <type_traits>

#ifdef __unix__

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#endif

#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

namespace ccl::sys::io
{
    template <typename T>
    struct NativeHandleTrait;

    /**
     * Specialization for POSIX file descriptors
     * Note: POSIX file descriptors are integers, typically starting from 0.
     * The invalid handle value is defined as -1.
     */
    template<>
    struct NativeHandleTrait<int>
    {
        static constexpr int invalid_handle = -1;
        static void close( int fd );
        static bool is_valid( int fd );
    };

#ifdef _WIN32
    // Specialization for Windows HANDLE
    // Note: HANDLE is a typedef for void* in Windows API
    // and is used for various types of handles, including file handles.
    // It is not a direct equivalent of file descriptors in Unix-like systems.
    // The invalid handle value is defined as INVALID_HANDLE_VALUE.
    template <>
    struct NativeHandleTrait<HANDLE>
    {
        static constexpr HANDLE invalid_handle = INVALID_HANDLE_VALUE;
        static void close( HANDLE h );
        static bool is_valid( HANDLE h );
    };

#endif

    /**
     * @brief A class template that provides a handle for IO operations.
     * This class is designed to manage the lifecycle of a native handle
     * (like a file descriptor or a Windows HANDLE) and ensure that it is
     * properly closed when the object is destroyed.
     * 
     * It follows the RAII principle, meaning that the handle is automatically
     * released when the StreamHandler object goes out of scope or is destroyed.
     */
    template <typename T>
    class StreamHandler
    {
    private:
        T m_handle; // The handle associated with the IO

    public:
        StreamHandler() : m_handle(NativeHandleTrait<T>::invalid_handle) {}
        explicit StreamHandler(T handle) : m_handle(handle) {}
        virtual ~StreamHandler()
        {
            if (isValid())
            {
                NativeHandleTrait<T>::close(m_handle);
            }
        }

        StreamHandler(const StreamHandler&) = delete;
        StreamHandler& operator=(const StreamHandler&) = delete;

        StreamHandler(StreamHandler&& other) noexcept 
            : m_handle(std::exchange(other.m_handle, NativeHandleTrait<T>::invalid_handle))
        {}
        
        StreamHandler& operator=(StreamHandler&& other) noexcept
        {
            if (this != &other)
            {
                if (isValid())
                {
                    NativeHandleTrait<T>::close(m_handle);
                }

                // Take the ownership of the handle from the other object
                m_handle = other.release();
            }

            return *this;
        }
        
        bool isValid() const
        {
            return NativeHandleTrait<T>::is_valid(m_handle);
        }

        T get() const
        {
            return m_handle;
        }

        void reset(T newHandle = NativeHandleTrait<T>::invalid_handle)
        {
            if (isValid())
            {
                NativeHandleTrait<T>::close(m_handle);
            }

            m_handle = newHandle;
        }

        T release() noexcept
        {
            return std::exchange(m_handle, NativeHandleTrait<T>::invalid_handle);
        }
    };

    using PosixFileDescriptor = StreamHandler<int>;

#ifdef _WIN32
    using WindowsHandle = StreamHandler<HANDLE>;
#endif
}