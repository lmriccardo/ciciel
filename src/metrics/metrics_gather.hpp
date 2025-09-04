#pragma once

#include <metrics/metrics_utils.hpp>
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
    class MetricsStack; // Forward declaration of MetricsStack
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
     * @class MetricsCollector
     * 
     * @brief Collect metrics for a single function call, where the object
     * is constructed. It follows the RAII principle, meaning that metrics
     * are collected once the function is entered and exited.
     */
    class MetricsCollector : std::enable_shared_from_this<MetricsCollector>
    {
        friend class MetricsStack;
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
