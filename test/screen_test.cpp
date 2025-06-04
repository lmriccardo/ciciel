#include <cli/ui/screen/screen.hpp>
#include <iostream>

using namespace ccl::cli::ui;

int main()
{
    // Screen screen;
    // screen.setCursorStyle(CursorStyle::BlinkUnderline);
    // screen.clear();

    // std::cout << "Press any key to continue ...";

    // screen.setCursorPosition( 10, 10 );
    // std::cin.get();

    struct winsize ws;
    Terminal::getWindowSize( &ws );
    ScreenBuffer buffer( ws.ws_row, ws.ws_col );

    Style s = Style().Background( Colors::Lime )
                     .Foreground( Colors::Maroon )
                     .Italic( true )
                     .Bold( true );

    buffer.set("ciao come 👋", 10, 10, s, false);
    buffer.flush( Terminal::getInstance() );

    Terminal::getInstance().setCursorPosition(11, 0);
    Terminal::getInstance().reset();

    return 0;
}