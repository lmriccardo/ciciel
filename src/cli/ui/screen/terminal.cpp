#include "terminal.hpp"

using namespace ccl::cli::ui;

Terminal::Terminal()
{
    if (isatty( STDOUT_FILENO ) != 1)
    {
        throw std::runtime_error( "Current STDOUT is not a tty!" );
    }

    int err;

    if ( setupterm( nullptr, STDOUT_FILENO, &err ) != F_OK )
    {
        throw std::runtime_error( "Failed to init terminfo: " + std::to_string(err) );
    }

    // Enables UTF-8 and correct locale settings
    enableRawMode();
    setlocale( LC_ALL, "" );
}

Terminal::~Terminal()
{
    disableRawMode();
    disableMouseEvents();
}

Terminal &Terminal::getInstance()
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
    raw_p.c_cc[VMIN] = 0;
    raw_p.c_cc[VTIME] = 1; // 100ms timeout

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
        if ( tcsetattr( STDIN_FILENO, TCSAFLUSH, &m_original_p ) == -1 )
        {
            // It's generally not good to throw from a destructor,
            // but for debugging this might be useful.
            // In a real application, you might just log this error.
            perror( "[Terminal::disableRawMode] tcsetattr() failed" );
        }
        m_isRaw = false;
    }
}

void Terminal::put(char32_t c, size_t x, size_t y, const Style& style)
{
    if ( style != m_prev_style )
    {
        reset();           // Reset the style
        setStyle( style ); // First set the style
        
        m_prev_style = style;
    }

    setCursorPosition( x, y );    // 2. Set the cursor position
    
    ssize_t len;
    char u8c[4]; if ( (len = utf32to8c( &c, u8c )) < 0 ) return;
    write( STDOUT_FILENO, u8c, len ); // 3. Write the content
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
    if ( style.m_reverse )    callCap( TCapabilities::REVERSE_VIDEO );

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

std::string Terminal::getCap(const char *capname)
{
    const char* cap = tigetstr( capname );
    if ( !cap || cap == (char*)-1 ) return "";
    return std::string( cap );
}

void Terminal::enableMouseEvents()
{
    printf("\033[?1002h\033[?1006h\n");
    fflush( stdout );
}

void Terminal::disableMouseEvents()
{
    printf("\033[?1002l\033[?1006l\n");
    fflush( stdout );
}
