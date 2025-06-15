#pragma once

#include <iostream>
#include <unordered_map>
#include <data_structures/grids/dynamic_array2d.hpp>
#include <cli/ui/style/style.hpp>
#include <cli/ui/utils/string.hpp>
#include "terminal.hpp"

namespace ccl::cli::ui
{
    struct CellChar
    {
        char32_t m_char;   // The character inside the cell
        Style    m_style;  // The style of the character
        bool     m_redraw; // Force redraw the current cell
    };

    /**
     * A Screen Buffer is a dynamic two dimensional array which is used to holds essentially 
     * char32 elements, along with their style and redrawability. It represents a 1:1 map of 
     * the low level terminal buffer. It stores element as 4 bytes char in order to handle also
     * wide chars correctly ( UTF-8 encoded chars and extended ASCII ). In order to maintain 
     * the correct low-level mapping, for every char that takes up more than 1 terminal column, 
     * it put a '\000' element in the next position.
     * 
     * Internally, it also implements double buffering by flushing to the terminal only those
     * cells that have changed with respect to the previous flush operation. This reduce
     * flickering which is bad for UI CLI applications.
     */
    class ScreenBuffer : public ccl::ds::grids::DynamicArray2D<struct CellChar>
    {
    private:
        std::unordered_map<size_t, int> m_lastUpdate; // Differences wrt the last flush
        int m_updateCounter; // The current update iterator

    public:
        ScreenBuffer( size_t width, size_t height );
        virtual ~ScreenBuffer() = default;

        size_t getWidth() const;
        size_t getHeight() const;

        /**
         * Resize the screen buffer adding to each dimension (row and columns)
         * the input size. That is, the first one is for rows ( height ) and
         * the second one for columns ( width ).
         */
        void resize( size_t new_h, size_t new_w );

        using ccl::ds::grids::DynamicArray2D<struct CellChar>::set;

        /**
         * Insert a string into the buffer at given position. If the string
         * does not fit into space, then a warning is raised and only a portion
         * of the string will be copied into the buffer.
         */
        size_t set( const std::string& content, size_t y, size_t x, const Style& style, bool redraw );
        size_t set( char32_t content, size_t y, size_t x, const Style& style, bool redraw );
        size_t set( char32_t content, size_t pos, const Style& style, bool redraw );
        size_t set( const std::u32string& content, size_t y, size_t x, const Style& style, bool redraw );

        /**
         * Flush the content to the terminal. Only differences are written.
         */
        void flush( Terminal& t_out );
    };
}