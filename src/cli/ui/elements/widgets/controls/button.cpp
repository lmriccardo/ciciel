#include "button.hpp"

using namespace ccl::cli::ui;

Button::Button(const std::string &id, const std::u32string &content, size_t x, size_t y)
    : StringContentWidget( id, U"[ " + content + U" ]", x, y )
{
}

Button::Button(const std::string &id, const std::string &content, size_t x, size_t y)
    : Button( id, utf8to32(content), x, y )
{
}

void Button::setContent(const std::u32string &text)
{
    this->StringContentWidget::setContent( U"[ " + text + U" ]", DefaultStyle() );
}

void Button::setClicked(bool value)
{
    m_clicked = value;
}

bool Button::isClicked() const
{
    return m_clicked;
}
