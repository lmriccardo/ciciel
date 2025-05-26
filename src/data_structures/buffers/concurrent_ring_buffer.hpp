#pragma once

#include <shared_mutex>
#include <mutex>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <data_structures/base/indexable_interface.hpp>
#include <algorithm>
#include "ring_buffer.hpp"

namespace ccl::ds::buffers
{
    /**
     * This a concurrent (thread-safe) version of the RingBuffer class.
     * There are two main differences: first, this container is not 
     * iterable, which is expected given concurrency problems, second
     * simple pop functions are blocking, waiting on a conditional variable.
     * 
     * @tparam T The type of values inside the container
     */
    template <typename T>
    class ConcurrentRingBuffer : public IndexableInterface<T>
    {
    private:
        std::vector<T> m_buffer;
        size_t         m_capacity;
        
        std::atomic<size_t> m_size      = 0;
        size_t              m_front_idx = 0;
        size_t              m_back_idx  = 0;

        std::mutex              m_mutex; // Shared mutex for reading data
        std::condition_variable m_cv;    // Condition variable on wait for emptiness

        constexpr void boundCheck( size_t ) const;
        constexpr T    _popFront ();
        constexpr T    _popBack  ();

    public:
        ConcurrentRingBuffer( size_t );
        ConcurrentRingBuffer( const ConcurrentRingBuffer& ) = delete;
        ConcurrentRingBuffer( ConcurrentRingBuffer&& ) = delete;

        explicit ConcurrentRingBuffer( const std::vector<T>& );
        explicit ConcurrentRingBuffer( std::vector<T>&& );
        explicit ConcurrentRingBuffer( const RingBuffer<T>& );
        explicit ConcurrentRingBuffer( RingBuffer<T>&& );

        ConcurrentRingBuffer& operator=( const ConcurrentRingBuffer& ) = delete;
        ConcurrentRingBuffer& operator=( ConcurrentRingBuffer&& ) = delete;

        constexpr bool   empty   () const;
        constexpr size_t size    () const;
        constexpr size_t capacity() const;

        virtual constexpr const T& at(size_t) const override;
        virtual constexpr const T& operator[](size_t) const override;

        virtual constexpr T& at(size_t) override;
        virtual constexpr T& operator[](size_t) override;
        
        constexpr const T& front() const;
        constexpr const T& back () const;

        constexpr bool tryAt   (T&, size_t) const;
        constexpr bool tryFront(T&)         const;
        constexpr bool tryBack (T&)         const;

        template <typename U> constexpr void put(U&&);
        
        // This call version is blocking
        constexpr T popFront();

        // This version is blocking
        constexpr T popBack();

        constexpr bool tryPopFront(T&);
        constexpr bool tryPopBack (T&);
    };

    template <typename T>
    inline constexpr void ConcurrentRingBuffer<T>::boundCheck(size_t pos) const
    {
        if ( pos >= size() )
        {
            std::stringstream oss;

            oss << "Unable to access element at given position "
                << pos << " since the current size is "
                << size() << std::endl;

            throw std::out_of_range( oss.str() );
        }
    }

    template <typename T>
    inline constexpr T ConcurrentRingBuffer<T>::_popFront()
    {
        T value = std::move( m_buffer[m_front_idx] );
        m_front_idx = (m_front_idx + 1) % m_capacity;
        --m_size;
        return value;
    }

    template <typename T>
    inline constexpr T ConcurrentRingBuffer<T>::_popBack()
    {
        T value = std::move( m_buffer[m_back_idx] );
        m_back_idx = (m_back_idx + m_capacity - 1) % m_capacity;
        --m_size;
        return value;
    }

    template <typename T>
    inline ConcurrentRingBuffer<T>::ConcurrentRingBuffer(size_t capacity)
        : m_buffer( capacity ), m_capacity( capacity )
    {
    }

    template <typename T>
    inline ConcurrentRingBuffer<T>::ConcurrentRingBuffer(const std::vector<T> &data)
        : m_buffer( data ), m_capacity( data.size() ), m_size( m_capacity )
    {
    }

    template <typename T>
    inline ConcurrentRingBuffer<T>::ConcurrentRingBuffer(std::vector<T> && data)
        : m_buffer( std::move( data ) ), m_capacity( data.size() ), m_size( m_capacity )
    {
    }

