#pragma once

#include <metrics/metrics_utils.hpp>
#include <patterns/pub_sub/publisher.hpp>
#include <patterns/pub_sub/event.hpp>
#include <fstream>
#include <string>
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

#ifndef _WIN32
#define TID_t pid_t
#else
#define TID_t int
#endif

namespace ccl::metrics
{
    class MetricsBroker; // Forward declaration of MetricsBroker
    class AbstractMetricsLogger; // Forward declaration of MetricsLogger

    /**
     * @struct Metrics
     * 
     * @brief Represents the collected metrics. In particular it contains
     * the total duration of the function in milliseconds, the total RAM
     * used by the function, and the total CPU time.
     */
    struct Metrics
    {
        using timep_t = typename std::chrono::time_point<std::chrono::high_resolution_clock>;

        timep_t       m_start_time;  // The time once the function is entered
        unsigned int  m_timestamp;   // The timestamp when the collector is created
        unsigned int  m_duration;    // The total duration of the function
        unsigned long m_cpu_time;    // The CPU Time at the end of the function
        long          m_stack_usage; // Usage of the Stack-allocated memory
        long          m_heap_usage;  // Usage of the Heap-allocated memory 
        long          m_heap_res;    // Heap residual memory
    };

    /**
     * @class MetricEvent
     *
     * @brief Represents a metrics-related event in the Pub-Sub system.
     *
     * This event is intended to carry function-level performance or resource
     * metrics, such as CPU usage, RAM consumption, execution time, and thread ID.
     * Typically published by a MetricsCollector to a PubSubBroker for consumption
     * by subscribers interested in performance monitoring or logging.
     */
    class MetricEvent : public ccl::dp::pub_sub::PubSubEvent
    {
    public:
        std::string m_parent_func_name; // Parent function name
        std::string m_func_name;        // Metric function name
        Metrics     m_metrics;          // Collection of Metrics (CPU time, RAM ...)
        TID_t       m_thread_id;        // Thread ID of the Collector

        MetricEvent( const std::string& parent, const std::string& f_name,
                     Metrics metrics, TID_t thread_id )
        : m_parent_func_name(parent), m_func_name(f_name),
          m_metrics(metrics), m_thread_id(thread_id)
        {}

        inline std::string name() const override
        {
            return "MetricEvent(" + m_func_name + ")";
        }
    };

    /**
     * @class MetricsCollector
     * 
     * @brief Collect metrics for a single function call, where the object
     * is constructed. It follows the RAII principle, meaning that metrics
     * are collected once the function is entered and exited.
     */
    class MetricsCollector : public ccl::dp::pub_sub::Publisher
    {
        friend class MetricsBroker;
        friend class AbstractMetricsLogger;

    private:
        
        std::string m_function_name; // The name of the entered function
        TID_t       m_thread_id;     // The Thread ID of the metrics collector
        Metrics     m_metrics;       // Collected metrics
        std::string m_parent;        // The parent metrics collector

        explicit MetricsCollector( const std::string& func_name );

        void setParent( const std::string& name ); // Set the parent function name

    public:
        ~MetricsCollector();

        // Creates a MetricsCollector and adds it to the MetricsLogger
        static std::shared_ptr<MetricsCollector> create( const std::string& f_name );

        // Collects all metrics up to the point the function is called
        void collect();

        Metrics     getCollectedMetrics() const; // Returns collected metrics
        TID_t       getThreadId() const; // Returns the Thread ID of the metrics collector
        std::string getFuncName() const; // Returns the function name
        std::string getParentName() const; // Returns the parent function name
    };
}
