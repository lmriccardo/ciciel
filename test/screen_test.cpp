#include <cli/ui/screen/screen.hpp>
#include <iostream>
#include <utf8proc.h>

using namespace ccl::cli::ui;

int main()
{
    Screen screen;
    screen.clear();

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