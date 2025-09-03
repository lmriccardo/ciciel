#pragma once

#include <metrics/metrics_gather.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>

namespace ccl::metrics
{
    /**
     * @class MetricsLogger
     */
    class MetricsLogger
    {
        friend class MetricsCollector;

    private:
        using collector_life_t = typename std::weak_ptr<MetricsCollector>;
        using collector_ptr = typename std::shared_ptr<MetricsCollector>;
        
        std::unordered_map<int, std::vector<collector_life_t>> m_metrics_map;
        std::mutex m_mutex;

        /**
         * @brief Gather the metrics from the last collector of the input TID
         */
        void gather( const MetricsCollector& collector );

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
        MetricsLogger() = default;

        MetricsLogger( const MetricsLogger& ) = delete;
        MetricsLogger& operator=( const MetricsLogger& ) = delete;

        static MetricsLogger& getInstance();
    };
}