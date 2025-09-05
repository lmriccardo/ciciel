#include "pub_sub_broker.hpp"

void ccl::dp::pub_sub::PubSubBroker::notifySubscribers(event_ptr event)
{
    // First I need to lock for thread-safetiness
    std::lock_guard<std::mutex> _l( m_mutex );

    // In this case, the key of the subscriber map is directly taken
    // from the type of the input event
    auto key = std::type_index( typeid(*event) );

    if ( m_subscribers.find(key) != m_subscribers.end() )
    {
        auto& subscribers = m_subscribers[key];
        
        // Cycle for each subscriber and notify with the event, if the
        // weak reference is not expired. In that case, the subscriber
        // is removed.
        auto vec_it = subscribers.begin();
        while ( vec_it != subscribers.end() )
        {
            if ( !vec_it->expired() )
            {
                auto sub_ptr = vec_it->lock();
                sub_ptr->notify( event );
                vec_it++;
                continue;
            }

            vec_it = subscribers.erase(vec_it);
        }
    }
}