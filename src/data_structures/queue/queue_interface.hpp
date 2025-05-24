#pragma once

namespace ccl::ds
{
    template <typename T>
    class QueueInterface
    {
    public:
        virtual ~QueueInterface() = default;

        virtual size_t size    () const = 0;
        virtual bool   empty   () const = 0;
        virtual size_t capacity() const = 0;
        virtual bool   full    () const = 0;

        virtual void     push(const T&)    = 0;
        virtual void     push(T&&)         = 0;
        virtual bool     tryPush(const T&) = 0;
        virtual bool     tryPush(T&&)      = 0;
        virtual T        pop()             = 0;
        virtual void     pop(T&)           = 0;
        virtual bool     tryPop(T&)        = 0;
        virtual const T& peek()      const = 0;
    };

    template <typename T>
    class RandomAccessQueueInterface : public QueueInterface<T>
    {
    public:
        virtual ~RandomAccessQueueInterface() = default;
        virtual const T& at(size_t) const = 0;
    };
}