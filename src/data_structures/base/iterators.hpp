#pragma once

#include <cstddef>
#include "container_interface.hpp"

namespace ccl::ds::base
{
    template <typename T, typename Pointer, typename Reference>
    class iterator_base
    {
    private:
        ContainerInterface<T>* m_container;
        size_t m_pos;
    
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = Pointer;
        using reference         = Reference;

        iterator_base(ContainerInterface<T>* container, size_t pos);

        reference      operator* () const;
        pointer        operator->() const;
        iterator_base& operator++();
        iterator_base  operator++(int);

        bool operator==(const iterator_base& other) const;
        bool operator!=(const iterator_base& other) const;
    };

    template <typename T, typename Pointer, typename Reference>
    inline iterator_base<T, Pointer, Reference>::iterator_base(ContainerInterface<T> *container, size_t pos)
        : m_container(container), m_pos(pos)
    {
    }

    template <typename T, typename Pointer, typename Reference>
    inline iterator_base<T, Pointer, Reference>::reference iterator_base<T, Pointer, Reference>::operator*() const
    {
        return (*m_container)[m_pos];
    }

    template <typename T, typename Pointer, typename Reference>
    inline iterator_base<T, Pointer, Reference>::pointer iterator_base<T, Pointer, Reference>::operator->() const
    {
        return &(*m_container)[m_pos];
    }

    template <typename T, typename Pointer, typename Reference>
    inline iterator_base<T, Pointer, Reference>& iterator_base<T, Pointer, Reference>::operator++()
    {
        ++m_pos;
        return *this;
    }

    template <typename T, typename Pointer, typename Reference>
    inline iterator_base<T, Pointer, Reference> iterator_base<T, Pointer, Reference>::operator++(int)
    {
        iterator_base<T, Pointer, Reference> tmp = *this;
        ++(*this);
        return tmp;
    }

    template <typename T, typename Pointer, typename Reference>
    inline bool iterator_base<T, Pointer, Reference>::operator==(const iterator_base &other) const
    {
        return m_container == other.m_container && m_pos == other.m_pos;
    }

    template <typename T, typename Pointer, typename Reference>
    inline bool iterator_base<T, Pointer, Reference>::operator!=(const iterator_base &other) const
    {
        return !(*this == other);
    }
}