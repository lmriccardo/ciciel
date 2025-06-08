#include "container.hpp"

using namespace ccl::cli::ui;

Container::Container(size_t w, size_t h, size_t x, size_t y)
    : UIElement( w, h, x, y )
{
}

void Container::addChild(Widget* child)
{
    m_children.push_back( child );
}

int Container::removeChild(size_t idx)
{
    if ( idx >= getNofChildren() ) return 0;
    m_children.erase(m_children.begin() + idx);
    return 1;
}

Widget* Container::getChild(size_t idx)
{
    if ( idx >= getNofChildren() ) return nullptr;
    return m_children[idx];
}

size_t Container::getNofChildren() const
{
    return m_children.size();
}

const std::vector<Widget *> &Container::getAllChildren() const
{
    return m_children;
}

std::vector<Widget *> &Container::getAllChildren()
{
    return m_children;
}
