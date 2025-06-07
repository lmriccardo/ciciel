#include <cli/ui/screen/screen.hpp>
#include <iostream>
#include <utf8proc.h>

using namespace ccl::cli::ui;

int main()
{
    try
    {
        Screen window;
        window.clear();
    }
    catch(const std::exception& e)
    {
        Terminal::getInstance().disableRawMode();
        Terminal::getInstance().reset();
        throw std::runtime_error( e.what() );
    }

    struct winsize ws;
    Terminal::getWindowSize( &ws );
    ScreenBuffer buffer( ws.ws_row, ws.ws_col );

    Style s = Style().Foreground( Colors::Red )
                     .Italic( true )
                     .Bold( true );

    buffer.set("ciao come 👋", 0, 0, s, false);
    buffer.flush( Terminal::getInstance() );

    Terminal::getInstance().setCursorPosition(1, 0);
    Terminal::getInstance().reset();

    return 0;
}