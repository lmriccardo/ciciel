#include "metrics_broker.hpp"

using namespace ccl::metrics;

std::shared_ptr<MetricsBroker> MetricsBroker::getInstance()
{
    static auto ptr = std::make_shared<MetricsBroker>();
    return ptr;
}

void MetricsBroker::addMetricsCollector(const collector_ptr &collector)
{
    auto& collectors_stack = getCollectorsByTid( collector->getThreadId() );

    // The stack for the given thread is always accessed sequentially
    // therefore, we do not need to lock something else here.

    // First, we need to check if the last element of the stack, if not empty,
    // is still accessible or not. If it is not, then we remove it.
    if ( !collectors_stack.empty() && collectors_stack.back().expired() )
    {
        collectors_stack.pop_back();
    }

    // If there are still elements in the stack, then take the last element and
    // set the input collector as its child.
    if ( !collectors_stack.empty() )
    {
        // First get the last collector and see if it still accessible
        collector_life_t prev_coll_ref = collectors_stack.back();

        if ( auto prev_coll = prev_coll_ref.lock() )
        {
            prev_coll->setParent( collector->getFuncName() );
        }
    }

    collectors_stack.push_back( collector );
}

std::vector<MetricsBroker::collector_life_t> &MetricsBroker::getCollectorsByTid(int tid)
{
    std::lock_guard<std::mutex> _l( m_mutex );
    return m_metrics_map[ tid ];
}