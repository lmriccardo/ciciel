#include <sys/concurrent/thread.hpp>

using namespace ccl::sys::concurrent;

int main()
{
    Thread_ptr t = Thread::start( [](int c) {
        for ( int a = 0; a < c; ++a )
        {
            std::cout << "Thread ID: " << std::this_thread::get_id() 
                      << " count " << a 
                      << std::endl;
        }
    }, false, CancellationPolicy::DEFERRED, 100000 );

    t->join();

    return 0;
}