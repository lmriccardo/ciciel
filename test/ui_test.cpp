#include <cli/ui.hpp>

using namespace ccl::cli::ui;

class A {};

int main()
{
    // Screen screen( Layout::HorizontalLayout );

    // ScreenBuffer sb( 20, 10 );
    // Label l( "Label1", U"Ciao 👋", 5, 5 );
    // l.setContentStyle( DefaultStyle() );
    // l.setWinsize( 10, 3 );
    // l.draw( sb );
    // sb.flush( Terminal::getInstance() );

    const char32_t c = U'┌';
    char c1[5] = {};
    utf32to8c( &c, c1 );
    std::cout << charwidth(&c) << std::endl;
    std::cout << c1 << std::endl;

    return 0;
}