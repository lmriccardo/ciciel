#include "screen.hpp"

using namespace ccl::cli::ui;

int Screen::_id = 0;

void Screen::fallBack()
{
    setCursorStyle( m_cinfo.m_default_style );
    Terminal::getInstance().reset();
}

void Screen::layoutSelection(Layout layout)
{
    m_layout = layout;

    // Let's create the panel with the given layout
    switch ( m_layout )
    {
        case Layout::HorizontalLayout:
            m_panel = createPanel<HBoxPanel>(); break;
        case Layout::VerticalLayout:
            m_panel = createPanel<VBoxPanel>(); break;
        case Layout::GridLayout:
        case Layout::AbsoluteLayout:
        default:
            fallBack();
            throw std::runtime_error( "Unknown layout type" );
    }
}

Screen::Screen(Layout layout)
    : m_terminal(Terminal::getInstance()),
      m_layout(layout)
{
    if (_id > 0) 
    {
        throw std::runtime_error("No more Screen instances can be created");
    }

    Terminal::getWindowSize(&m_winsize);
    m_buffer = std::make_unique<ScreenBuffer>((size_t)m_winsize.ws_col, 
        (size_t)m_winsize.ws_row);

    layoutSelection( layout );

    _id++;
}

Screen::~Screen()
{
    fallBack();
}

void Screen::enableMouse() const
{
    Terminal::enableMouseEvents();
}

void Screen::setCursorStyle(CursorStyle style)
{
    std::cout << "\033[" << static_cast<int>(style) << " q";
    std::cout.flush();

    m_cinfo.m_style = style;
}

void Screen::setCursorPosition(int x_pos, int y_pos)
{
    if ( ( x_pos < 0 ) || ( y_pos < 0 ) )
    {
        fallBack();
        throw std::runtime_error( "setCursorPosition: Cannot set negative position" );
    }

    if ( ( x_pos >= m_winsize.ws_row ) || ( y_pos >= m_winsize.ws_col ) )
    {
        fallBack();
        throw std::out_of_range( "setCursorPosition: Out of Bound position" );
    }

    m_cinfo.m_xpos = static_cast<size_t>(x_pos);
    m_cinfo.m_ypos = static_cast<size_t>(y_pos);
    m_terminal.setCursorPosition( x_pos, y_pos );
}

void Screen::setCursorVisibility(bool value)
{
    if ( !value ) {
        if ( m_terminal.callCap( TCapabilities::HIDE_CURSOR ) < 0 )
        {
            std::cerr << "Call HIDE_CURSOR capability failed" << std::endl;
        }
    } else {
        if ( m_terminal.callCap( TCapabilities::SHOW_CURSOR ) < 0 )
        {
            std::cerr << "Call HIDE_CURSOR capability failed" << std::endl;
        }
    }

    m_cinfo.m_hidden = !value;
}

void Screen::setLayout(Layout layout)
{
    layoutSelection(layout);
}

void Screen::setVerticalAlignment( LayoutAlignment value )
{
    m_panel->setVerticalAlignment( value );
}

void Screen::setHorizontalAlignment( LayoutAlignment value )
{
    m_panel->setHorizontalAlignment( value );
}

const Layout &Screen::getLayout() const
{
    return m_layout;
}

void Screen::moveCursor(int x_off, int y_off)
{
    setCursorPosition( m_cinfo.m_xpos + x_off, m_cinfo.m_ypos + y_off );
}

void Screen::clear()
{
    m_terminal.callCap( TCapabilities::CLEAR_SCREEN );
    setCursorPosition( 0, 0 );
}

void Screen::addWidget(Widget & widget)
{
    m_panel->addChild( widget );
}

Widget *Screen::getCollidingWidget(size_t x, size_t y) const
{
    return m_panel->getCollidingWidget( x, y );
}

void Screen::draw()
{
    m_panel->pack();                // 1. Pack if necessary
    m_panel->draw( *m_buffer );     // 2. Draw into the buffer
    m_buffer->flush( m_terminal );  // 3. Flush to the terminal
    
    // Hide the cursor
    setCursorVisibility( false );
}
