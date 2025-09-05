#include "metrics_logger.hpp"

using namespace ccl::metrics;

namespace ps = ccl::dp::pub_sub;

TreeMetricsLogger::TreeMetricsLogger()
: AbstractMetricsLogger(__FUNCTION__)
{}

void TreeMetricsLogger::consume(event_ptr event)
{
    auto metric_ev = std::static_pointer_cast<const MetricEvent>(event);
    
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "( " << metric_ev->m_func_name << " ) "
              << "Total Duration: " << metric_ev->m_metrics.m_duration    << " [us] "
              << "CPU Time: "       << metric_ev->m_metrics.m_cpu_time    << " [us] "
              << "Stack Usage: "    << metric_ev->m_metrics.m_stack_usage << " [KB] "
              << "Heap Usage: "     << metric_ev->m_metrics.m_heap_usage  << " [KB] "
              << "Residual Mem: "   << metric_ev->m_metrics.m_heap_res    << " [KB]"
              << std::endl;

    std::cout << "----------------------------------------------------" << std::endl;
}