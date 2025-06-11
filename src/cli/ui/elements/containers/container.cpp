#include "container.hpp"

using namespace ccl::cli::ui;

Container::Container( PanelBase* panelptr )
    : m_panel( panelptr )
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

int Container::removeChildById(const std::string &id)
{
    auto it_children = m_children.begin();
    for ( ; it_children != m_children.end(); ++it_children )
    {
        if ( (*it_children)->getId() == id ) break;
    }

    // If the requested element does not exists in the subtree
    if ( it_children == m_children.end() ) return 0;

    m_children.erase( it_children );
    return 0;
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
