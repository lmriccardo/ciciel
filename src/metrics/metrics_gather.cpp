#include "metrics_gather.hpp"
#include <metrics/metrics_stack.hpp>

using namespace ccl::metrics;

MetricsCollector::MetricsCollector(const std::string &func_name)
    : m_function_name(func_name), m_thread_id( syscall(SYS_gettid) )
{
    m_metrics.m_start_time = std::chrono::high_resolution_clock::now();
    m_metrics.m_timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
        m_metrics.m_start_time.time_since_epoch()).count();
    
    m_metrics.m_cpu_time = get_cpu_time();
    m_metrics.m_stack_usage = get_stack_usage( m_thread_id );
    m_metrics.m_heap_usage = get_heap_usage();
    m_metrics.m_heap_res = m_metrics.m_heap_usage;
}   

ccl::metrics::MetricsCollector::~MetricsCollector()
{
    long final_heap_res = get_heap_usage();
    m_metrics.m_heap_res = final_heap_res - m_metrics.m_heap_res;

    // Now the logger has a copy of the metrics for this collector
    MetricsStack::getInstance().gather( *this );
}

TID_t MetricsCollector::getThreadId() const
{
    return m_thread_id;
}

std::string MetricsCollector::getFuncName() const
{
    return m_function_name;
}

std::string ccl::metrics::MetricsCollector::getParentName() const
{
    return m_parent;
}

void MetricsCollector::setParent(const std::string &name)
{
    m_parent = name;
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
    long final_heap_usage = get_heap_usage();

    // Compute deltas for the current function
    m_metrics.m_duration = std::chrono::duration_cast<std::chrono::microseconds>
        ( end_time - m_metrics.m_start_time ).count();

    m_metrics.m_cpu_time = final_cpu_time - m_metrics.m_cpu_time;
    m_metrics.m_stack_usage = std::max(final_stack_usage, m_metrics.m_stack_usage) / 1024;
    m_metrics.m_heap_usage = final_heap_usage - m_metrics.m_heap_usage;
}

std::shared_ptr<MetricsCollector> MetricsCollector::create(const std::string &f_name)
{
    auto collector = std::shared_ptr<MetricsCollector>( new MetricsCollector( f_name ) );
    MetricsStack::getInstance().addMetricsCollector( collector );
    return collector;
}
