#include "subscriber.hpp"

using namespace ccl::dp::pub_sub;

void AsyncSubscriber::notify(event_ptr &event)
{
    m_events.push( event );
}

void AsyncSubscriber::cancel()
{
    Thread::cancel();
    m_events.push( nullptr );
}

void AsyncSubscriber::stop()
{
    cancel();
    join();
}

void AsyncSubscriber::run()
{
    while ( true )
    {
        event_ptr event = m_events.pop();
        if (!event) break;
        consume( event );
    }
}
