#pragma once

#include <string>
#include <memory>

namespace ccl::dp::pub_sub
{
    /**
     * @class PubSubEvent
     * 
     * @brief A generic publish-subscribe event published by
     * the Publisher (or the Broker, if it acts as both). Given
     * the purely virtual nature, this class must be specified.
     */
    class PubSubEvent
    {
    public:
        virtual ~PubSubEvent() = default;

        /* Returns the name of the event */
        virtual std::string name() const = 0;

        /* Clone the current event to generate a mutable reference */
        inline virtual std::unique_ptr<PubSubEvent> clone() const
        {
            return nullptr; // Immutable events don't need cloning
        }
    };
}