#pragma once

#include <ostream>
#include "vec_n.hpp"

namespace ccl::ds::grids
{
    template <typename T>
    struct Vec2 : public VectorN<T, 2>
    {
        T& m_x;
        T& m_y;

        Vec2();
        Vec2( T x, T y );
        virtual ~Vec2() = default;

        using VectorN<T,2>::set;
        using VectorN<T,2>::at;
    };

    template <typename T>
    inline Vec2<T>::Vec2() : m_x( at(0) ), m_y( at(1) )
    {
    }

    template <typename T>
    inline Vec2<T>::Vec2(T x, T y) : Vec2()
    {
        set( x, 0 );
        set( y, 1 );
    }

    // TODO: add traits that T must be printable
    template <typename T>
    std::ostream &operator<<(std::ostream &os, const Vec2<T> &vec)
    {
        os << "[ " << vec.m_x << ", " << vec.m_y << " ]";
        return os;
    }
}