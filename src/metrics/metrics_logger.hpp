#pragma once

#include <metrics/metrics_broker.hpp>
#include <patterns/pub_sub/event.hpp>
#include <patterns/pub_sub/subscriber.hpp>
#include <unordered_map>

namespace ccl::metrics
{
    /**
     * @class AbstractMetricsLogger
     *
     * @brief Abstract base class for asynchronous metrics loggers in the Pub-Sub system.
     *
     * This class extends AsyncSubscriber to consume metric events asynchronously.
     * It provides an interface for logging or processing function-level metrics
     * delivered via MetricEvent instances.
     */
    class AbstractMetricsLogger : public ccl::dp::pub_sub::AsyncSubscriber
    {
    public:
        AbstractMetricsLogger( const std::string& name )
        : AsyncSubscriber( name )
        {}

        virtual ~AbstractMetricsLogger() = default;

        virtual void consume(event_ptr event) = 0;
    };
    
    class TreeMetricsLogger : public AbstractMetricsLogger
    {
    private:
        // TODO: Add metrics tree
        
    public:
        TreeMetricsLogger();
        
        virtual ~TreeMetricsLogger() 
        { AsyncSubscriber::stop(); }
        
        void consume(event_ptr event) override;
    };
};