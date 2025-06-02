#pragma once

#include "indexable_interface.hpp"
#include "iterators.hpp"

namespace ccl::ds::base
{
    /**
     * A generic base class for all grid containers that would like also to implement
     * iterable-like behavior. It already constructs the iterator by using the 
     * iterator_base class, and for this reason it also must inherit from
     * Indexable2DInterface (a subclass of IndexableInterface).
     * 
     * @tparam T The type of the elements that belong to the container
     */
    template <typename T>
    class Iterable2DContainer : public Indexable2DInterface<T>
    {
    public:
        using iterator = ds::base::iterator_base<T, T*, T&>;
        using const_iterator = ds::base::iterator_base<T, const T*, const T&>;

        virtual ~Iterable2DContainer() = default;
        virtual constexpr size_t size() const = 0;
        virtual constexpr size_t size( size_t ) const = 0;

        iterator       begin ();
        iterator       end   ();
        const_iterator begin () const;
        const_iterator end   () const;
        const_iterator cbegin() const;
        const_iterator cend  () const;
    };

    template <typename T>
    inline typename Iterable2DContainer<T>::iterator Iterable2DContainer<T>::begin() 
    { 
        return iterator(this, 0); 
    }

    template <typename T>
    inline typename Iterable2DContainer<T>::iterator Iterable2DContainer<T>::end() 
    { 
        return iterator(this, size()); 
    }

    template <typename T>
    inline typename Iterable2DContainer<T>::const_iterator Iterable2DContainer<T>::begin() const 
    { 
        return const_iterator(this, 0); 
    }

    template <typename T>
    inline typename Iterable2DContainer<T>::const_iterator Iterable2DContainer<T>::end() const 
    { 
        return const_iterator(this, size()); 
    }

    template <typename T>
    inline typename Iterable2DContainer<T>::const_iterator Iterable2DContainer<T>::cbegin() const 
    { 
        return begin(); 
    }

    template <typename T>
    inline typename Iterable2DContainer<T>::const_iterator Iterable2DContainer<T>::cend() const 
    { 
        return end(); 
    }
}