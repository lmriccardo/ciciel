#pragma once

#include <iostream>
#include <unordered_map>
#include <data_structures/grids/dynamic_array2d.hpp>
#include <cli/ui/style/style.hpp>
#include "terminal.hpp"
#include "u32string.hpp"

namespace ccl::cli::ui
{
    struct CellChar
    {
        char32_t m_char;   // The character inside the cell
        Style    m_style;  // The style of the character
        bool     m_redraw; // Forse redraw the current cell
    };

    class ScreenBuffer : public ccl::ds::grids::DynamicArray2D<struct CellChar>
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

        using ccl::ds::grids::DynamicArray2D<struct CellChar>::set;

        /**
         * Insert a string into the buffer at given position. If the string
         * does not fit into space, then a warning is raised and only a portion
         * of the string will be copied into the buffer.
         */
        size_t set( const std::string&, size_t, size_t, const Style&, bool );
        size_t set( char32_t, size_t, size_t, const Style&, bool );
        size_t set( char32_t, size_t, const Style&, bool );
        size_t set( const std::u32string&, size_t, size_t, const Style&, bool );

        /**
         * Flush the content to the terminal. Only differences are written.
         */
        void flush( Terminal& t_out );
    };
}