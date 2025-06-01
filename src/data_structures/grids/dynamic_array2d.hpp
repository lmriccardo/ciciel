#pragma once

#include <vector>
#include <array>
#include <type_traits>
#include <data_structures/base/grid_container.hpp>

namespace ccl::ds::grids
{
    template <typename T, typename U>
    struct is_std_array_of : std::false_type {};

    template <typename T, std::size_t N>
    struct is_std_array_of<T, std::array<T, N>> : std::true_type {};

    template <typename T, typename U>
    using is_container = std::enable_if_t<std::is_same_v<std::decay_t<U>, std::vector<T>> 
        || is_std_array_of<T, std::decay_t<U>>::value>;

    // TODO: Custom type traits for checking the template Container parameter must be created.

    /**
     * This class represents a simple Array2D class with dynamic space allocation
     * for storing grid data, therefore it uses by default std::vector as underline 
     * container. However, the container can be user-defined, like a class that
     * stores grid data in a different way. The most important thing is that the
     * chosen container has the following methods: .resize(), .begin(), .end() 
     * (i.e., it is an iterable container), .at(), .size(), therefore a number of
     * methods coming from the std::vector container.
     */
    template <typename T, typename Container = std::vector<T>>
    class DynamicArray2D : public base::AbstractGridContainer<T, Container>
    {
    private:
        using Base = base::AbstractGridContainer<T, Container>;
        using Base::m_grid;
        using Base::m_policy;

        size_t m_rows, m_cols; // Total number of rows and columns
        
    public:
        DynamicArray2D( size_t, size_t, Ordering2DPolicy p = Ordering2DPolicy::ROW_MAJOR );
        DynamicArray2D( size_t, Ordering2DPolicy p = Ordering2DPolicy::ROW_MAJOR );
        DynamicArray2D( const DynamicArray2D& );
        DynamicArray2D( DynamicArray2D&& );

        template <typename U, typename = is_container<T,U>> 
        explicit DynamicArray2D( U&&, size_t, size_t );

        template <typename U, typename = is_container<T,U>> 
        explicit DynamicArray2D( U&&, size_t );

        DynamicArray2D& operator=( const DynamicArray2D& );
        DynamicArray2D& operator=( DynamicArray2D&& );

        virtual ~DynamicArray2D() = default;

        using Base::set;
        using Base::at;

        constexpr size_t size( ) const override;
        constexpr size_t size( size_t ) const override;

        void transpose();
        
        void extend( Selector2D, size_t );
        void extend( size_t, size_t );
        void extend( size_t );
    };

    template <typename T, typename Container>
    inline DynamicArray2D<T, Container>::DynamicArray2D(size_t rows, size_t cols, Ordering2DPolicy p)
        : Base( p ), m_rows( rows ), m_cols( cols )
    {
        m_grid.resize( m_rows * m_cols );
    }

    template <typename T, typename Container>
    inline DynamicArray2D<T, Container>::DynamicArray2D(size_t size, Ordering2DPolicy p)
        : DynamicArray2D( size, size, p )
    {
    }

    template <typename T, typename Container>
    inline DynamicArray2D<T, Container>::DynamicArray2D(const DynamicArray2D &other)
    {
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        m_grid.resize( m_rows * m_cols );
        m_policy = other.m_policy;

        std::copy( 
            other.m_grid.begin(), other.m_grid.end(), 
            m_grid.begin() );
    }

    template <typename T, typename Container>
    inline DynamicArray2D<T, Container>::DynamicArray2D(DynamicArray2D &&other)
    {
        m_grid = std::move( other.m_grid );
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        m_policy = other.m_policy;

        other.m_cols = 0;
        other.m_rows = 0;
    }

    template <typename T, typename Container>
    template <typename U, typename>
    inline DynamicArray2D<T, Container>::DynamicArray2D(U &&input, size_t rows, size_t cols)
        : Base(), m_rows( rows ), m_cols( cols )
    {
        if ( input.size() != size() ) throw std::invalid_argument( "Size mismatch" );
        
        if constexpr ( std::is_rvalue_reference_v<decltype(input)> )
        {
            std::move(
                std::make_move_iterator(input.begin()),
                std::make_move_iterator(input.end()),
                std::back_inserter(m_grid)
            );

        } else {
            std::copy( input.begin(), input.end(), std::back_inserter( m_grid ) );
        }
    }

