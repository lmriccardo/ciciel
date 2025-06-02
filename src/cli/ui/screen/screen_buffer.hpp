#pragma once

#include <iostream>
#include <unordered_map>
#include <data_structures/grids/dynamic_array2d.hpp>
#include "terminal.hpp"

namespace ccl::cli::ui
{
    struct CellChar
    {
        char32_t m_char; // The character inside the cell
    };

    class ScreenBuffer : public ccl::ds::grids::DynamicArray2D<char32_t>
    {
    private:
        std::unordered_map<size_t, int> m_lastUpdate; // Differences wrt the last flush
        int m_updateCounter; // The current update iterator

    public:
        ScreenBuffer( size_t, size_t );
        virtual ~ScreenBuffer() = default;

        size_t getWidth() const;
        size_t getHeight() const;

        /**
         * Resize the screen buffer adding to each dimension (row and columns)
         * the input size. That is, the first one is for rows ( height ) and
         * the second one for columns ( width ).
         */
        void resize( size_t, size_t );

        using ccl::ds::grids::DynamicArray2D<char32_t>::set;

        /**
         * Insert a string into the buffer at given position. If the string
         * does not fit into space, then a warning is raised and only a portion
         * of the string will be copied into the buffer.
         */
        void set( const std::string&, size_t, size_t );
        void set( const char32_t&, size_t, size_t );

        /**
         * Flush the content to the terminal. Only differences are written.
         */
        void flush( Terminal& t_out ) const;
    };
}