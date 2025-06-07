#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <memory>

#include "iterable2d.hpp"
#include "enum.hpp"

namespace ccl::ds::base
{
    /**
     * This is the base class for all Grid containers. The actual underline container
     * is left to the user, but it is set as a dynamic C++ vector as default value.
     * It extends Iterable2DContainer therefore provides the implementation of a number
     * of utilities for accessing elements and iterating through the entire container.
     * 
     * Data is stored in a 1D flattened vector and accessed both for reading and writing
     * depending on the provided Ordering Policy: ROW-MAJOR or COLUMN-MAJOR.
     * 
     * This class cannot be instantiated since it does not provides the implementation
     * of .size method which is left to the derived classes. Therefore, it is abstract.
     * 
     * @tparam T The type of data stored into the container
     * @tparam ContainerT The type of indexable container used to store data
     */
    template <typename T, typename ContainerT = std::vector<T>>
    class AbstractGridContainer : public Iterable2DContainer<T>
    {
    protected:
        ContainerT        m_grid;
        Ordering2DPolicy m_policy;

        constexpr void boundCheck( size_t, size_t ) const;
        constexpr void boundCheck( size_t ) const;
        constexpr void getRowCol ( size_t&, size_t&, size_t ) const;

        template <typename U, typename ContainerU>
        void transposeRaw( AbstractGridContainer<U, ContainerU>* dst );

    public:
        AbstractGridContainer( Ordering2DPolicy policy = Ordering2DPolicy::ROW_MAJOR );
        virtual ~AbstractGridContainer() = default;

        using Iterable2DContainer<T>::size;

        constexpr const T &at( size_t, size_t ) const override;
        constexpr T&       at( size_t, size_t ) override;

        constexpr T&       at(size_t)               override;              
        constexpr const T& at(size_t)         const override;
        constexpr const T& operator[](size_t) const override;
        constexpr T&       operator[](size_t)       override;

        constexpr void set(const T &, size_t , size_t );
        constexpr void set(const T &, size_t );

        void transpose();
        size_t flatten( size_t, size_t ) const;
    };

    template <typename T, typename ContainerT>
    inline constexpr void AbstractGridContainer<T, ContainerT>::boundCheck(size_t row, size_t col) const
    {
        // Perform Boundary checks
        if (m_policy == Ordering2DPolicy::COLUMN_MAJOR) std::swap(row, col);

        if (row >= size( (size_t)Selector2D::ROWS ) || col >= size( (size_t)Selector2D::COLUMNS ))
        {
            throw std::out_of_range("[OutOfBounds] Invalid access to grid.");
        }
    }

    template <typename T, typename ContainerT>
    inline constexpr void AbstractGridContainer<T, ContainerT>::boundCheck(size_t pos) const
    {
        if ( pos >= size() )
        {
            throw std::out_of_range("[OutOfBounds] Invalid access to grid.");
        }
    }

    template <typename T, typename ContainerT>
    inline constexpr void AbstractGridContainer<T, ContainerT>::getRowCol(size_t &row, size_t &col, size_t pos) const
    {
        size_t _size = size((size_t)Selector2D::COLUMNS);
        size_t logical_row = pos / _size;
        size_t logical_col = pos % _size;

        row = logical_row;
        col = logical_col;
    }

    template <typename T, typename ContainerT>
    template <typename U, typename ContainerU>
    inline void AbstractGridContainer<T, ContainerT>::transposeRaw(AbstractGridContainer<U, ContainerU> *dst)
    {
        bool tranposed = false;

        // If the array is not yet transposed then tranpose it, in order to make
        // it easier to get all values.
        if ( m_policy == Ordering2DPolicy::ROW_MAJOR )
        {
            transpose();
            tranposed = true;
        }
        
        auto it = this->begin();
        for ( ; it != this->end(); ++it )
        {
            dst->set( *it, it.pos() );
        }

        // Transpose it back if it was transposed in the first lines
        if ( m_policy != Ordering2DPolicy::ROW_MAJOR && tranposed ) transpose();
    }

    template <typename T, typename ContainerT>
    inline AbstractGridContainer<T, ContainerT>::AbstractGridContainer(Ordering2DPolicy policy)
        : m_policy( policy )
    {
    }

    template <typename T, typename ContainerT>
    inline constexpr const T& AbstractGridContainer<T, ContainerT>::at( size_t row, size_t col ) const
    {
        if (m_policy == Ordering2DPolicy::COLUMN_MAJOR) std::swap(row, col);
        boundCheck(row, col);
        return m_grid[row * size((size_t)Selector2D::COLUMNS) + col];
    }

    template <typename T, typename ContainerT>
    inline constexpr T &AbstractGridContainer<T, ContainerT>::at(size_t row, size_t col)
    {
        size_t _size = size((size_t)Selector2D::COLUMNS);

        if (m_policy == Ordering2DPolicy::COLUMN_MAJOR)
        {
            std::swap(row, col);
            _size = size((size_t)Selector2D::ROWS);
        }

        boundCheck(row, col);
        return m_grid[row * _size + col];
    }

    template <typename T, typename ContainerT>
    inline constexpr T &AbstractGridContainer<T, ContainerT>::at(size_t pos)
    {
        size_t row, col; getRowCol( row, col, pos );
        return at(row, col);
    }

    template <typename T, typename ContainerT>
    inline constexpr const T &AbstractGridContainer<T, ContainerT>::at(size_t pos) const
    {
        size_t row, col; getRowCol( row, col, pos );
        return at(row, col);
    }

    template <typename T, typename ContainerT>
    inline constexpr T &AbstractGridContainer<T, ContainerT>::operator[](size_t pos)
    {
        return at(pos);
    }

    template <typename T, typename ContainerT>
    inline constexpr const T &AbstractGridContainer<T, ContainerT>::operator[](size_t pos) const
    {
        return at(pos);
    }

    template <typename T, typename ContainerT>
    inline constexpr void AbstractGridContainer<T, ContainerT>::set(const T& val, size_t row, size_t col)
    {
        if (m_policy == Ordering2DPolicy::COLUMN_MAJOR) std::swap(row, col);
        boundCheck(row, col);
        m_grid[row * size((size_t)Selector2D::COLUMNS) + col] = val;
    }

    template <typename T, typename ContainerT>
    inline constexpr void AbstractGridContainer<T, ContainerT>::set(const T &val, size_t pos)
    {
        size_t row, col; getRowCol( row, col, pos );
        set(val, row, col);
    }

    /**
     * This transpose operation performs a logic transposition. It means that the
     * underline container is left untouched, but the ordering logic changes from
     * ROW-MAJOR to COLUMN-MAJOR and vice versa. User-side, operations like size,
     * set and .at or operator[] will appear to be transposed, as well as iterating
     * through out the entire container.
     */
    template <typename T, typename ContainerT>
    inline void AbstractGridContainer<T, ContainerT>::transpose()
    {
        m_policy = m_policy == Ordering2DPolicy::ROW_MAJOR ? 
              Ordering2DPolicy::COLUMN_MAJOR
            : Ordering2DPolicy::ROW_MAJOR;
    }

    /**
     * Given row and column indexes it returns the flatted index
     * according to transposition logic or not.
     */
    template <typename T, typename ContainerT>
    inline size_t AbstractGridContainer<T, ContainerT>::flatten(size_t r_idx, size_t c_idx) const
    {
        if (m_policy == Ordering2DPolicy::COLUMN_MAJOR) std::swap( r_idx, c_idx );
        return r_idx * size( static_cast<size_t>( Selector2D::COLUMNS ) ) + c_idx;
    }
}