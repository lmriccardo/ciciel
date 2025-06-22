#include "button.hpp"

using namespace ccl::cli::ui;

void Button::onMouseEnterCallback()
{
    m_border.Show( true );
}

void Button::onMouseExitCallback()
{
    m_border.Show( false );
}

Button::Button(const std::string &id, const std::u32string &content, size_t x, size_t y)
    : StringContentWidget(id, U"[ " + content + U" ]", x, y)
{
    onMouseEnter.connect( [this](void) { this->onMouseEnterCallback(); } );
    onMouseExit.connect( [this](void){ this->onMouseExitCallback(); } );
}

Button::Button(const std::string &id, const std::string &content, size_t x, size_t y)
    : Button( id, utf8to32(content), x, y )
{
}

Button::Button(const std::string &id, const std::u32string &content)
    : Button( id, content, 0, 0 )
{
}

Button::Button(const std::string &id, const std::string &content)
    : Button( id, content, 0, 0 )
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
