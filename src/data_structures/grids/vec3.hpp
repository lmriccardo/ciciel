#pragma once

#include <ostream>
#include "vec_n.hpp"

namespace ccl::ds::grids
{
    template <typename T>
    struct Vec3 : public VectorN<T, 3>
    {
        T& m_x;
        T& m_y;
        T& m_z;

        Vec3();
        Vec3( T x, T y, T z );
        Vec3(const Vec3&) = default;
        Vec3(Vec3&&) = default;

        virtual ~Vec3() = default;

        template <typename U> Vec3& operator=(const Vec3<U>&);
        Vec3& operator=(const Vec3&);

        using VectorN<T,3>::set;
        using VectorN<T,3>::at;
    };

    template <typename T>
    inline Vec3<T>::Vec3() : m_x( at(0) ), m_y( at(1) ), m_z( at(2) )
    {
    }

    template <typename T>
    inline Vec3<T>::Vec3(T x, T y, T z) : Vec3()
    {
        set( x, 0 );
        set( y, 1 );
        set( z, 2 );
    }

    // TODO: add traits that T must be printable
    template <typename T>
    std::ostream &operator<<(std::ostream &os, const Vec3<T> &vec)
    {
        os << "[ " << vec.m_x << ", " << vec.m_y << ", " << vec.m_z << " ]";
        return os;
    }

    template <typename T>
    template <typename U>
    inline Vec3<T> &Vec3<T>::operator=(const Vec3<U> &other)
    {
        this->template VectorN<T,3>::template operator=<U>(other);
        return *this;
    }

    template <typename T>
    inline Vec3<T>& Vec3<T>::operator=(const Vec3& other)
    {
        return this->operator=<T>(other);
    }
}