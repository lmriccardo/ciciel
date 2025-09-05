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

void AsyncSubscriber::run()
{
    while ( !isCancelled() )
    {
        event_ptr event = m_events.pop();
        if (!event) break;
        consume( event );
    }
}