    template <typename T, typename Container>
    template <typename U, typename>
    inline DynamicArray2D<T, Container>::DynamicArray2D(U &&input, size_t size)
        : DynamicArray2D<T, Container>( std::forward<U>(input), size, size )
    {
    }

    template <typename T, typename Container>
    inline DynamicArray2D<T, Container> &DynamicArray2D<T, Container>::operator=(const DynamicArray2D &other)
    {
        if ( this != &other )
        {
            m_policy = other.m_policy;
            m_rows = other.m_rows;
            m_cols = other.m_cols;

            m_grid.resize( size() );
            std::copy( other.m_grid.begin(), other.m_grid.end(), m_grid.begin() );
        }

        return *this;
    }

    template <typename T, typename Container>
    inline DynamicArray2D<T, Container> &DynamicArray2D<T, Container>::operator=(DynamicArray2D &&other)
    {
        m_grid = std::move( other.m_grid );
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        m_policy = other.m_policy;

        other.m_cols = 0;
        other.m_rows = 0;

        return *this;
    }

    template <typename T, typename Container>
    inline constexpr size_t DynamicArray2D<T, Container>::size() const
    {
        return m_cols * m_rows;
    }

    template <typename T, typename Container>
    inline constexpr size_t DynamicArray2D<T,Container>::size(size_t dim) const
    {
        if ( dim != (size_t)Selector2D::ROWS && dim != (size_t)Selector2D::COLUMNS )
            throw std::invalid_argument( "Array2D has only 2 dimensions" );

        return dim == static_cast<size_t>(Selector2D::ROWS) ? m_rows : m_cols;
    }

    /**
     * This transpose operation performs a logic transposition. It means that the
     * underline container is left untouched, but the ordering logic changes from
     * ROW-MAJOR to COLUMN-MAJOR and vice versa. User-side, operations like size,
     * set and .at or operator[] will appear to be transposed, as well as iterating
     * through out the entire container.
     */
    template <typename T, typename Container>
    inline void DynamicArray2D<T,Container>::transpose()
    {
        this->Base::transpose();
        std::swap( m_rows, m_cols );
    }

    /**
     * Extend the given dimension with the input padding size. Notice, that this
     * is an expensive operations given heap allocation and vector resizing.
     */
    template <typename T, typename Container>
    inline void DynamicArray2D<T,Container>::extend(Selector2D dim, size_t pad_size)
    {
        // If the selected dimension is the row dimension, then it is
        // sufficient to update the total number of rows, and then
        // resize the data container. In this way a number of zeros
        // will be added at at the end of the container
        if ( dim == Selector2D::ROWS )
        {
            m_rows = m_rows + pad_size;
            m_grid.resize( size() );
            return;
        }

        // In this case instead we need to iterate for each cell in the
        // last column and insert the given number of default values
        for ( size_t r_idx = 0; r_idx < m_rows; ++r_idx )
        {
            size_t insert_idx = m_cols * ( r_idx + 1 ) + r_idx * pad_size;
            m_grid.insert( m_grid.begin() + insert_idx, pad_size, T{} );
        }

        m_cols = m_cols + pad_size;
    }

    /**
     * Extends the row dimension with the first padding size and
     * the column dimension with the second padding size. Notice, 
     * that this is an expensive operations given heap allocation 
     * and vector resizing.
     */
    template <typename T, typename Container>
    inline void DynamicArray2D<T,Container>::extend(size_t r_pad_size, size_t c_pad_size)
    {
        extend( Selector2D::ROWS, r_pad_size );
        extend( Selector2D::COLUMNS, c_pad_size );
    }

    /**
     * Extend both dimensions of the given padding size. Notice, that this
     * is an expensive operations given heap allocation and vector resizing.
     */
    template <typename T, typename Container>
    inline void DynamicArray2D<T,Container>::extend(size_t pad_size)
    {
        extend( pad_size, pad_size );
    }
}