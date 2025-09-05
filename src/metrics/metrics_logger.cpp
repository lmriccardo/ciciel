#include "metrics_logger.hpp"

using namespace ccl::metrics;

namespace concurrent = ccl::sys::concurrent;
namespace queue = ccl::ds::queue;

AbstractMetricsLogger::AbstractMetricsLogger( const std::string& name )
: Thread( name ), m_queue( 100 )
{
    setCancellationPolicy( concurrent::CancellationPolicy::AT_CONDITION_CHECK );
}

ccl::metrics::TreeMetricsLogger::TreeMetricsLogger()
: AbstractMetricsLogger( "TreeMetricsLogger" )
{}

void ccl::metrics::TreeMetricsLogger::run()
{
    while ( !isCancelled() )
    {
        MetricEvent elem = m_queue.pop();
    }
}

void TreeMetricsLogger::push(MetricsCollector &collector)
{
    MetricEvent element;

    element.m_func_name = collector.getFuncName();
    element.m_metrics = collector.getCollectedMetrics();
    element.m_parent_func_name = collector.getParentName();
    element.m_thread_id = collector.getThreadId();

    m_queue.push( std::move(element) );
}
