#pragma once

#include <queue>
#include <stdexcept>
#include <iostream>
#include <ostream>
#include "queue_interface.hpp"

namespace ccl::ds
{
    /**
     * Classical Queue FIFO implementation with some additional
     * facilities with respect to the std::queue container.
     */
    template <typename T>
    class Queue : public QueueInterface<T>
    {
    protected:
        std::queue<T> m_queue;
        size_t        m_capacity;
        
        // This attribute identify if the queue has a limited
        // or unlimited capacity. Modifies the behavior or push.
        bool m_unlimited = true;

    public:
        Queue() = default;
        Queue( size_t max_capacity );
        Queue( const Queue &other ) = default;
        Queue( Queue&& other ) = default;
        Queue( const std::queue<T>& other );
        Queue( std::queue<T>&& other );

        Queue& operator=(const Queue&) = default;
        Queue& operator=(Queue&&) = default;

        virtual ~Queue() = default;

        size_t size    () const override;
        bool   empty   () const override;
        bool   full    () const override;
        size_t capacity() const override;

        /**
         * Sets the maximum capacity of the queue. It also changes the
         * value of m_unlimited attribute by setting it to false.
         * The actual capacity will be the maximum between the current
         * one and the input one.
         */
        void capacity(size_t value);

        void push(T&&)         override;
        void push(const T&)    override;
        bool tryPush(T&&)      override;
        bool tryPush(const T&) override;
        T    pop()             override;
        void pop(T&)           override;
        bool tryPop(T&)        override;
        bool peek(T&)    const override;
    };

    template <typename T>
    inline Queue<T>::Queue(size_t max_capacity)
        : m_capacity(max_capacity)
    {
    }

    template <typename T>
    inline Queue<T>::Queue(const std::queue<T> &other)
        : m_queue(other), m_unlimited(true)
    {
        m_capacity = other.size();
    }

    template <typename T>
    inline Queue<T>::Queue(std::queue<T> &&other)
        : m_queue(std::move(other)), m_unlimited(true)
    {
        m_capacity = other.size();
    }

    template <typename T>
    inline size_t Queue<T>::size() const { return m_queue.size(); }

    template <typename T>
    inline bool Queue<T>::empty() const { return size() == 0; }

    template <typename T>
    inline bool Queue<T>::full() const { return m_unlimited ? true : (size() == m_capacity); }
    
    template <typename T>
    inline size_t Queue<T>::capacity() const { return m_capacity; }

    template <typename T>
    inline void Queue<T>::capacity(size_t value)
    {
        m_capacity = (value > m_capacity) ? value : m_capacity;
        m_unlimited = false;
    }

    template <typename T>
    inline void Queue<T>::push(const T &value)
    {
        push(T(value));
    }

    template <typename T>
    inline void Queue<T>::push(T &&value)
    {
        if ( !m_unlimited && size() == m_capacity )
        {
            std::stringstream oss;

            oss << "Unable to put more elements into the queue. " 
                << "Maximum capacity of " << m_capacity << " "
                << "is equal to current size" 
                << std::endl;

            throw std::runtime_error( oss.str() );
        }

        m_queue.push(std::move(value));
        if ( !m_unlimited ) ++m_capacity;
    }

    template <typename T>
    inline bool Queue<T>::tryPush(T &&value)
    {
        if ( !m_unlimited && size() == m_capacity ) return false;
        m_queue.push(std::move(value));
        if ( !m_unlimited ) ++m_capacity;
        return true;
    }

    template <typename T>
    inline bool Queue<T>::tryPush(const T &value)
    {
        return tryPush(T(value));
    }

    template <typename T>
    inline T Queue<T>::pop()
    {
        T element;
        pop(element);
        return element;
    }

    template <typename T>
    inline void Queue<T>::pop(T &dest)
    {
        if ( empty() )
        {
            std::stringstream oss;
            oss << "Unable to pop since the queue is empty" << std::endl;
            throw std::runtime_error( oss.str() );
        }

        dest = m_queue.front();
        m_queue.pop();
    }

    template <typename T>
    inline bool Queue<T>::tryPop(T &dest)
    {
        if ( empty() ) return false;
        dest = m_queue.front();
        return true;
    }

    template <typename T>
    inline bool Queue<T>::peek(T &dest) const
    {
        if ( empty() ) return false;
        dest = m_queue.front();
        return true;
    }
}