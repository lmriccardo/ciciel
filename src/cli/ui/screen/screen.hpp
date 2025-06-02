#pragma once

#include "terminal.hpp"
#include "screen_buffer.hpp"

namespace ccl::cli::ui
{
    class Screen
    {
    private:
        Terminal& m_terminal = Terminal::getInstance(); // Controls low-level data for the terminal
        ScreenBuffer m_buffer;   // Controls the buffer of chars

    public:
        
    };
}