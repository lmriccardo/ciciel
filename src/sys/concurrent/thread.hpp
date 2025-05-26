#pragma once

#include <thread>
#include <functional>
#include <string>
#include <memory>
#include <tuple>
#include <cstring>
#include <atomic>
#include <pthread.h>
#include <iostream>

namespace ccl::sys::concurrent
{
    enum class CancellationPolicy
    {
        AT_CANCELLATION_POINT, // When pthread_cancel is called
        AT_CONDITION_CHECK,    // When the thread checks for cancellation
        DEFERRED               // Thread cannot be cancel except internally
    };

    class Thread;
    typedef std::unique_ptr<Thread> Thread_ptr;

    /**
     * This class offers a wrapper around the common std::thread class already present
     * in the C++ standard library. It aims to offer better fine-grained support for
     * creating a thread, managing thread cancellation using cancellation policies and
     * tearDown logic, setting thread affinity and so on. It can also be the base class 
     * for custom Threads by just implementing the pure virtual "run" method. 
     * 
     * Note: as a normal (non-advanced) user prefer using one between DEFFERRED or 
     * AT_CONDITION_CHECK, over AT_CANCELLATION_POINT, since it can lead to undefined
     * or unwanted behavior. 
     * 
     * This is the meaning behind the cancellation policies:
     *  - DEFFERRED: The entire logic when the thread should stop is user-defined
     *  - AT_CONDITION_CHECK: The Thread should stop when isCancelled() returns True
     *  - AT_CANCELLATION_POINT: The thread is cancelled using the pthread_cancel C function
     *    which will takes effect once the thread has reached a valid cancellation point
     */
    class Thread
    {
    protected:
        std::unique_ptr<std::thread> m_thread = nullptr;
        CancellationPolicy           m_policy;

        std::thread::id m_id;
        std::string m_name;
        bool m_daemon;

        std::atomic<bool> m_cancelled = false;
        std::atomic<bool> m_started   = false;

        virtual void run() = 0;
        virtual void tearDown() {}; // Function executed when the thread is cancelled
    
    public:
        Thread();
        Thread( 
            const std::string& name, bool daemon = false,
            CancellationPolicy policy = CancellationPolicy::DEFERRED
        );

        // Delete the copy constructor and copy assignment operator
        Thread(const Thread &) = delete;
        Thread &operator=(const Thread &) = delete;

        Thread( Thread&& ) = default;
        Thread& operator=( Thread&& ) = default;

        virtual ~Thread();

        bool isJoinable () const;
        bool isDeamon   () const;
        bool isStarted  () const;
        bool isCancelled() const;

        void setCancellationPolicy( CancellationPolicy );
        CancellationPolicy getCancellationPolicy() const;

        const std::string& getName() const;
        std::thread::id getId() const;

        void join();
        void detach();

        template<typename _Callable, typename ..._Args>
        static Thread_ptr start( _Callable&&, bool, CancellationPolicy, _Args&& ...args );

        void start();
        void cancel();
        void stop();

        void setAffinity( int );
    };

    /**
     * This is a thread implementation for simple thread logic. It takes as input
     * a function with all parameters and just implements the run method. I would
     * like to point out that the only purpouse of this class is to use it as a
     * middle point in the static "start" method of the Thread class, therefore
     * it is suggested to not use it to create generic threads. Just use the "start"
     * method of Thread class.
     * 
     * @tparam _Callable The function type
     * @tparam _Args     Types for each function parameter
     */
    template<typename _Callable, typename ..._Args>
    class ThreadImpl : public Thread
    {
    private:
        _Callable            m_fun;  // Callable function
        std::tuple<_Args...> m_args; // Arguments to the callable

        void run() override 
        {
            std::apply( m_fun, m_args );
        }

    public:
        ThreadImpl( _Callable&&, _Args&&... );
    };

    template <typename _Callable, typename... _Args>
    inline ThreadImpl<_Callable, _Args...>::ThreadImpl(_Callable &&fun, _Args &&...args)
        : m_fun( std::forward<_Callable>(fun) ), m_args( std::forward<_Args>(args)... )
    {}

    template <typename _Callable, typename... _Args>
    inline Thread_ptr Thread::start(_Callable &&fun, bool daemon, 
        CancellationPolicy p, _Args &&...args)
    {
        Thread_ptr curr_thread = std::make_unique<ThreadImpl<_Callable, _Args...>>( 
            std::forward<_Callable>(fun), std::forward<_Args>(args)... );

        curr_thread->start();
        curr_thread->setCancellationPolicy( p );

        if ( daemon ) curr_thread->detach();

        return std::move(curr_thread);
    }
}