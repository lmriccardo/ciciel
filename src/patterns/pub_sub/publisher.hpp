#pragma once

#include <patterns/pub_sub/event.hpp>
#include <memory>

namespace ccl::dp::pub_sub
{
    class PubSubBroker; // Forward declaration of the Broker

    /**
     * @class Publisher
     * 
     * @brief Represents a source of events in a Pub-Sub system.
     * 
     * The Publisher is responsible for sending events to a PubSubBroker,
     * without knowing about individual subscribers. Events are shared 
     * via std::shared_ptr<const PubSubEvent> to ensure immutability and 
     * safe asynchronous consumption.
     * 
     * Key points:
     * - Holds a weak reference to a broker (std::weak_ptr<PubSubBroker>) to 
     *   avoid circular ownership and allow the broker to be safely destroyed.
     * - Can set or change the associated broker at runtime using setBroker().
     * - Publishes events via publish(), delegating delivery to the broker.
     * 
     * This class decouples event generation from event consumption, following 
     * standard Pub-Sub principles.
     */
    class Publisher
    {
    private:
        using event_ptr = typename std::shared_ptr<const PubSubEvent>;
        using broker_life_token = typename std::weak_ptr<PubSubBroker>;
        using broker_ptr = typename std::shared_ptr<PubSubBroker>;

        broker_life_token m_broker; // Weak reference to the broker
    
    public:
        Publisher() = default;
        Publisher( const broker_ptr& broker )
        : m_broker( broker )
        {}

        virtual ~Publisher() = default;

        /* Set the broker for this publisher */
        void setBroker( const broker_ptr& broker );

        /* Publish an Event to the broker */
        void publish( event_ptr event ) const;
    };
}