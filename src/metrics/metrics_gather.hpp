#pragma once

#include <fstream>
#include <string>
#include <chrono>
#include <sys/resource.h>
#include <iostream>
#include <regex>
#include <unistd.h>
#include <sys/syscall.h>
#include <memory>

namespace ccl::metrics
{
    class MetricsLogger; // Forward declaration of MetricsLogger

    /**
     * @brief Collects the metric specified by the input Key for the current process
     * using the /proc/self/status file on Linux and Windows specific functions
     * on the other hand.
     * 
     * @param key The specific metric to collect
     * @return The value of the metric
     */
    long get_proc_value( const std::string& key );

    /**
     * @brief Returns the current user CPU time of the calling process
     */
    long get_cpu_time();

    /**
     * @brief Returns the current stack usage, using the special 'maps' file.
     * In particular, stack pointers for the current thread is given by the file
     * /proc/self/task/<TID>/maps.
     * 
     * @param tid The current TID (Thread ID) 
     */
    long get_stack_usage( int tid );

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
        friend class MetricsLogger;

    private:
        
        std::string m_function_name; // The name of the entered function
        pid_t       m_thread_id;     // The Thread ID of the metrics collector
        Metrics     m_metrics;       // Collected metrics

        explicit MetricsCollector( const std::string& func_name );

        Metrics getCollectedMetrics() const; // Returns collected metrics
        pid_t getThreadId() const; // Returns the Thread ID of the metrics collector
        std::string getFuncName() const; // Returns the function name

    public:
        ~MetricsCollector();

        // Creates a MetricsCollector and adds it to the MetricsLogger
        static std::shared_ptr<MetricsCollector> create( const std::string& f_name );

        // Collects all metrics up to the point the function is called
        void collect();
    };
}
