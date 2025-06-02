#include <cli/ui/screen/screen_buffer.hpp>
#include <iostream>

using namespace ccl::cli::ui;

int main()
{
    ScreenBuffer buffer( 4, 4 );

    buffer.set("ciao come stai ??", 0, 0);

    std::cout << "\x1b[38;2;255;120;120mRED TrueColor\x1b[0m\n";
    std::cout << "\x1b[38;5;196mRED 256-color\x1b[0m\n";
    std::cout << "\x1b[31mRED ANSI 16-color\x1b[0m\n";

    return 0;
}