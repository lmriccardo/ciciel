#pragma once

#include <data_structures/base/indexable_interface.hpp>

namespace ccl::ds::queue
{
    /**
     * A generic interface for all queues. 
     */
    template <typename T>
    class QueueInterface
    {
    public:
        virtual ~QueueInterface() = default;

        virtual size_t size    () const = 0;
        virtual bool   empty   () const = 0;
        virtual size_t capacity() const = 0;
        virtual bool   full    () const = 0;

        virtual void push(const T&)    = 0;
        virtual void push(T&&)         = 0;
        virtual bool tryPush(const T&) = 0;
        virtual bool tryPush(T&&)      = 0;

        virtual T    pop   ()         = 0;
        virtual void pop   (T&)       = 0;
        virtual bool tryPop(T&)       = 0;
        virtual bool peek  (T&) const = 0;
    };
}