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
    /**
     * Low-level wrapper to the actual STDOUT and STDIN. It is a singleton.
     * It setups the terminal by enabling and disabling raw mode, set styles
     * using ANSI escape sequence and write content directly into the STDOUT.
     */
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
        
        /**
         * Put a char into the terminal at given position with given style.
         */
        void put( char32_t c, size_t x, size_t y, const Style& style );

        template <typename... _Args>
        int callCap(const char* capname, _Args&&... args) const;

        void reset() const;
        void setStyle(const Style& style) const;
        void setCursorPosition( size_t x, size_t y ) const;

        static void getWindowSize(struct winsize* ws);
        static std::string getCap( const char* capname );
        static void enableMouseEvents();
        static void disableMouseEvents();
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