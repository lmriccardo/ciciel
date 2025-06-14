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
    screen.setVerticalAlignment( VerticalAlignment::Top );

    // Create and draw your Label
    Label l( "Label1", U"Ciao 👋", 0, 0 );
    l.setContentStyle( DefaultStyle() );
    l.setPadding( { 3, 2, 3, 3 } );
    l.setMargin( 1, Direction::Left );
    l.setGrowFactor( 3 );
    l.getContentStyle().Alignment( TextAlignment::Center );

    Style sl2 = Style().Foreground( Colors::Red )
                       .Bold( true )
                       .Italic( true );

    Label l2( "Label2", "Sono Riccardo", 0, 0 );
    l2.setContentStyle( sl2 );
    l2.setPadding( { 1, 1, 1, 1 } );
    l2.setMargin( { 0, 1, 1, 0 } );
    l2.setGrowFactor( 1 );
    l2.getContentStyle().Alignment( TextAlignment::Rigth );

    HBoxPanel p1( "SubPanel1", 0, 0, 0, 0 );
    p1.setGrowFactor( 1 );

    Label l3( "Label3", "Hey!!!", 0, 0 );
    l3.setMargin( { 0, 1, 1, 0 } );
    l3.setGrowFactor( 1 );

    p1.addChild( l3 );
    
    // Add the label to the screen main panel
    screen.addWidget( l );
    screen.addWidget( l2 );
    screen.addWidget( p1 );

    // Draw the UI
    screen.draw();

    return 0;
}