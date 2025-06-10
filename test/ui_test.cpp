#include <cli/ui.hpp>
#include <thread>

using namespace ccl::cli::ui;

class A {};

int main()
{
    // Initialize the Screen. This also initializes the Terminal singleton.
    Screen screen( Layout::HorizontalLayout );
    std::cerr << "DEBUG: Screen instance created." << std::endl;

    // --- Step 1: Explicitly clear the screen ---
    // This ensures a clean canvas for your label.
    screen.clear();
    std::cerr << "DEBUG: Screen cleared." << std::endl;

    std::this_thread::sleep_for( std::chrono::milliseconds(500) );
    std::cerr << "DEBUG: Initial sleep finished." << std::endl;

    // Create your local ScreenBuffer.
    ScreenBuffer sb( 20, 10 );
    std::cerr << "DEBUG: Local ScreenBuffer created (20x10)." << std::endl;

    // Create and draw your Label to the local ScreenBuffer.
    Label l( "Label1", U"Ciao 👋", 5, 5 ); // Positioned at row 5, column 5
    l.setContentStyle( DefaultStyle() );
    l.draw( sb );
    std::cerr << "DEBUG: Label drawn to local ScreenBuffer." << std::endl;

    // Flush the ScreenBuffer content to the actual terminal.
    std::cout << "Start Flushing ... " << std::endl;
    std::cerr << "DEBUG: Calling sb.flush()..." << std::endl;
    sb.flush( Terminal::getInstance() );
    std::cerr << "DEBUG: sb.flush() completed." << std::endl;
    std::cout << "End Flushing ... " << std::endl;

    // --- Step 2: Add a pause to allow the user to see the output ---
    std::cerr << "DEBUG: Waiting for 2 seconds before exit to show output..." << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds(500) );

    std::cerr << "DEBUG: Program finished successfully." << std::endl;
}