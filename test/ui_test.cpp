#include <cli/ui.hpp>
#include <thread>

using namespace ccl::cli::ui;

class A {};

int main()
{
    UIApplication app( "Simple UI Application" );
    
    Screen& screen = app.getScreen();
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

    VBoxPanel p1( "SubPanel1", 0, 0, 0, 0 );
    p1.setTitle( "New Panel" );
    p1.setGrowFactor( 1 );
    p1.setShrinkFactor( 1 );
    p1.setMargin( 1, Direction::Left );
    p1.setMargin( 1, Direction::Rigth );

    Button b( "Button1", "Quit" );
    b.setMargin( { 0, 1, 1, 0 } );
    p1.addChild( b );

    b.onClick.connect( std::bind( &UIApplication::quit, &app, std::placeholders::_1 ) );

    InputBox ib( "InputBox1", "Enter email" );
    ib.setMaxLength( 20 );
    ib.setLabel( U"✉ Email:" );
    ib.setBorderVisibility( true );

    p1.addChild( ib );
    
    // Add the label to the screen main panel
    screen.addWidget( l );
    screen.addWidget( l1 );
    screen.addWidget( p1 );

    app.run();

    return 0;
}