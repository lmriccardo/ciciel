#pragma once

#include <patterns/pub_sub/subscriber.hpp>
#include <patterns/pub_sub/publisher.hpp>
#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <type_traits>
#include <memory>
#include <vector>

namespace ccl::dp::pub_sub
{
    class PubSubBroker
    {
    protected:
        using subscriber_ptr = std::shared_ptr<Subscriber>;
        using subscriber_life_tkn = std::weak_ptr<Subscriber>;
        using collection = std::vector<subscriber_life_tkn>;
        using subscriber_map_t = std::unordered_map<std::type_index, collection>;
        using event_ptr = std::shared_ptr<const PubSubEvent>;

        /**
         * This map contains all the subscribers of this broker, categorized
         * based on event preferences.
         */
        subscriber_map_t m_subscribers;
        std::mutex       m_mutex; // To ensure thread-safetiness

    public:
        virtual ~PubSubBroker() = default;

        /**
         * @brief Subscribe the input subscriber to a list of one or more Events
         * @param subscriber The input subscriber
         * @tparam Events A List of events
         */
        template <typename... Events>
        void subscribe( const subscriber_ptr& subscriber );

        /**
         * @brief Unsubscribe the input subscriber from a list of one or more Events
         * @param subscriber The input subscriber
         * @tparam Events A List of events
         */
        template <typename... Events>
        void unsubscribe( const subscriber_ptr& subscriber );

        /**
         * @brief This function takes as input an event and notify all
         * linked subscribers.
         */
        void notifySubscribers( event_ptr event );
    };

    template <typename... Events>
    inline void PubSubBroker::subscribe(const subscriber_ptr &subscriber)
    {
        std::lock_guard<std::mutex> _l( m_mutex );
        
        // Fold expression to handle all event types
        (m_subscribers[std::type_index(typeid(Events))].push_back(subscriber), ...);
    }

    template <typename... Events>
    inline void PubSubBroker::unsubscribe(const subscriber_ptr &subscriber)
    {
        std::lock_guard<std::mutex> _l( m_mutex );

        // Fold expression to handle unsubscribes
        (([this, &subscriber]()
          {
            // If the key exists
            auto key = std::type_index(typeid(Events));
            if ( m_subscribers.find( key ) != m_subscribers.end() )
            {
                collection& vec = m_subscribers[key];

                // This will remove all expired subscribers and also
                // the one given to the unsubscribe function.
                vec.erase(std::remove_if(
                    vec.begin(), vec.end(),
                    [&subscriber]( const subscriber_life_tkn& tkn )
                    {
                        auto ptr = tkn.lock();
                        return !ptr || ( ptr == subscriber );
                    }
                ));
            }
          }
        )(), ...);
    }
}