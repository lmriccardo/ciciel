#pragma once

#include <type_traits>
#include <data_structures/buffers/ring_buffer.hpp>
#include "queue_interface.hpp"

namespace ccl::ds::queue
{
    enum class LossPolicy
    {
        OVERWRITE_OLDEST,
        ERROR,
        BLOCK
    };

    /**
     * CircularQueue implementation. It uses a RingBuffer as storage option
     * and a LossPolicy value for pushing operations. In particular, when
     * pushing over the capacity: OVERWRITE_OLDEST performs the classical
     * RingBuffer operation by overwriting at the read index; ERROR raises
     * an error, or returns false; and BLOCK cannot be used with non-concurrent
     * queues.
     * 
     * It inherits from QueueInterface and so implements all queue-releated methods.
     * 
     * @tparam T The type of all values in the container.
     */
    template <typename T>
    class CircularQueue : public QueueInterface<T>
    {
    private:
        ds::buffers::RingBuffer<T> m_buffer;
        LossPolicy                 m_policy;
    
    public:
        CircularQueue() = default;
        CircularQueue( size_t capacity, LossPolicy policy = LossPolicy::ERROR );
        CircularQueue( const CircularQueue& ) = default;
        CircularQueue( CircularQueue&& ) = default;

        template <typename U> explicit CircularQueue( U&& );

        virtual ~CircularQueue() = default;

        size_t size    () const override;
        bool   empty   () const override;
        size_t capacity() const override;
        bool   full    () const override;

        constexpr LossPolicy getLossPolicy() const;
        constexpr void setLossPolicy( LossPolicy );
        
        void push(const T&)    override;
        void push(T&&)         override;
        bool tryPush(const T&) override;
        bool tryPush(T&&)      override;

        T    pop   ()         override;
        void pop   (T&)       override;
        bool tryPop(T&)       override;
        bool peek  (T&) const override;

        virtual const ds::buffers::RingBuffer<T>& data() const;
        virtual ds::buffers::RingBuffer<T>&       data();
    };

    template <typename T>
    inline CircularQueue<T>::CircularQueue(size_t capacity, LossPolicy policy)
        : m_buffer( capacity ), m_policy( policy )
    {
        static_assert( m_policy != LossPolicy::BLOCK, 
            "BLOCK Loss policy is available only for concurrect queue!" );
    }

    template <typename T>
    template <typename U>
    inline CircularQueue<T>::CircularQueue(U &&buffer)
        : m_buffer( std::forward<U>(buffer) )
    {
        static_assert( std::is_same_v<std::remove_cvref_t<U>, ds::buffers::RingBuffer<T>>,
            "Constructor requires a RingBuffer<T> argument.");
    }

    template <typename T>
    inline size_t CircularQueue<T>::size() const
    {
        return m_buffer.size();
    }

    template <typename T>
    inline bool CircularQueue<T>::empty() const
    {
        return m_buffer.empty();
    }

    template <typename T>
    inline size_t CircularQueue<T>::capacity() const
    {
        return m_buffer.capacity();
    }

    template <typename T>
    inline bool CircularQueue<T>::full() const
    {
        return size() == capacity();
    }

    template <typename T>
    inline constexpr LossPolicy CircularQueue<T>::getLossPolicy() const
    {
        return m_policy;
    }

    template <typename T>
    inline constexpr void CircularQueue<T>::setLossPolicy(LossPolicy policy)
    {
        if ( m_policy != LossPolicy::BLOCK )
        {
            throw std::invalid_argument( "BLOCK Loss policy is available only for concurrect queue!" );
        }

        m_policy = policy;
    }

    template <typename T>
    inline void CircularQueue<T>::push(T&& value)
    {
        if ( full() && m_policy == LossPolicy::ERROR )
        {
            throw std::runtime_error( "CircularQueue is full! No more element can be pushed" );
        }

        m_buffer.put(std::move(value));
    }

    template <typename T>
    inline void CircularQueue<T>::push(const T& value)
    {
        if ( full() && m_policy == LossPolicy::ERROR )
        {
            throw std::runtime_error( "CircularQueue is full! No more element can be pushed" );
        }

        m_buffer.put(value);
    }

    template <typename T>
    inline bool CircularQueue<T>::tryPush(T&& value)
    {
        if ( full() && m_policy == LossPolicy::ERROR ) return false;
        m_buffer.put(std::move(value));
        return true;
    }

    template <typename T>
    inline bool CircularQueue<T>::tryPush(const T& value)
    {
        if ( full() && m_policy == LossPolicy::ERROR ) return false;
        m_buffer.put(value);
        return true;
    }

    template <typename T>
    inline T CircularQueue<T>::pop()
    {
        try {
            m_buffer.popFront();
        } catch ( std::exception& e ) {
            throw std::out_of_range( "CircularQueue is empty!" );
        }
    }

    template <typename T>
    inline void CircularQueue<T>::pop(T& dest)
    {
        dest = pop();
    }

    template <typename T>
    inline bool CircularQueue<T>::tryPop(T& dest)
    {
        if ( empty() ) return false;
        pop(dest);
        return true;
    }

    template<typename T>
    inline bool CircularQueue<T>::peek(T& dest) const
    {
        return m_buffer.tryFront(dest);
    }

    template <typename T>
    inline const ds::buffers::RingBuffer<T> &CircularQueue<T>::data() const
    {
        return m_buffer;
    }

    template <typename T>
    inline ds::buffers::RingBuffer<T> &CircularQueue<T>::data()
    {
        return m_buffer;
    }
};