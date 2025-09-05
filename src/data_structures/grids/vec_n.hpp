#pragma once

#include <array>
#include <data_structures/base/vec_container.hpp>

namespace ccl::ds::grids
{
    template <typename T, size_t N>
    class VectorN : public ds::base::AbstractVectorContainer<T, std::array<T,N>>
    {
    private:
        using Base = ds::base::AbstractVectorContainer<T, std::array<T,N>>;
        using Base::m_container;
        
    public:
        VectorN() = default;
        VectorN(const VectorN&);
        VectorN(VectorN&&);

        virtual ~VectorN() = default;

        template <typename U> VectorN& operator=(const VectorN<U,N>&);
        VectorN& operator=(const VectorN&);

        constexpr size_t size() const override;
    };

    template <typename T, size_t N>
    inline VectorN<T, N>::VectorN(const VectorN &other) : VectorN()
    {
        std::copy( 
            other.m_container.begin(), other.m_container.end(), 
            m_container.begin() );
    }

    template <typename T, size_t N>
    inline VectorN<T, N>::VectorN(VectorN &&other) : VectorN()
    {
        m_container = std::move( other.m_container );
    }

    template <typename T, size_t N>
    template <typename U>
    inline VectorN<T,N> &VectorN<T, N>::operator=(const VectorN<U,N> &other)
    {
        static_assert(std::is_convertible_v<U,T>, "Incompatible Types");
        
        for ( size_t idx = 0; idx < this->size(); idx++ )
        {
            this->set( other.at( idx ), idx );
        }

        return *this;
    }

    template <typename T, size_t N>
    inline VectorN<T,N>& VectorN<T,N>::operator=(const VectorN& other)
    {
        if (this != &other) {
            return this->operator=<T>(other);
        }

        return *this;
    }

    template <typename T, size_t N>
    inline constexpr size_t VectorN<T, N>::size() const
    {
        return N;
    }

    template <typename T>
    class DynamicVectorN : public ds::base::AbstractVectorContainer<T, std::vector<T>>
    {
    private:
        using Base = ds::base::AbstractVectorContainer<T, std::vector<T>>;
        using Base::m_container;
    
    public:
        DynamicVectorN( size_t );
        virtual ~DynamicVectorN() = default;

        template <typename U>
        DynamicVectorN& operator=(const DynamicVectorN<U>&);
        DynamicVectorN& operator=(const DynamicVectorN&);
    };

    template <typename T>
    inline DynamicVectorN<T>::DynamicVectorN(size_t size_in)
        : Base()
    {
        m_container.resize(size_in);
    }

    template <typename T>
    template <typename U>
    inline DynamicVectorN<T> &DynamicVectorN<T>::operator=(const DynamicVectorN<U> &other)
    {
        static_assert(std::is_convertible_v<U,T>, "Incompatible Types");

        if (this->size() != other.size())
        {
            m_container.resize( other.size() );
        }

        for ( size_t idx = 0; idx < this->size(); idx++ )
        {
            this->set( other.at( idx ), idx );
        }

        return *this;
    }

    template <typename T>
    inline DynamicVectorN<T> &DynamicVectorN<T>::operator=(const DynamicVectorN& other)
    {
        if ( this != &other )
        {
            return this->operator=<T>(other);
        }

        return *this;
    }
}