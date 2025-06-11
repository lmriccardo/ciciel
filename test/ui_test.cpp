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

    // Create and draw your Label
    Label l( "Label1", U"Ciao 👋", 3, 1 );
    l.setContentStyle( DefaultStyle() );
    l.setPadding( { 3, 2, 3, 3 } );
    
    // Add the label to the screen main panel
    screen.addWidget( l );

    std::this_thread::sleep_for( std::chrono::seconds(1) );

    l.setContent( "Io mi chiamo Riccardo.", 
        Style().Foreground( Colors::Fuchsia )
               .Background( Colors::Olive )
               .Bold( true )
               .Italic( true ) );
    
    Terminal::getInstance().setCursorPosition( 12, 0 );

    std::cerr << "Absolute ID: " << l.getAbsoluteId() << std::endl;

    return 0;
}