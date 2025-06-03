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
    };

    // Defines a Default style 
    inline const Style& DefaultStyle()
    {
        static Style style;
        return style;
    }
};