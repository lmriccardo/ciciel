#pragma once

#include "container_interface.hpp"
#include "iterators.hpp"

namespace ccl::ds::base
{
    template <typename T>
    class IterableContainer : public ContainerInterface<T>
    {
    public:
        typedef ds::base::iterator_base<T, T*, T&> iterator;
        typedef ds::base::iterator_base<T, const T*, const T&> const_iterator;

        virtual ~IterableContainer() = default;
        virtual size_t size() const = 0;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;
    };

    template <typename T>
    inline typename IterableContainer<T>::iterator IterableContainer<T>::begin() 
    { 
        return iterator(this, 0); 
    }

    template <typename T>
    inline typename IterableContainer<T>::iterator IterableContainer<T>::end() 
    { 
        return iterator(this, size()); 
    }

    template <typename T>
    inline typename IterableContainer<T>::const_iterator IterableContainer<T>::begin() const 
    { 
        return const_iterator(this, 0); 
    }

    template <typename T>
    inline typename IterableContainer<T>::const_iterator IterableContainer<T>::end() const 
    { 
        return const_iterator(this, size()); 
    }

    template <typename T>
    inline typename IterableContainer<T>::const_iterator IterableContainer<T>::cbegin() const 
    { 
        return begin(); 
    }

    template <typename T>
    inline typename IterableContainer<T>::const_iterator IterableContainer<T>::cend() const 
    { 
        return end(); 
    }
}