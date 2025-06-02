#pragma once

#include <stdexcept>
#include <sstream>
#include "custom_traits.hpp"
#include "iterable.hpp"

namespace ccl::ds::base
{
    template <typename T, typename C, typename = is_container<T, C>>
    class AbstractVectorContainer : public IterableContainer<T>
    {
    protected:
        C m_container;

        void checkOutOfBound( size_t ) const;
    
    public:
        AbstractVectorContainer() = default;
        virtual ~AbstractVectorContainer() = default;

        constexpr size_t size() const override;

        constexpr T&       at(size_t)               override;              
        constexpr const T& at(size_t)         const override;
        constexpr const T& operator[](size_t) const override;
        constexpr T&       operator[](size_t)       override;

        void set( const T&, size_t );

        // TODO: Add overloading for operator+, operator-, operator== and so on
    };

    template <typename T, typename C, typename U>
    inline void AbstractVectorContainer<T, C, U>::checkOutOfBound(size_t pos) const
    {
        if ( pos >= size() )
        {
            std::stringstream ss;
            ss << ".at() failed with out of range error. Trying to access "
               << "element at position " << pos << " but size is "
               << size() << " !!!"
               << std::endl;

            throw std::out_of_range( ss.str() );
        }
    }

    template <typename T, typename C, typename U>
    inline constexpr size_t AbstractVectorContainer<T, C, U>::size() const
    {
        return m_container.size();
    }

    template <typename T, typename C, typename U>
    inline constexpr T& AbstractVectorContainer<T,C,U>::at(size_t pos)
    {
        checkOutOfBound( pos );
        return m_container[pos];
    }

    template <typename T, typename C, typename U>
    inline constexpr const T& AbstractVectorContainer<T,C,U>::at(size_t pos) const
    {
        checkOutOfBound( pos );
        return m_container[pos];
    }

    template <typename T, typename C, typename U>
    inline constexpr T& AbstractVectorContainer<T,C,U>::operator[](size_t pos)
    {
        return at( pos );
    }

    template <typename T, typename C, typename U>
    inline constexpr const T& AbstractVectorContainer<T,C,U>::operator[](size_t pos) const
    {
        return at( pos );
    }

    template <typename T, typename C, typename U>
    inline void AbstractVectorContainer<T,C,U>::set(const T& val, size_t pos)
    {
        checkOutOfBound( pos );
        m_container[pos] = val;
    }
}