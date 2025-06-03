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
