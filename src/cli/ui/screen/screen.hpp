#pragma once

#include <memory>
#include <cstring>
#include "terminal.hpp"
#include "screen_buffer.hpp"

namespace ccl::cli::ui
{
    struct CursorInfo
    {
        const CursorStyle m_default_style = CursorStyle::SteadyBlock;

        CursorStyle m_style  = m_default_style;
        bool        m_hidden = false;
        size_t      m_xpos   = 0;
        size_t      m_ypos   = 0;
    };

    class Screen
    {
    private:
        std::unique_ptr<ScreenBuffer> m_buffer;   // Controls the buffer of chars
        Terminal&                     m_terminal; // Controls low-level data for the terminal
        CursorInfo                    m_cinfo;    // Informations about the current cursor
        struct winsize                m_winsize;  // Window size information

        static int _id; // Screen unique identifier for preventing additional instances

    public:
        Screen();
        ~Screen();

        /**
         * Sets the cursor style for the application. The input parameter
         * select the style and ranges from 1 to 6 ( out of bound values 
         * are ignored and the default one is used, i.e., 2 ). For more
         * information regarding cursor styles check the style.hpp file.
         * 
         * @param style The cursor style identifier
         */
        void setCursorStyle( CursorStyle style );

        /**
         * Move the cursor to the given position, if valid, i.e., inside
         * range bounds given by the current window size.
         * 
         * @param x_pos Position on the X axis
         * @param y_pos Position on the Y axis
         */
        void setCursorPosition( int, int );
        
        /**
         * Move the cursor of the given offset.
         * 
         * @param x_off X axis Offset
         * @param y_off Y axis Offset
         */
        void moveCursor( int, int );

        /**
         * Clear the screen.
         */
        void clear();
    };
}