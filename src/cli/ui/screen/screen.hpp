#pragma once

#include <memory>
#include "terminal.hpp"
#include "screen_buffer.hpp"

namespace ccl::cli::ui
{
    class Screen
    {
    private:
        Terminal& m_terminal;  // Controls low-level data for the terminal
        std::unique_ptr<ScreenBuffer> m_buffer; // Controls the buffer of chars

        static int _id; // Screen unique identifier for preventing additional instances

    public:
        Screen();
        ~Screen() = default;
    };
}