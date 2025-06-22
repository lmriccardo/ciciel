#include <cli/ui.hpp>
#include <thread>

using namespace ccl::cli::ui;

class A {};

int main()
{
    UIApplication app( "Simple UI Application" );
    
    Screen& screen = app.getContentPanel();
    screen.enableMouse(); // Enable mouse tracking
    screen.setLayout( Layout::VerticalLayout );

    // --- Step 1: Explicitly clear the screen ---
    // This ensures a clean canvas for your label.
    screen.clear();
    screen.setVerticalAlignment( LayoutAlignment::Start );

    // Create and draw your Label
    Label l( "Label1", U"Ciao 👋" );
    l.setContentStyle( DefaultStyle() );
    l.setPadding( { 1, 1, 1, 1 } );
    l.setMargin( 1, Direction::Left );
    l.setMargin( 2, Direction::Rigth );
    l.setGrowFactor( 1 );
    l.getContentStyle().Alignment( TextAlignment::Center );
    l.setShrinkFactor( 1 );
    l.setBorderVisibility( true );

    Style sl2 = Style().Foreground( Colors::Red )
                       .Bold( true )
                       .Italic( true );

    List l1( "List1", 10 );
    l1.addElement( "Element 1" )
      .addElement( "Element 2 longer than 1" )
      .addElement( "Element 3 smaller" )
      .addElement( "Element 4" );

    l1.setPadding( { 1, 1, 1, 1 } );
    l1.setMargin( 1, Direction::Left );
    l1.setMargin( 1, Direction::Rigth );
    l1.setGrowFactor( 0 );
    l1.setTextAlignment( TextAlignment::Center );
    l1.setVisibility( true );

    HBoxPanel p1( "SubPanel1", 0, 0, 0, 0 );
    p1.setTitle( "New Panel" );
    p1.setGrowFactor( 1 );
    p1.setShrinkFactor( 1 );
    p1.setMargin( 1, Direction::Left );
    p1.setMargin( 1, Direction::Rigth );

    // Label l3( "Label3", "Hey!!!", 0, 0 );
    // l3.setMargin( { 0, 1, 1, 0 } );
    // l3.setGrowFactor( 1 );
    // l3.setShrinkFactor( 1 );

    // p1.addChild( l3 );

    Button b( "Button1", "Click" );
    b.setMargin( { 0, 1, 1, 0 } );
    p1.addChild( b );
    
    // Add the label to the screen main panel
    screen.addWidget( l );
    screen.addWidget( l1 );
    screen.addWidget( p1 );

    // Draw the UI
    // screen.draw();

    // std::this_thread::sleep_for( std::chrono::seconds(1) );

    // l1.setVisibility( true );

    // // Draw the UI
    // screen.draw();

    app.run();

    // for ( int i = 0; i < 20; ++i )
    // {
    //     std::this_thread::sleep_for( std::chrono::milliseconds(500) );

    //     l1.addElement( "New Element " + std::to_string( i ) );

    //     // Draw the UI
    //     screen.draw();
    // }

    return 0;
}