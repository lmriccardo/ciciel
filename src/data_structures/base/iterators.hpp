#pragma once

#include <cstddef>
#include "indexable_interface.hpp"

namespace ccl::ds::base
{
    /**
     * Abstract base class for iterators. This class already defines some of the
     * methods used by iterators like equality check and pointer incrementation.
     * On the other hand, more specialized functions are left pure virtual like
     * operators * and ->. 
     * 
     * The constractor takes as input a pointer to the 'Container' type, which is
     * left as template parameter to maintain it general, and the starting position.
     * 
     * @tparam T The type of values in the container (in general returned by the iterator)
     * @tparam Pointer The value returned by the operator->
     * @tparam Reference The value returned by the operator*
     * @tparam U The type of the container
     * @tparam Derived The derived class used to implement increment operators
     */
    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    class abstract_iterator
    {
    protected:
        U*     m_iterable;
        size_t m_pos;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = Pointer;
        using reference         = Reference;

        abstract_iterator(U* iterable, size_t pos);

        virtual reference operator* () const = 0;
        virtual pointer   operator->() const = 0;
        virtual Derived&  operator++();
        virtual Derived   operator++(int);

        Derived operator+( difference_type n ) const;
        Derived operator-( difference_type n ) const;

        difference_type operator-( const abstract_iterator& other ) const;
        reference operator[]( difference_type n ) const;

        bool operator<( const abstract_iterator& other ) const;
        bool operator>( const abstract_iterator& other ) const;
        bool operator<=( const abstract_iterator& other ) const;
        bool operator>=( const abstract_iterator& other ) const;
        bool operator==(const abstract_iterator& other) const;
        bool operator!=(const abstract_iterator& other) const;

        size_t pos() const;
    };

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline abstract_iterator<T, Pointer, Reference, U, Derived>::abstract_iterator(U *iterable, size_t pos)
        : m_iterable(iterable), m_pos(pos)
    {
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline Derived& abstract_iterator<T, Pointer, Reference,U, Derived>::operator++()
    {
        ++m_pos;
        return static_cast<Derived&>(*this);
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline Derived abstract_iterator<T, Pointer, Reference,U, Derived>::operator++(int)
    {
        Derived tmp = static_cast<Derived&>(*this);
        ++(*this);
        return tmp;
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline Derived abstract_iterator<T, Pointer, Reference,U, Derived>::operator+( difference_type n ) const
    {
        return Derived( m_iterable, m_pos + n );
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline Derived abstract_iterator<T, Pointer, Reference,U, Derived>::operator-( difference_type n ) const
    {
        return Derived( m_iterable, m_pos - n );
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline abstract_iterator<T, Pointer, Reference, U, Derived>::difference_type 
        abstract_iterator<T, Pointer, Reference, U, Derived>::operator-(const abstract_iterator &other) const
    {
        return static_cast<difference_type>( m_pos ) - static_cast<difference_type>(other.m_pos);
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline abstract_iterator<T, Pointer, Reference, U, Derived>::reference
        abstract_iterator<T, Pointer, Reference, U, Derived>::operator[]( difference_type n ) const
    {
        return *(*this + n);
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline bool abstract_iterator<T, Pointer, Reference, U, Derived>::operator<(const abstract_iterator &other) const
    {
        return m_pos < other.m_pos;
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline bool abstract_iterator<T, Pointer, Reference, U, Derived>::operator>(const abstract_iterator &other) const
    {
        return m_pos > other.m_pos;
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline bool abstract_iterator<T, Pointer, Reference, U, Derived>::operator<=(const abstract_iterator &other) const
    {
        return m_pos <= other.m_pos;
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline bool abstract_iterator<T, Pointer, Reference, U, Derived>::operator>=(const abstract_iterator &other) const
    {
        return m_pos >= other.m_pos;
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline size_t abstract_iterator<T, Pointer, Reference, U, Derived>::pos() const
    {
        return m_pos;
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline bool abstract_iterator<T, Pointer, Reference, U, Derived>::operator==(const abstract_iterator &other) const
    {
        return m_iterable == other.m_iterable && m_pos == other.m_pos;
    }

    template <typename T, typename Pointer, typename Reference, typename U, typename Derived>
    inline bool abstract_iterator<T, Pointer, Reference, U, Derived>::operator!=(const abstract_iterator &other) const
    {
        return !(*this == other);
    }

    /**
     * Classical container-driven iterator. It inherit directly from the abstract_iterator
     * specifying Container type U and Derived type (itself). It implements the two pure
     * abstract methods operator* and operator->.
     * 
     * The base container type is an IndexableInterface which forces all subclasses
     * to implement the indexing on the derived container, i.e., methods operator[] and .at.
     * The constructor takes as input a pointer to an IndexableInterface derived object
     * and the starting position.
     */
    template <typename T, typename Pointer, typename Reference>
    class iterator_base : 
        public abstract_iterator<T, Pointer, Reference, IndexableInterface<T>,
            iterator_base<T, Pointer, Reference>>
    {
    private:
        using Base = abstract_iterator<T, Pointer, Reference, IndexableInterface<T>,iterator_base>;
        using typename Base::reference;
        using typename Base::pointer;

    public:
        iterator_base(IndexableInterface<T>* container, size_t pos);

        reference operator* () const override;
        pointer   operator->() const override;
    };

    template <typename T, typename Pointer, typename Reference>
    inline iterator_base<T, Pointer, Reference>::iterator_base(IndexableInterface<T> *container, size_t pos)
        : Base(container, pos)
    {
    }

    template <typename T, typename Pointer, typename Reference>
    inline iterator_base<T, Pointer, Reference>::reference iterator_base<T, Pointer, Reference>::operator*() const
    {
        return (*this->m_iterable)[this->m_pos];
    }

    template <typename T, typename Pointer, typename Reference>
    inline iterator_base<T, Pointer, Reference>::pointer iterator_base<T, Pointer, Reference>::operator->() const
    {
        return &(*this->m_iterable)[this->m_pos];
    }
}