#include "label.hpp"

using namespace ccl::cli::ui;

void Label::setContentStyle(const Style &style)
{
    m_content_style = style;
}

Style &Label::getContentStyle()
{
    return m_content_style;
}