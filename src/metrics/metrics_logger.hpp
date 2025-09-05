#pragma once

#include <concurrent/thread.hpp>
#include <metrics/metrics_stack.hpp>
#include <data_structures/queue/concurrent_circular_queue.hpp>
#include <unordered_map>

namespace ccl::metrics
{
    struct MetricEvent
    {
        std::string m_parent_func_name;
        std::string m_func_name;
        Metrics     m_metrics;
        TID_t       m_thread_id;
    };

    class AbstractMetricsLogger : ccl::sys::concurrent::Thread
    {
    protected:
        ccl::ds::queue::ConcurrentCQueue<MetricEvent> m_queue;

        using Thread::isCancelled;

    public:
        AbstractMetricsLogger( const std::string& name );
        
        virtual void run() override = 0;
        virtual void push( MetricsCollector& collector ) = 0;
    };

    class TreeMetricsLogger : public AbstractMetricsLogger
    {
    private:
        
    public:
        TreeMetricsLogger();
        
        void run() override;
        void push( MetricsCollector& collector );
    };
};