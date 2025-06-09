#pragma once

#include <termios.h>
#include <unistd.h>
#include <term.h>
#include <stdexcept>
#include <iostream>
#include <locale>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <sys/ioctl.h>
#include <cli/ui/style/style.hpp>
#include <cli/ui/utils/string.hpp>
#include "term_cap.hpp"

namespace ccl::cli::ui
{
    class Terminal
    {
    private:
        Style   m_prev_style;    // Previous style used when writing to stdout
        termios m_original_p;    // Original setting before rawing
        bool    m_isRaw = false; // If the current terminal setting is raw

        Terminal();

    public:
        ~Terminal();

        Terminal( const Terminal& ) = delete;
        Terminal& operator=( const Terminal& ) = delete;

        static Terminal& getInstance();

        void enableRawMode();  // Set the terminal to Raw mode for interactive UI
        void disableRawMode(); // Reset the terminal to its original state
        
        void put( char32_t, size_t, size_t, const Style& ); // Put a char into the terminal

        template <typename... _Args>
        int callCap(const char* capname, _Args&&... args) const;

        void reset() const;
        void setStyle(const Style&) const;
        void setCursorPosition( size_t, size_t ) const;

        static void getWindowSize(struct winsize*);
    };

    template <typename... _Args>
    inline int Terminal::callCap(const char *capname, _Args &&...args) const
    {
        const char* str = tigetstr(capname);
        if ( !str || str == (char*)-1 ) return -1;

        if constexpr (sizeof...(_Args) > 0)
        {
            char* expanded = tparm( (char*)str, std::forward<_Args>(args)... );
            write( STDOUT_FILENO, expanded, strlen(expanded) );
            return 0;
        }

        write( STDOUT_FILENO, str, strlen(str) );
        return 0;
    }
}