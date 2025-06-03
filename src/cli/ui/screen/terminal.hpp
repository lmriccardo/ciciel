#pragma once

#include <termios.h>
#include <unistd.h>
#include <term.h>
#include <wchar.h>
#include <stdexcept>
#include <iostream>
#include <locale>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <sys/ioctl.h>
#include <unordered_map>
#include <cli/ui/style/style.hpp>

#include "term_cap.hpp"

namespace ccl::cli::ui
{
    class Terminal
    {
    private:
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
        
        void put( char32_t, const Style& );           // Put a char into the terminal
        void put( const std::string&, const Style& ); // Put a UTF-8 string into the terminal

        template <typename... _Args>
        int callCap(const char* capname, _Args&&... args) const;

        void reset() const;

        static std::pair<int, int> getWindowSize();
    };

    template <typename... _Args>
    inline int Terminal::callCap(const char *capname, _Args &&...args) const
    {
        const char* str = tigetstr(capname);
        if ( str && str == (char*)-1 ) return -1;

        if constexpr (sizeof...(_Args) > 0)
        {
            char* expanded = tparm( (char*)str, std::forward<_Args>(args)... );
            putp( expanded );
            return 0;
        }

        putp( str );
        return 0;
    }
}