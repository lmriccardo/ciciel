#pragma once

#include <metrics/metrics_gather.hpp>
#include <patterns/pub_sub/broker.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <iterator>

namespace ccl::metrics
{
    /**
     * @class MetricsBroker
     */
    class MetricsBroker : public ccl::dp::pub_sub::PubSubBroker
    {
        friend class MetricsCollector;

    private:
        using collector_life_t = typename std::weak_ptr<MetricsCollector>;
        using collector_ptr = typename std::shared_ptr<MetricsCollector>;
        
        std::unordered_map<int, std::vector<collector_life_t>> m_metrics_map;
        std::mutex m_mutex;

        /**
         * @brief Add a new metric collector on the stack
         */
        void addMetricsCollector( const collector_ptr& collector );

        /**
         * @brief Returns the collectors stack given the Thread ID
         * @param tid The current Thread ID
         * @return The collectors Stack
         */
        std::vector<collector_life_t>& getCollectorsByTid( int tid );

    public:
        MetricsBroker() = default;

        MetricsBroker( const MetricsBroker& ) = delete;
        MetricsBroker& operator=( const MetricsBroker& ) = delete;

        /**
         * @brief Create a unique instance of the MetricsBroker object
         * and returns a reference to it.
         */
        static std::shared_ptr<MetricsBroker> getInstance();
    };
}