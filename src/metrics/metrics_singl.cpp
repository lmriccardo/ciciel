#include "metrics_singl.hpp"

using namespace ccl::metrics;

MetricsLogger& MetricsLogger::getInstance()
{
    static std::unique_ptr<MetricsLogger> ptr{ new MetricsLogger() };
    return *ptr;
}

void ccl::metrics::MetricsLogger::gather( const MetricsCollector& collector )
{
    Metrics metrics = collector.getCollectedMetrics();
    std::string f_name = collector.getFuncName();

    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "( " << f_name << " ) "
              << "Total Duration: " << metrics.m_duration << " [us] "
              << "CPU Time: " << metrics.m_cpu_time << " [us] "
              << "Stack Usage: " << metrics.m_stack_usage << " [KB] "
              << "Heap Usage: " << metrics.m_heap_usage << " [KB] "
              << "Residual Mem: " << metrics.m_heap_res << " [KB]"
              << std::endl;

    std::cout << "----------------------------------------------------" << std::endl;
}

void MetricsLogger::addMetricsCollector(const collector_ptr &collector)
{
    auto& collectors_stack = getCollectorsByTid( collector->getThreadId() );

    // The stack for the given thread is always accessed sequentially
    if ( !collectors_stack.empty() )
    {
        // First get the last collector and see if it still accessible
        collector_life_t& prev_coll_ref = collectors_stack.back();
        if ( auto prev_coll = prev_coll_ref.lock() )
        {
            
        }
        else
        {
            // Pop the last element if it is not accessible
            collectors_stack.pop_back();
        }
    }

    collectors_stack.push_back( collector );
}

std::vector<MetricsLogger::collector_life_t> &MetricsLogger::getCollectorsByTid(int tid)
{
    std::lock_guard<std::mutex> _l( m_mutex );
    return m_metrics_map[ tid ];
}
