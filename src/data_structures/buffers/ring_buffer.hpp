#pragma once

#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <data_structures/base/iterable.hpp>

namespace ccl::ds::buffers
{
    /**
     * Implementation of a Circular (Ring) Buffer. It uses the std::vector container
     * as data storage and a read and write index to locate items in the buffer
     * through modular arithmetic. Given the fact that it is circular, elements
     * written over the actual capacity of the underline container overwrite oldest
     * values (those pointed by the read index).
     * 
     * @tparam T Base type for all elements in the buffer
     */
    template <typename T>
    class RingBuffer : public ds::base::IterableContainer<T>
    {
    private:
        std::vector<T> m_buffer;
        size_t         m_capacity;
        
        size_t m_size      = 0;
        size_t m_read_idx  = 0;
        size_t m_write_idx = 0;

        constexpr void boundCheck( size_t ) const;

    public:
        RingBuffer() = default;
        RingBuffer( size_t );
        RingBuffer( const std::vector<T>& );
        RingBuffer( std::vector<T>&& );
        RingBuffer( const RingBuffer& ) = default;
        RingBuffer( RingBuffer&& ) = default;

        virtual ~RingBuffer() = default;

        RingBuffer& operator=( const RingBuffer&) = default;
        RingBuffer& operator=( RingBuffer&& ) = default;

        constexpr bool   empty   () const;
        constexpr size_t size    () const override;
        constexpr size_t capacity() const;

        constexpr const T& at(size_t) const;
        constexpr const T& operator[](size_t) const override;

        constexpr T& at(size_t);
        constexpr T& operator[](size_t) override;
        
        constexpr const T& front() const;
        constexpr const T& back () const;

        constexpr bool tryAt   (T&, size_t) const;
        constexpr bool tryFront(T&)         const;
        constexpr bool tryBack (T&)         const;

        template <typename U> constexpr void put(U&&);
        constexpr void clear();
    };

    template <typename T>
    inline constexpr void RingBuffer<T>::boundCheck(size_t pos) const
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
    inline RingBuffer<T>::RingBuffer(size_t capacity)
        : m_buffer(capacity), m_capacity(capacity)
    {
    }

    template <typename T>
    inline RingBuffer<T>::RingBuffer(const std::vector<T> &vector)
        : m_buffer(vector), m_capacity(vector.size()), 
          m_size(vector.size())
    {
    }

    template <typename T>
    inline RingBuffer<T>::RingBuffer(std::vector<T> &&vector)
        : m_buffer(std::move(vector)), m_capacity(m_buffer.size()), 
          m_size(m_buffer.size())
    {
    }

    template <typename T>
    inline constexpr size_t RingBuffer<T>::size() const { return m_size; }

    template <typename T>
    inline constexpr size_t RingBuffer<T>::capacity() const { return m_capacity; }

    template <typename T>
    inline constexpr bool RingBuffer<T>::empty() const { return m_size == 0; }

    template <typename T>
    inline constexpr const T &RingBuffer<T>::at(size_t pos) const
    {
        boundCheck( pos ); // Perform bounds checking
        return m_buffer[(m_read_idx + pos) % m_capacity];
    }

    template <typename T>
    inline constexpr const T &RingBuffer<T>::operator[](size_t pos) const { return at(pos); }

    template <typename T>
    inline constexpr T &RingBuffer<T>::at(size_t pos)
    {
        boundCheck( pos ); // Perform bounds checking
        return m_buffer[(m_read_idx + pos) % m_capacity];
    }

    template <typename T>
    inline constexpr T &RingBuffer<T>::operator[](size_t pos) { return at(pos); }

    template <typename T>
    template <typename U>
    inline constexpr void RingBuffer<T>::put(U &&value)
    {
        if ( m_size == m_capacity ) m_read_idx = (m_read_idx + 1) % m_capacity;

        m_buffer[m_write_idx] = std::forward<U>(value);
        m_write_idx = (m_write_idx + 1) % m_capacity;
        
        if ( m_size < m_capacity ) m_size++;
    }

    template <typename T>
    inline constexpr const T &RingBuffer<T>::front() const
    {
        return at(0);
    }

    template <typename T>
    inline constexpr const T &RingBuffer<T>::back() const
    {
        return at(size() - 1);
    }

    template <typename T>
    inline constexpr bool RingBuffer<T>::tryAt(T &dst, size_t pos) const
    {
        try {
            dst = at(pos);
            return true;
        } catch(const std::exception& e) {
            return false;
        }
    }

    template <typename T>
    inline constexpr bool RingBuffer<T>::tryFront(T &dst) const
    {
        return tryAt(dst, 0);
    }

    template <typename T>
    inline constexpr bool RingBuffer<T>::tryBack(T &dst) const
    {
        return tryAt(dst, size() - 1);
    }

    template <typename T>
    inline constexpr void RingBuffer<T>::clear()
    {
        m_size = 0;
        m_write_idx = 0;
        m_read_idx = 0;

        m_buffer.clear();
        m_buffer.resize(m_capacity);
    }
}