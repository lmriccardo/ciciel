#pragma once

#include <data_structures/queue/queue_interface.hpp>
#include <data_structures/buffers/concurrent_ring_buffer.hpp>
#include <data_structures/base/enum.hpp>

namespace ccl::ds::queue
{
    // TODO: To be extended with more features
    /**
     * @class ConcurrentCircQueue
     * 
     * @brief Concurrent Circular Queue class. It is just a Circular Queue
     * implemented thread-safetiness features. It is based on the Concurrent
     * Ring Buffer Implementation.
     * 
     * @tparam T The type of data belonging to the queue
     */
    template <typename T>
    class ConcurrentCircQueue : public QueueInterface<T>
    {
    private:
        ds::buffers::ConcurrentRingBuffer<T> m_queue;
        LossPolicy m_policy;

        mutable std::mutex m_mutex;
        std::condition_variable m_full_cv; // Conditional variable for full check

    public:
        ConcurrentCircQueue() = default;
        ConcurrentCircQueue(size_t capacity, LossPolicy policy=LossPolicy::BLOCK);

        size_t size    () const override;
        bool   empty   () const override;
        size_t capacity() const override;
        bool   full    () const override;

        void push(const T& data) override;
        void push(T&& data) override;
        bool tryPush(const T& data) override;
        bool tryPush(T&& data) override;

        T    pop() override;
        void pop(T& dst) override;
        bool tryPop(T& dst) override;
        bool peek  (T& dst) const override;
    };

    template <typename T>
    inline ConcurrentCircQueue<T>::ConcurrentCircQueue(size_t capacity, LossPolicy policy)
    : m_queue( capacity ), m_policy( policy )
    {}

    template <typename T>
    inline size_t ConcurrentCircQueue<T>::size() const
    {
        return m_queue.size();
    }

    template <typename T>
    inline bool ConcurrentCircQueue<T>::empty() const
    {
        return m_queue.empty();
    }

    template <typename T>
    inline size_t ConcurrentCircQueue<T>::capacity() const
    {
        return m_queue.capacity();
    }

    template <typename T>
    inline bool ConcurrentCircQueue<T>::full() const
    {
        return size() == capacity();
    }

    template <typename T>
    inline void ConcurrentCircQueue<T>::push(const T &data)
    {
        if ( full() && m_policy == LossPolicy::ERROR )
        {
            throw std::runtime_error( "Queue is full! No more element can be pushed" );
        }

        if ( full() && m_policy == LossPolicy::BLOCK )
        {
            std::unique_lock _l( m_mutex );
            m_full_cv.wait( _l, [this](){ return !full(); } );
        }

        m_queue.put( data );
    }

    template <typename T>
    inline void ConcurrentCircQueue<T>::push(T &&data)
    {
        if ( full() && m_policy == LossPolicy::ERROR )
        {
            throw std::runtime_error( "Queue is full! No more element can be pushed" );
        }

        if ( full() && m_policy == LossPolicy::BLOCK )
        {
            std::unique_lock _l( m_mutex );
            m_full_cv.wait( _l, [this](){ return !full(); } );
        }

        m_queue.put( std::move(data) );
    }

    template <typename T>
    inline bool ConcurrentCircQueue<T>::tryPush(const T &data)
    {
        if ( full()) return false;
        push( data );
        return true;
    }

    template <typename T>
    inline bool ConcurrentCircQueue<T>::tryPush(T &&data)
    {
        if ( full()) return false;
        push( std::move(data) );
        return true;
    }

    template <typename T>
    inline T ConcurrentCircQueue<T>::pop()
    {
        T elem = m_queue.popFront();
        m_full_cv.notify_one();
        return elem;
    }

    template <typename T>
    inline void ConcurrentCircQueue<T>::pop(T &dst)
    {
        dst = m_queue.popFront();
        m_full_cv.notify_one();
    }

    template <typename T>
    inline bool ConcurrentCircQueue<T>::tryPop(T &dst)
    {
        if ( empty() ) return false;
        pop(dst);
        return true;
    }

    template <typename T>
    inline bool ConcurrentCircQueue<T>::peek(T &dst) const
    {
        return m_queue.tryFront( dst );
    }
}