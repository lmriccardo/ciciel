#include "thread.hpp"

using namespace ccl::sys::concurrent;

Thread::Thread()
    : m_policy( CancellationPolicy::DEFERRED ), m_name( "GenericThread" ), m_daemon( false )
{
}

Thread::Thread(const std::string &name, bool daemon, CancellationPolicy policy)
    : m_policy( policy ), m_name( name ), m_daemon( daemon )
{
}

Thread::~Thread()
{
    cancel();
    join();
}

bool Thread::isJoinable() const
{
    return m_thread != nullptr && m_thread->joinable() && !m_daemon;
}

bool Thread::isDaemon() const
{
    return m_daemon;
}

bool Thread::isStarted() const
{
    return m_started;
}

bool Thread::isCancelled() const
{
    return m_cancelled;
}

void Thread::setCancellationPolicy(CancellationPolicy policy)
{
    m_policy = policy;
}

CancellationPolicy Thread::getCancellationPolicy() const
{
    return m_policy;
}

const std::string& Thread::getName() const
{
    return m_name;
}

std::thread::id Thread::getId() const
{
    return m_id;
}

void Thread::join()
{
    if ( isJoinable() )
    {
        m_thread->join();
        tearDown();
    }
}

void Thread::detach()
{
    if (!m_cancelled && m_thread != nullptr)
    {
        m_thread->detach();
        m_daemon = true;
    }
}

void Thread::start()
{
    if (!m_started)
    {
        m_thread = std::make_unique<std::thread>(&Thread::run, this);
        m_id = m_thread->get_id();
        m_started.store(true);

        // Detach if the thread is a daemon or not
        if ( isDaemon() )
        {
            detach();
        }
    }
}

void Thread::cancel()
{
    if ( m_thread != nullptr && !m_cancelled && m_started )
    {
        if ( m_policy == CancellationPolicy::DEFERRED ) return;
        if ( m_policy == CancellationPolicy::AT_CANCELLATION_POINT )
        {
            pthread_cancel( m_thread->native_handle() );
        }

        m_cancelled.store( true );
    }
}

void Thread::stop()
{
    cancel();
    join();
}

void Thread::setAffinity(int cpu)
{
    if (m_thread != nullptr)
    {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(cpu, &cpuset);

        pthread_t thread = m_thread->native_handle();
        if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) != 0)
        {
            std::cerr << "Failed to set thread affinity: "
                      << std::strerror(errno)
                      << std::endl;
        }
    }
}