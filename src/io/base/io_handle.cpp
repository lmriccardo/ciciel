#include "io_handle.hpp"

using namespace ccl::sys::io;

void NativeHandleTrait<int>::close( int fd )
{
    // Check that the file handler is not invalid
    if ( fd != invalid_handle && is_valid( fd ) )
    {
#ifdef __unix__
        if ( ::close(fd) == -1 )
        {
            std::cerr << "Error when closing the file descriptor: " 
                      << fd 
                      << std::endl;
        }
#else
        throw std::runtime_error("Unknown handler for this OS");
#endif
    }
}

bool NativeHandleTrait<int>::is_valid(int fd)
{
    return fd >= 0 && (fcntl(fd, F_GETFD) != -1 || errno != EBADF);
}

#ifdef _WIN32
void NativeHandleTrait<HANDLE>::close( HANDLE h )
{
    // Check that the handle is not invalid
    if (h != invalid_handle && is_valid(h))
    {
        if (!::CloseHandle(h))
        {
            std::cerr << "Error when closing the handle: " 
                      << h 
                      << std::endl;
        }
    }
}

void NativeHandleTrait<HANDLE>::is_valid( HANDLE h )
{
    if (h == nullptr || h == INVALID_HANDLE_VALUE) return false;

    DWORD flags;
    return ::GetHandleInformation(h, &flags) != 0;
}
#endif
