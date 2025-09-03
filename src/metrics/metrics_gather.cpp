#include "metrics_gather.hpp"
#include <metrics/metrics_singl.hpp>

using namespace ccl::metrics;

long ccl::metrics::get_proc_value(const std::string &key)
{
#ifndef _WIN32

    std::ifstream file( "/proc/self/status" ); // Open the /proc/self/status file
    std::string curr_line;
    
    // Iterate for each line in the file and get the one
    // starting with the input key.
    while ( std::getline( file, curr_line ) )
    {
        if ( curr_line.rfind( key, 0 ) != 0 ) continue;

        // Gather only the numeric information associated with
        // the input key. 
        std::string value_str;
        for ( char ch : curr_line )
        {
            if ( !std::isdigit(ch) ) continue;
            value_str += ch;
        }

        // If there is any results, converts it into long and returns
        if (!value_str.empty()) return std::stol(value_str);
    }

#else

    // Windows Implementation

#endif

    return 0;
}

long ccl::metrics::get_cpu_time()
{
#ifndef _WIN32

    struct rusage usage;

    // Get the resource usage for the current process (RUSAGE_SELF).
    if (getrusage(RUSAGE_SELF, &usage) == -1) return 0;

    // Return the sum of user CPU time (ru_utime) and system CPU time (ru_stime)
    // in microseconds, which provides better resolution.
    return (usage.ru_utime.tv_sec * 1000000) + usage.ru_utime.tv_usec +
        (usage.ru_stime.tv_sec * 1000000) + usage.ru_stime.tv_usec;

#else

    // Windows Implementation

#endif
}

long ccl::metrics::get_stack_usage( int tid )
{
#ifndef _WIN32

    std::string file_name = "/proc/self/task/" + std::to_string( tid ) + "/maps";
    std::ifstream maps_file( file_name );
    std::string curr_line;
    long stack_high = 0;

    // Find the base address of the stack
    while ( std::getline( maps_file, curr_line ) )
    {
        if ( curr_line.find( "[stack]" ) != std::string::npos )
        {
            // The stack address is given in the format <start>-<end> both
            // hexadecimal numbers. We can use regex with capturing groups
            // to extract both addresses and keeps only the first one.
            std::regex reg_exp("([0-9a-f]+)-([0-9a-f]+)");
            std::smatch match;
            if (std::regex_search(curr_line, match, reg_exp) && match.size() > 1) {
                stack_high = std::stoul(match.str(2), nullptr, 16);
                break;
            }
        }
    }

    // Get the current stack pointer's address using a local variable
    char dummy_on_stack;
    long current_sp = reinterpret_cast<long>(&dummy_on_stack);

    return stack_high - current_sp;

#else

    // Windows Implementation

#endif
}

MetricsCollector::MetricsCollector(const std::string &func_name)
    : m_function_name(func_name), m_thread_id( syscall(SYS_gettid) )
{
    m_metrics.m_start_time = std::chrono::high_resolution_clock::now();
    m_metrics.m_timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
        m_metrics.m_start_time.time_since_epoch()).count();
    
    m_metrics.m_cpu_time = get_cpu_time();
    m_metrics.m_stack_usage = get_stack_usage( m_thread_id );
    m_metrics.m_heap_usage = get_proc_value("VmData");
    m_metrics.m_heap_res = m_metrics.m_heap_usage;
}   

ccl::metrics::MetricsCollector::~MetricsCollector()
{
    long final_heap_res = get_proc_value("VmData");
    m_metrics.m_heap_res = final_heap_res - m_metrics.m_heap_res;

    // Now the logger has a copy of the metrics for this collector
    MetricsLogger::getInstance().gather( *this );
}

pid_t MetricsCollector::getThreadId() const
{
    return m_thread_id;
}

std::string MetricsCollector::getFuncName() const
{
    return m_function_name;
}

Metrics MetricsCollector::getCollectedMetrics() const
{
    return m_metrics;
}

void MetricsCollector::collect()
{

    // Gather final metrics
    auto end_time = std::chrono::high_resolution_clock::now();
    unsigned long final_cpu_time = get_cpu_time();
    long final_stack_usage = get_stack_usage( m_thread_id );
    long final_heap_usage = get_proc_value("VmData");

    // Compute deltas for the current function
    m_metrics.m_duration = std::chrono::duration_cast<std::chrono::microseconds>
        ( end_time - m_metrics.m_start_time ).count();

    m_metrics.m_cpu_time = final_cpu_time - m_metrics.m_cpu_time;
    m_metrics.m_stack_usage = std::max(final_stack_usage, m_metrics.m_stack_usage) / 1024;
    m_metrics.m_heap_usage = final_heap_usage - m_metrics.m_heap_usage;
}

std::shared_ptr<MetricsCollector> MetricsCollector::create(const std::string &f_name)
{
    auto collector = std::make_shared<MetricsCollector>( f_name );
    MetricsLogger::getInstance().addMetricsCollector( collector );
    return collector;
}
