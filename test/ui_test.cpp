#include <cli/ui.hpp>
#include <thread>

using namespace ccl::cli::ui;

class A {};

int main()
{
    // Initialize the Screen. This also initializes the Terminal singleton.
    Screen screen( Layout::HorizontalLayout );

    // --- Step 1: Explicitly clear the screen ---
    // This ensures a clean canvas for your label.
    screen.clear();

    // Create your local ScreenBuffer.
    ScreenBuffer sb( 40, 11 );

    // Create and draw your Label to the local ScreenBuffer.
    Label l( "Label1", U"Ciao 👋", 3, 1 ); // Positioned at row 5, column 5
    l.setContentStyle( DefaultStyle() );
    l.setPadding( { 3, 2, 3, 3 } );
    l.draw( sb );
    sb.flush( Terminal::getInstance() );

    std::this_thread::sleep_for( std::chrono::seconds(1) );

    l.setContent( "Io mi chiamo Riccardo.", 
        Style().Foreground( Colors::Fuchsia )
               .Background( Colors::Olive )
               .Bold( true )
               .Italic( true ) );
    
    l.draw( sb );
    sb.flush( Terminal::getInstance() );
    Terminal::getInstance().setCursorPosition( 12, 0 );

    return 0;
}