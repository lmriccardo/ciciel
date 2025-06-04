#include "style.hpp"

using namespace ccl::cli::ui;

Style &Style::Foreground(const Color &f_color)
{
    m_foreground = f_color;
    m_has_foreground = true;
    return *this;
}

Style &Style::Background(const Color &b_color)
{
    m_background = b_color;
    m_has_background = true;
    return *this;
}

Style &Style::Italic(bool value)
{
    m_italic = value;
    return *this;
}

Style &Style::Bold(bool value)
{
    m_bold = value;
    return *this;
}

Style &Style::Underlined(bool value)
{
    m_underlined = value;
    return *this;
}

Style &Style::Blink(bool value)
{
    m_blink = value;
    return *this;
}

bool Style::operator==(const Style &other) const
{
    if (
           (m_has_background != other.m_has_background)
        || (m_has_foreground != other.m_has_foreground)
        || (m_background.hex() != other.m_background.hex())
        || (m_foreground.hex() != other.m_foreground.hex())
        || (m_italic != other.m_italic)
        || (m_bold != other.m_bold)
        || (m_underlined != other.m_underlined)
        || (m_blink != other.m_blink)
    ) {
        return false;
    }

    return true;
}

bool Style::operator!=(const Style &other) const
{
    return !( *this == other );
}