    template <typename T>
    inline ConcurrentRingBuffer<T>::ConcurrentRingBuffer(const RingBuffer<T> &buffer)
    {
        m_capacity = buffer.capacity();
        m_size = buffer.size();
        m_buffer.reserve( m_capacity );

        std::copy( buffer.begin(), buffer.end(), std::back_inserter(m_buffer) );

        m_front_idx = m_size % m_capacity;
        m_back_idx  = m_size == 0 ? m_size : (m_size + m_capacity - 1) % m_capacity;
    }

    template <typename T>
    inline ConcurrentRingBuffer<T>::ConcurrentRingBuffer(RingBuffer<T> &&buffer)
    {
        RingBuffer<T> tmp = std::move( buffer );

        m_capacity = tmp.capacity();
        m_size = tmp.size();
        m_buffer.reserve( m_capacity );

        for ( const auto& elem: tmp )
        {
            m_buffer.push_back( std::move(elem) );
        }

        m_front_idx = m_size % m_capacity;
        m_back_idx  = m_size == 0 ? m_size : (m_size + m_capacity - 1) % m_capacity;
    }

    template <typename T>
    inline constexpr bool ConcurrentRingBuffer<T>::empty() const
    {
        return m_size.load() == 0;
    }

    template <typename T>
    inline constexpr size_t ConcurrentRingBuffer<T>::size() const
    {
        return m_size.load();
    }

    template <typename T>
    inline constexpr size_t ConcurrentRingBuffer<T>::capacity() const
    {
        return m_capacity;
    }

    template <typename T>
    inline constexpr const T& ConcurrentRingBuffer<T>::at(size_t pos) const
    {
        boundCheck( pos );

        std::unique_lock lock(m_mutex);
        return m_buffer[(m_front_idx + pos) % m_capacity];
    }

    template <typename T>
    inline constexpr const T &ConcurrentRingBuffer<T>::operator[](size_t pos) const { return at(pos); }

    template <typename T>
    inline constexpr T& ConcurrentRingBuffer<T>::at(size_t pos)
    {
        boundCheck( pos );

        std::unique_lock lock(m_mutex);
        return m_buffer[(m_front_idx + pos) % m_capacity];
    }

    template <typename T>
    inline constexpr T &ConcurrentRingBuffer<T>::operator[](size_t pos) { return at(pos); }

    template <typename T>
    inline constexpr const T &ConcurrentRingBuffer<T>::front() const
    {
        return at(0);
    }

    template <typename T>
    inline constexpr const T &ConcurrentRingBuffer<T>::back() const
    {
        return at(size() - 1);
    }

    template <typename T>
    inline constexpr bool ConcurrentRingBuffer<T>::tryAt(T &dst, size_t pos) const
    {
        try {
            dst = at(pos);
            return true;
        } catch(const std::exception& e) {
            return false;
        }
    }

    template <typename T>
    inline constexpr bool ConcurrentRingBuffer<T>::tryFront(T &dst) const
    {
        return tryAt(dst, 0);
    }

    template <typename T>
    inline constexpr bool ConcurrentRingBuffer<T>::tryBack(T &dst) const
    {
        return tryAt(dst, size() - 1);
    }

    template <typename T>
    template <typename U>
    inline constexpr void ConcurrentRingBuffer<T>::put(U &&value)
    {
        {
            std::unique_lock lock(m_mutex);

            if ( m_size == m_capacity ) m_front_idx = (m_front_idx + 1) % m_capacity;

            m_buffer[m_back_idx] = std::forward<U>(value);
            m_back_idx = (m_back_idx + 1) % m_capacity;
            
            if ( m_size < m_capacity ) m_size++;
        }

        m_cv.notify_one();
    }

    template <typename T>
    inline constexpr T ConcurrentRingBuffer<T>::popFront()
    {
        std::unique_lock lock(m_mutex);
        m_cv.wait( lock, [this](){ return !empty(); } );
        return _popFront();
    }

    template <typename T>
    inline constexpr T ConcurrentRingBuffer<T>::popBack()
    {
        std::unique_lock lock(m_mutex);
        m_cv.wait( lock, [this](){ return !empty(); } );
        return _popBack();
    }

    template <typename T>
    inline constexpr bool ConcurrentRingBuffer<T>::tryPopFront(T &dst)
    {
        if ( empty() ) return false;
        
        std::unique_lock lock(m_mutex);
        dst = _popFront();
        return true;
    }

    template <typename T>
    inline constexpr bool ConcurrentRingBuffer<T>::tryPopBack(T &dst)
    {
        if ( empty() ) return false;
        
        std::unique_lock lock(m_mutex);
        dst = _popBack();
        return true;
    }
}