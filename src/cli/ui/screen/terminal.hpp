#pragma once

#include <termios.h>
#include <unistd.h>
#include <term.h>
#include <wchar.h>
#include <stdexcept>
#include <iostream>
#include <locale>

namespace ccl::cli::ui::screen
{
    class Terminal
    {
    private:
        termios m_original_p;    // Original setting before rawing
        bool    m_isRaw = false; // If the current terminal setting is raw

    public:
        Terminal();
        ~Terminal();

        void enableRawMode();  // Set the terminal to Raw mode for interactive UI
        void disableRawMode(); // Reset the terminal to its original state
        void put( char );      // Put a char into the terminal
        void put( const std::string& ); // Put a UTF-8 string into the terminal

        template <typename... _Args>
        void callCap(const char* capname, _Args&&... args);
    };

    template <typename... _Args>
    inline void Terminal::callCap(const char *capname, _Args &&...args)
    {
        const char* str = tigetstr(capname);
        if ( str == (char*)-1 ) return;

        if constexpr (sizeof...(_Args) > 0)
        {
            char* expanded = tparm( (char*)str, std::forward<_Args>(args)... );
            putp( expanded );
            return;
        }

        putp( str );
    }
}