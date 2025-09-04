#include "metrics_stack.hpp"

using namespace ccl::metrics;

MetricNode::MetricNode(const std::string &name, TID_t tid, Metrics &&metrics)
: m_f_name( name ), m_tid( tid ), m_metrics( std::move(metrics) )
{}

MetricsStack& MetricsStack::getInstance()
{
    static std::unique_ptr<MetricsStack> ptr{ new MetricsStack() };
    return *ptr;
}

void ccl::metrics::MetricsStack::gather( const MetricsCollector& collector )
{
    Metrics metrics = collector.getCollectedMetrics();
    std::string f_name = collector.getFuncName();
    int thread_id = (int)collector.getThreadId();

    // Remove the collector from the stack since it will be destroyed
    // after the end of this function
    auto& collectors_stack = getCollectorsByTid( collector.getThreadId() );
    collectors_stack.pop_back();

    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "( " << f_name << " ) "
              << "Total Duration: " << metrics.m_duration    << " [us] "
              << "CPU Time: "       << metrics.m_cpu_time    << " [us] "
              << "Stack Usage: "    << metrics.m_stack_usage << " [KB] "
              << "Heap Usage: "     << metrics.m_heap_usage  << " [KB] "
              << "Residual Mem: "   << metrics.m_heap_res    << " [KB]"
              << std::endl;

    std::cout << "----------------------------------------------------" << std::endl;
}

void MetricsStack::addMetricsCollector(const collector_ptr &collector)
{
    auto& collectors_stack = getCollectorsByTid( collector->getThreadId() );

    // The stack for the given thread is always accessed sequentially
    if ( !collectors_stack.empty() )
    {
        // First get the last collector and see if it still accessible
        collector_life_t& prev_coll_ref = collectors_stack.back();

        if ( auto prev_coll = prev_coll_ref.lock() )
        {
            prev_coll->setParent( collector->getFuncName() );
        }
    }

    collectors_stack.push_back( collector );
}

std::vector<MetricsStack::collector_life_t> &MetricsStack::getCollectorsByTid(int tid)
{
    std::lock_guard<std::mutex> _l( m_mutex );
    return m_metrics_map[ tid ];
}
