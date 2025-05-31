#pragma once

#include <cstddef>
#include <math.h>
#include <iostream>
#include <array>
#include <vector>

#include <data_structures/base/grid_container.hpp>

namespace ccl::ds::grids
{
    /**
     * This class defines a simple (R x C) Grid container of generic type T. It stores the
     * data as a 1D vector so that all elements are contiguously stored in memory.
     * Notice that both the total number of rows and columns must be known at
     * compile-time. It uses static std::array to store data in memory
     *
     * @tparam T The type of elements in the grid
     * @tparam _NR The total number of rows of the grid
     * @tparam _NC The total number of columns of the grid
     */
    template <typename T, size_t _NR, size_t _NC>
    class Array2D : public base::AbstractGridContainer<T, std::array<T, _NR * _NC>>
    {
    private:
        using Base = base::AbstractGridContainer<T, std::array<T, _NR * _NC>>;
        using Base::m_grid;
        using Base::m_policy;

    public:
        Array2D(Ordering2DPolicy p = Ordering2DPolicy::ROW_MAJOR);
        Array2D( const Array2D& );
        Array2D( Array2D&& );

        explicit Array2D( const std::vector<T>& vec );
        explicit Array2D( const std::array<T, _NR * _NC>& array ); 

        virtual ~Array2D() = default;

        constexpr Array2D& operator=( const Array2D& other );
        constexpr Array2D& operator=( Array2D&& other );

        using Base::set;
        using Base::at;
        using Base::transpose;

        constexpr size_t size( ) const override;
        constexpr size_t size( size_t ) const override;

        Array2D<T, _NC, _NR> deepTranspose();
    };

    template <typename T, size_t _NR, size_t _NC>
    inline Array2D<T, _NR, _NC>::Array2D(Ordering2DPolicy p) : Base(p)
    {
    }

    template <typename T, size_t _NR, size_t _NC>
    inline Array2D<T, _NR, _NC>::Array2D(const Array2D &other)
    {
        m_policy = other.m_policy;
        std::copy(other.m_grid.begin(), other.m_grid.end(), m_grid.begin());
    }

    template <typename T, size_t _NR, size_t _NC>
    inline Array2D<T, _NR, _NC>::Array2D(Array2D &&other)
    {
        m_grid = std::move( other.m_grid );
        m_policy = other.m_policy;
    }

    template <typename T, size_t _NR, size_t _NC>
    inline Array2D<T, _NR, _NC>::Array2D(const std::vector<T> &vec)
    {
        if ( vec.size() != size() )
        {
            throw std::runtime_error( 
                "Input vector does not match given num of rows and columns" );
        }

        size_t vec_idx = 0;

        for ( size_t r_idx = 0; r_idx < _NR; ++r_idx )
        {
            for ( size_t c_idx = 0; c_idx < _NC; ++c_idx )
            {
                set( vec.at(vec_idx), r_idx, c_idx );
            }
        }
    }

    template <typename T, size_t _NR, size_t _NC>
    inline Array2D<T, _NR, _NC>::Array2D(const std::array<T, _NR * _NC> &array)
    {
        if ( array.size() != size() )
        {
            throw std::runtime_error( 
                "Input array does not match given num of rows and columns" );
        }

        std::copy( array.begin(), array.end(), m_grid.begin() );
    }

    template <typename T, size_t _NR, size_t _NC>
    constexpr inline Array2D<T, _NR, _NC> &Array2D<T, _NR, _NC>::operator=(const Array2D &other)
    {
        if ( this != &other )
        {
            m_policy = other.m_policy;
            std::copy(other.m_grid.begin(), other.m_grid.end(), m_grid.begin());
        }

        return *this;
    }

    template <typename T, size_t _NR, size_t _NC>
    inline constexpr Array2D<T, _NR, _NC> &Array2D<T, _NR, _NC>::operator=(Array2D &&other)
    {
        if ( this != &other )
        {
            m_grid = std::move( other.m_grid );
            m_policy = other.m_policy;
        }

        return *this;
    }

    template <typename T, size_t _NR, size_t _NC>
    inline constexpr size_t Array2D<T, _NR, _NC>::size() const
    {
        return _NR * _NC;
    }

    template <typename T, size_t _NR, size_t _NC>
    inline constexpr size_t Array2D<T, _NR, _NC>::size(size_t dim) const
    {
        if ( dim != (size_t)Selector2D::ROWS && dim != (size_t)Selector2D::COLUMNS )
            throw std::invalid_argument( "Array2D has only 2 dimensions" );

        if ( m_policy == Ordering2DPolicy::ROW_MAJOR ) {
            return dim == static_cast<size_t>(Selector2D::ROWS) ? _NR : _NC;
        } else {
            return dim == static_cast<size_t>(Selector2D::ROWS) ? _NC : _NR;
        }
    }

    /**
     * Tranpose the current data into a new Array2D object and returns it.
     * Notice that the resulting ordering logic in the new instance will be
     * ROW-MAJOR, while the current one is left unchanged.
     */
    template <typename T, size_t _NR, size_t _NC>
    inline Array2D<T, _NC, _NR> Array2D<T, _NR, _NC>::deepTranspose()
    {
        Array2D<T, _NC, _NR> result;
        this->transposeRaw( &result );
        return result;
    }
};