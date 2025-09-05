#include "publisher.hpp"
#include <patterns/pub_sub/broker.hpp>

using namespace ccl::dp::pub_sub;

void Publisher::setBroker(const broker_ptr &broker)
{
    m_broker = std::weak_ptr( broker );
}

void Publisher::publish(event_ptr event) const
{
    if ( m_broker.expired() ) return;
    auto broker_ptr = m_broker.lock();
    broker_ptr->notifySubscribers( event );
}
