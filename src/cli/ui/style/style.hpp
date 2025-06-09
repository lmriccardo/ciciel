#pragma once

#include <string>
#include <cli/ui/utils/ascii.hpp>
#include <cli/ui/utils/string.hpp>
#include "colors.hpp"

namespace ccl::cli::ui
{
    enum class TextAlignment
    {
        Rigth,
        Left,
        Center
    };

    struct Style
    {
        Color m_foreground;
        Color m_background;

        bool m_italic     = false;
        bool m_bold       = false;
        bool m_underlined = false;
        bool m_blink      = false;
        bool m_reverse    = false;

        TextAlignment m_alignment = TextAlignment::Rigth;

        bool m_has_foreground = false;
        bool m_has_background = false;
    
        Style& Foreground( const Color& );
        Style& Background( const Color& );

        Style& Italic    ( bool );
        Style& Bold      ( bool );
        Style& Underlined( bool );
        Style& Blink     ( bool );
        Style& Reverse   ( bool );
        Style& Alignment ( TextAlignment );

        bool operator==( const Style& ) const;
        bool operator!=( const Style& ) const;
    };

    // Defines a Default style 
    inline const Style& DefaultStyle()
    {
        static Style style;
        style.Foreground( Colors::White );
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

    enum class BorderLine
    {
        Smooth,
        Segmented
    };

    enum class BorderSize
    {
        Unit,
        Double
    };

    struct BorderStyle
    {
        Color      m_color = Colors::White;
        bool       m_show  = true;
        BorderLine m_line  = BorderLine::Smooth;
        BorderSize m_size  = BorderSize::Unit;

        Style toStyle() const;

        BorderStyle& Foreground( const Color& );
        BorderStyle& Show      ( bool );
        BorderStyle& Line      ( BorderLine );
        BorderStyle& Size      ( BorderSize );

        int getBorderWcwidth() const;

        bool operator==( const BorderStyle& ) const;
        bool operator!=( const BorderStyle& ) const;
    };
};