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

Style &Style::Reverse(bool value)
{
    m_reverse = value;
    return *this;
}

Style &Style::Alignment(TextAlignment alignment)
{
    m_alignment = alignment;
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
        || (m_reverse != other.m_reverse)
        || (m_alignment != other.m_alignment)
    ) {
        return false;
    }

    return true;
}

bool Style::operator!=(const Style &other) const
{
    return !( *this == other );
}

BorderStyle& BorderStyle::Foreground( const Color& color )
{
    m_color = color;
    return *this;
}

BorderStyle &BorderStyle::Show(bool value)
{
    m_show = value;
    return *this;
}

BorderStyle &BorderStyle::Line(BorderLine line)
{
    m_line = line;
    return *this;
}

BorderStyle &BorderStyle::Size(BorderSize size)
{
    m_size = size;
    return *this;
}

int BorderStyle::getBorderWcwidth() const
{
    const char32_t b_char = ( m_size == BorderSize::Double ) ?
          ASCII::DOUBLE_HORIZONTAL
        : ASCII::LIGHT_HORIZONTAL;

    return charwidth( &b_char );
}

const std::array<char32_t, 6> ccl::cli::ui::BorderStyle::getCharset() const
{
    if ( m_size == BorderSize::Unit )
    {
        return { 
            ASCII::DOUBLE_DOWN_RIGHT, ASCII::DOUBLE_HORIZONTAL,
            ASCII::DOUBLE_DOWN_LEFT,  ASCII::DOUBLE_VERTICAL,
            ASCII::DOUBLE_UP_RIGHT,   ASCII::DOUBLE_UP_LEFT };
    }
    else
    {
        return { 
            ASCII::LIGHT_DOWN_RIGHT, ASCII::LIGHT_HORIZONTAL,
            ASCII::LIGHT_DOWN_LEFT,  ASCII::LIGHT_VERTICAL,
            ASCII::LIGHT_UP_RIGHT,   ASCII::LIGHT_UP_LEFT };
    }
}

Style BorderStyle::toStyle() const
{
    return Style().Foreground(m_color).Bold( m_size == BorderSize::Double );
}

bool BorderStyle::operator==(const BorderStyle & other) const
{
    if (
           (m_color.hex() == other.m_color.hex())
        && (m_line == other.m_line)
        && (m_show == other.m_show)
        && (m_size == other.m_size)
    ) {
        return true;
    }

    return false;
}

bool BorderStyle::operator!=(const BorderStyle &other) const
{
    return !( *this == other );
}