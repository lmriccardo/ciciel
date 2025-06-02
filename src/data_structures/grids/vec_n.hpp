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
        virtual ~VectorN() = default;
    };

    template <typename T>
    class DynamicVectorN : public ds::base::AbstractVectorContainer<T, std::vector<T>>
    {
    private:
        using Base = ds::base::AbstractVectorContainer<T, std::vector<T>>;
        using Base::m_container;
    
    public:
        DynamicVectorN( size_t );
        virtual ~DynamicVectorN() = default;
    };

    template <typename T>
    inline DynamicVectorN<T>::DynamicVectorN(size_t size_in)
        : Base()
    {
        m_container.resize(size_in);
    }
}