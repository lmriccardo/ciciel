#include "widget.hpp"

using namespace ccl::cli::ui;

Widget::Widget(const std::string &id, size_t w, size_t h, 
               size_t x, size_t y, bool leaf
) : UIElement( w, h, x, y ), 
    m_name( id ), 
    m_leaf( leaf ), 
    m_parent( nullptr )
{
}

void Widget::setParent(Widget &parent)
{
    if ( m_parent != nullptr )
    {
        std::cerr << "[Widget:setParent] WARNING!! Overriding "
                  << "the existing parent of the current widget "
                  << "with Id: " << getId() << " and Parent ID: "
                  << m_parent->getId()
                  << std::endl;
    }

    m_parent = &parent;
}

void Widget::setBorderStyle(const BorderStyle &style)
{
    m_border = style;
}

void Widget::setVisibility(bool visibility)
{
    m_hidden = !visibility;
}

bool Widget::isVisible() const
{
    return !m_hidden;
}

bool Widget::isLeaf() const
{
    return m_leaf;
}

bool Widget::hasChildren() const
{
    return !isLeaf();
}

const std::string& Widget::getId() const
{
    return m_name;
}

std::string Widget::getAbsoluteId() const
{
    if ( m_parent == nullptr ) return m_name;
    return m_parent->getAbsoluteId() + "." + m_name;
}
