#include "screen.hpp"

using namespace ccl::cli::ui;

int Screen::_id = 0;

Screen::Screen() : m_terminal(Terminal::getInstance())
{
    if (_id > 0) 
    {
        throw std::runtime_error("No more Screen instances can be created");
    }

    auto [ r, c ] = Terminal::getWindowSize();
    m_buffer = std::make_unique<ScreenBuffer>((size_t)r, (size_t)c);
    _id++;
}
