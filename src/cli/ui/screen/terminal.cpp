#include "terminal.hpp"

using namespace ccl::cli::ui;

Terminal::Terminal()
{
    int err;

    if ( setupterm( nullptr, STDOUT_FILENO, &err ) != F_OK )
    {
        throw std::runtime_error( "Failed to init terminfo: " + std::to_string(err) );
    }

    // Enables UTF-8 and correct locale settings
    setlocale( LC_ALL, "" );
    enableRawMode();
}

Terminal::~Terminal()
{
    disableRawMode();
}

Terminal &ccl::cli::ui::Terminal::getInstance()
{
    static std::unique_ptr<Terminal> ptr{ new Terminal() };
    return *ptr;
}

void Terminal::enableRawMode()
{
    if ( tcgetattr( STDIN_FILENO, &m_original_p ) == -1 )
    {
        perror( "[Terminal::enableRawMode] tcgetattr() failed" );
        throw std::runtime_error( "Error" );
    }

    termios raw_p = m_original_p;

    // ~ECHO   -> Disable echoing input characters
    // ~ICANON -> Set the terminal to non-canonical mode
    // ~ISIG   -> Prevents classical signals to be applied (like CTRL+C)
    raw_p.c_lflag &= ~( ECHO | ICANON | ISIG );

    if ( tcsetattr( STDIN_FILENO, TCSAFLUSH, &raw_p ) == -1 )
    {
        perror( "[Terminal::enableRawMode] tcsetattr() failed" );
        throw std::runtime_error( "Error" );
    }

    m_isRaw = true;
}

void Terminal::disableRawMode()
{
    if ( m_isRaw )
    {
        tcsetattr( STDIN_FILENO, TCSAFLUSH, &m_original_p );
        m_isRaw = false;
    }
}

void Terminal::put(char c, size_t x, size_t y, const Style& style)
{
    if ( style != m_prev_style )
    {
        reset();           // Reset the style
        setStyle( style ); // First set the style
        
        m_prev_style = style;
    }

    setCursorPosition( x, y );    // 2. Set the cursor position
    write( STDOUT_FILENO, &c, 1); // 3. Write the content
}

void Terminal::reset() const
{
    if (callCap(TCapabilities::RESET) < 0)
    {
        std::cerr << "Unable to reset the terminal." << std::endl;
    }
}

void Terminal::setStyle(const Style& style) const
{
    if ( style.m_bold )       callCap( TCapabilities::BOLD_TEXT );
    if ( style.m_blink )      callCap( TCapabilities::BLINK_TEXT );
    if ( style.m_underlined ) callCap( TCapabilities::START_UNDERLINE );
    if ( style.m_italic )     callCap( TCapabilities::START_ITALIC );

    if ( style.m_has_background )
    {
        callCap( TCapabilities::BACKGROUND_C, style.m_background.m_xterm_number );
    }

    if ( style.m_has_foreground )
    {
        callCap( TCapabilities::FOREGROUND_C, style.m_foreground.m_xterm_number );
    }
}

void Terminal::setCursorPosition(size_t x_pos, size_t y_pos) const
{
    if ( callCap( TCapabilities::MOVE_CURSOR, x_pos, y_pos ) < 0 )
    {
        std::cerr << "Unable to set cursor position" << std::endl;
    }
}

void Terminal::getWindowSize(struct winsize* ws)
{
    if ( ioctl( STDOUT_FILENO, TIOCGWINSZ, ws ) == -1 )
    {
        throw std::runtime_error( "Failed to get window size" );
    }
}
