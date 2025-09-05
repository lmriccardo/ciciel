#pragma once

#include <queue>
#include <stdexcept>
#include <iostream>
#include <ostream>
#include <sstream>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "queue_interface.hpp"

namespace ccl::ds::queue
{
    /**
     * Thread-safe blocking Queue FIFO implementation.
     * - push() waits if the queue is full (if bounded).
     * - pop() waits if the queue is empty.
     * - tryPush()/tryPop() are non-blocking.
     */
    template <typename T>
    class ConcurrentQueue : public QueueInterface<T>
    {
    protected:
        mutable std::mutex    m_mutex;
        std::condition_variable m_notEmpty;
        std::condition_variable m_notFull;

        std::queue<T>         m_queue;

        std::atomic<size_t>   m_size{0};
        std::atomic<size_t>   m_capacity{0};
        std::atomic<bool>     m_unlimited{true};

    public:
        ConcurrentQueue() = default;
        explicit ConcurrentQueue(size_t max_capacity);
        ConcurrentQueue(const ConcurrentQueue &other);
        ConcurrentQueue(ConcurrentQueue&& other) noexcept;

        explicit ConcurrentQueue(const std::queue<T>& other);
        explicit ConcurrentQueue(std::queue<T>&& other);

        ConcurrentQueue& operator=(const ConcurrentQueue&) = delete;
        ConcurrentQueue& operator=(ConcurrentQueue&&) = delete;

        virtual ~ConcurrentQueue() = default;

        size_t size    () const override;
        bool   empty   () const override;
        bool   full    () const override;
        size_t capacity() const override;

        void capacity(size_t value);

        void push(const T&)    override;
        void push(T&&)         override;
        bool tryPush(const T&) override;
        bool tryPush(T&&)      override;

        T    pop()             override;
        void pop(T&)           override;
        bool tryPop(T&)        override;
        bool peek(T&)    const override;
    };

    template <typename T>
    inline ConcurrentQueue<T>::ConcurrentQueue(size_t max_capacity)
        : m_capacity(max_capacity), m_unlimited(false)
    {
    }

    template <typename T>
    inline ConcurrentQueue<T>::ConcurrentQueue(const ConcurrentQueue &other)
    {
        std::scoped_lock lock(other.m_mutex);
        m_queue     = other.m_queue;
        m_size      = other.m_size.load();
        m_capacity  = other.m_capacity.load();
        m_unlimited = other.m_unlimited.load();
    }

    template <typename T>
    inline ConcurrentQueue<T>::ConcurrentQueue(ConcurrentQueue&& other) noexcept
    {
        std::scoped_lock lock(other.m_mutex);
        m_queue     = std::move(other.m_queue);
        m_size      = other.m_size.load();
        m_capacity  = other.m_capacity.load();
        m_unlimited = other.m_unlimited.load();
    }

    template <typename T>
    inline ConcurrentQueue<T>::ConcurrentQueue(const std::queue<T> &other)
        : m_queue(other), m_unlimited(true)
    {
        m_size     = other.size();
        m_capacity = other.size();
    }

    template <typename T>
    inline ConcurrentQueue<T>::ConcurrentQueue(std::queue<T> &&other)
        : m_queue(std::move(other)), m_unlimited(true)
    {
        m_size     = m_queue.size();
        m_capacity = m_queue.size();
    }

    template <typename T>
    inline size_t ConcurrentQueue<T>::size() const 
    { 
        return m_size.load(std::memory_order_relaxed);
    }

    template <typename T>
    inline bool ConcurrentQueue<T>::empty() const 
    { 
        return m_size.load(std::memory_order_relaxed) == 0;
    }

    template <typename T>
    inline bool ConcurrentQueue<T>::full() const 
    { 
        return !m_unlimited.load(std::memory_order_relaxed) 
               && m_size.load(std::memory_order_relaxed) >= m_capacity.load(std::memory_order_relaxed);
    }
    
    template <typename T>
    inline size_t ConcurrentQueue<T>::capacity() const 
    { 
        return m_capacity.load(std::memory_order_relaxed);
    }

    template <typename T>
    inline void ConcurrentQueue<T>::capacity(size_t value)
    {
        size_t old = m_capacity.load();
        while (value > old && !m_capacity.compare_exchange_weak(old, value)) {}
        m_unlimited.store(false);
    }

    template <typename T>
    inline void ConcurrentQueue<T>::push(const T &value)
    {
        push(T(value));
    }

    template <typename T>
    inline void ConcurrentQueue<T>::push(T &&value)
    {
        std::unique_lock lock(m_mutex);
        m_notFull.wait(lock, [this] {
            return m_unlimited.load() || m_size.load() < m_capacity.load();
        });

        m_queue.push(std::move(value));
        m_size.fetch_add(1, std::memory_order_relaxed);

        lock.unlock();
        m_notEmpty.notify_one();
    }

    template <typename T>
    inline bool ConcurrentQueue<T>::tryPush(T &&value)
    {
        std::unique_lock lock(m_mutex);
        if (!m_unlimited.load() && m_size.load() >= m_capacity.load()) return false;

        m_queue.push(std::move(value));
        m_size.fetch_add(1, std::memory_order_relaxed);

        lock.unlock();
        m_notEmpty.notify_one();
        return true;
    }

    template <typename T>
    inline bool ConcurrentQueue<T>::tryPush(const T &value)
    {
        return tryPush(T(value));
    }

    template <typename T>
    inline T ConcurrentQueue<T>::pop()
    {
        T element;
        pop(element);
        return element;
    }

    template <typename T>
    inline void ConcurrentQueue<T>::pop(T &dest)
    {
        std::unique_lock lock(m_mutex);
        m_notEmpty.wait(lock, [this] {
            return !m_queue.empty();
        });

        dest = std::move(m_queue.front());
        m_queue.pop();
        m_size.fetch_sub(1, std::memory_order_relaxed);

        lock.unlock();
        m_notFull.notify_one();
    }

    template <typename T>
    inline bool ConcurrentQueue<T>::tryPop(T &dest)
    {
        std::unique_lock lock(m_mutex);
        if (m_queue.empty()) return false;

        dest = std::move(m_queue.front());
        m_queue.pop();
        m_size.fetch_sub(1, std::memory_order_relaxed);

        lock.unlock();
        m_notFull.notify_one();
        return true;
    }

    template <typename T>
    inline bool ConcurrentQueue<T>::peek(T &dest) const
    {
        std::scoped_lock lock(m_mutex);
        if (m_queue.empty()) return false;
        dest = m_queue.front();
        return true;
    }
}
