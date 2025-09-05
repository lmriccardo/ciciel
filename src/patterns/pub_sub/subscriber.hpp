#pragma once

#include <concurrent/thread.hpp>
#include <data_structures/queue/concurrent_queue.hpp>
#include <patterns/pub_sub/event.hpp>
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
     * concurrent queue and then asynchronously consumed. On cancellation
     * it injects a nullptr event to break the infinite running loop. Notice
     * that the subscriber will stop running once all elements left in the
     * queue are processed. That is why, the thread cancellation policy is
     * DEFERRED.
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
        AsyncSubscriber(const std::string& name) 
        : Thread( name, false, 
            ccl::sys::concurrent::CancellationPolicy::DEFERRED )
        {}

        AsyncSubscriber()
        : AsyncSubscriber( __FUNCTION__ )
        {}

        virtual ~AsyncSubscriber() = default;

        // Imports some methods from the Thread class
        using Thread::join;
        using Thread::start;
        
        void cancel(); // Custom cancel method for the AsyncSubscriber
        void stop();   // Custom stop method for AsyncSubscriber

        void run() override;
    };
}