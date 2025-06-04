#pragma once

#include <string>
#include "colors.hpp"

namespace ccl::cli::ui
{
    struct Style
    {
        Color m_foreground;
        Color m_background;

        bool m_italic     = false;
        bool m_bold       = false;
        bool m_underlined = false;
        bool m_blink      = false;

        bool m_has_foreground = false;
        bool m_has_background = false;
    
        Style& Foreground( const Color& );
        Style& Background( const Color& );

        Style& Italic    ( bool );
        Style& Bold      ( bool );
        Style& Underlined( bool );
        Style& Blink     ( bool );

        bool operator==( const Style& ) const;
        bool operator!=( const Style& ) const;
    };

    // Defines a Default style 
    inline const Style& DefaultStyle()
    {
        static Style style;
        return style;
    }

    enum class CursorStyle
    {
        BlinkBlock      = 1,
        SteadyBlock     = 2,
        BlinkUnderline  = 3,
        SteadyUnderline = 4,
        BlinkBar   	    = 5,
        SteadyBar       = 6
    };
};