#pragma once

#include <concurrent/thread.hpp>
#include <data_structures/queue/concurrent_queue.hpp>
#include <patterns/pub_sub/pub_sub_event.hpp>
#include <string>
#include <memory>

namespace ccl::dp::pub_sub
{
    class PubSubBroker; // Forward Declaration for friend class

    /**
     * @class Subscriber
     * 
     * @brief A PubSub Subscriber class, which can subscribe to a
     * broker and receives events. 
     */
    class Subscriber
    {
    protected:

        friend class PubSubBroker;

        using event_ptr = typename std::shared_ptr<const PubSubEvent>;

        /* Actions to perform when an event comes from the broker */
        virtual void notify(event_ptr& event) = 0;
        
    public:
        virtual ~Subscriber() = default;
    };

    /**
     * @class AsyncSubscriber
     * 
     * @brief Asynchronous Subscriber in which events are pushed into a
     * concurrent queue and then asynchronously consumed.
     */
    class AsyncSubscriber : public Subscriber, private ccl::sys::concurrent::Thread
    {
    protected:
        ccl::ds::queue::ConcurrentQueue<event_ptr> m_events; // Concurrent queue of events

        /* Push the event into the event queue */
        void notify( event_ptr& event ) override;

        /* Consume one single event from the event queue */
        virtual void consume( event_ptr event ) = 0;
    
    public:
        AsyncSubscriber() 
        : Thread( __FUNCTION__, false, 
            ccl::sys::concurrent::CancellationPolicy::AT_CONDITION_CHECK )
        {}

        // Imports some methods from the Thread class
        using Thread::join;
        using Thread::stop;
        using Thread::start;
        
        void cancel(); // Custom cancel method for the AsyncSubscriber

        virtual ~AsyncSubscriber() = default;

        void run() override;
    };
}