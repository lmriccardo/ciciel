#include "label.hpp"

using namespace ccl::cli::ui;

Label::Label(const std::string &id, const std::u32string &text, size_t x, size_t y)
    : ContentWidget(id, (size_t)u32swidth(text), 3, x, y, false)
{
    setContent( text );
}

Label::Label(const std::string &id, const std::string &text, size_t x, size_t y)
    : Label(id, utf8to32(text), x, y)
{
}

void Label::setContentStyle(const Style &style)
{
    m_content_style = style;
}

void Label::setContent(const std::string &content, const Style &style)
{
    utf8to32( content, m_content );
    m_content_style = style;
}

void Label::setContent(const std::u32string &content, const Style &style)
{
    m_content = content;
    m_content_style = style;
}

void Label::setContent(const std::string &content)
{
    setContent( content, DefaultStyle() );
}

void Label::setContent(const std::u32string &content)
{
    setContent( content, DefaultStyle() );
}

void Label::draw( ScreenBuffer& buffer ) const
{
    if (!isVisible()) return;
    this->ContentWidget<std::u32string>::draw( buffer ); // Draw the border if visible

    auto [ _, w ] = getWinsize();
    size_t b_with = static_cast<size_t>( m_border.getBorderWcwidth() );
    std::u32string content = u32align( m_content, 
        m_content_style.m_alignment, w - 2*b_with );

    std::cerr << "Drawing the content : " << std::endl;
    buffer.set( content, m_pos_y + 1, m_pos_x + 1, m_content_style, false );
}