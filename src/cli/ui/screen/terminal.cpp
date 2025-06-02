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

void Terminal::put(char32_t c)
{
    write( STDOUT_FILENO, &c, 1);
}

void Terminal::put(const std::string &str)
{
    write( STDOUT_FILENO, str.c_str(), str.size() );
}

void Terminal::reset() const
{
    if (callCap(Terminal::RESET_STR) < 0)
    {
        std::cerr << "Unable to reset the terminal." << std::endl;
    }
}
