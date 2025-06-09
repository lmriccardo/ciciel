#include "label.hpp"

using namespace ccl::cli::ui;

void Label::drawBorder(ScreenBuffer &buffer) const
{

}

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
    if (!isVisible()) return; // Draw only if the current widget is visible

    auto [ _, w ] = getWinsize();
    std::u32string content = u32align( m_content, m_content_style.m_alignment, w);
    buffer.set( content, m_pos_x, m_pos_y, m_content_style, false );
}