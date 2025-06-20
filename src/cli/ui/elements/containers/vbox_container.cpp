#include "vbox_container.hpp"

using namespace ccl::cli::ui;

VBoxContainer::VBoxContainer( PanelBase* panel )
    : FlowContainer( FlowDirection::Vertical, panel )
{
}

size_t VBoxContainer::getMainAxisMinSize(const Widget &child) const
{
    return child.getMinimumSize().second;
}

size_t VBoxContainer::getMainAxisSize(const Widget &child) const
{
    return child.getWinsize().first;
}

size_t VBoxContainer::getCrossAxisSize(const Widget &child) const
{
    return child.getWinsize().second;
}

size_t VBoxContainer::getMainAxisSizeWithMargin(const Widget &child) const
{
    return child.getWinsizeWithMargin().second;
}

size_t VBoxContainer::getCrossAxisSizeWithMargin(const Widget &child) const
{
    return child.getWinsizeWithMargin().first;
}

size_t VBoxContainer::getMainMarginSizeStart(const Widget &child) const
{
    return child.getMargin( Direction::Top );
}

size_t VBoxContainer::getMainMarginSizeEnd(const Widget &child) const
{
    return child.getMargin( Direction::Bottom );
}

size_t VBoxContainer::getCrossMarginSizeStart(const Widget &child) const
{
    return child.getMargin( Direction::Left );
}

size_t VBoxContainer::getCrossMarginSizeEnd(const Widget &child) const
{
    return child.getMargin( Direction::Rigth );
}

size_t VBoxContainer::getPanelAvailableMainAxis() const
{
    return m_panel->getContentWinsize().second;
}

size_t VBoxContainer::getPanelAvailableCrossAxis() const
{
    return m_panel->getContentWinsize().first;
}

size_t VBoxContainer::getPanelMainPos() const
{
    return m_panel->getY();
}

size_t VBoxContainer::getPanelCrossPos() const
{
    return m_panel->getX();
}

int VBoxContainer::getAlignment() const
{
    return static_cast<int>( m_panel->getHorizontalAlignment() );
}

void VBoxContainer::setPanelContentMainAxis(size_t size)
{
    m_panel->setContentHeight( size );
}

void VBoxContainer::setPanelContentCrossAxis(size_t size)
{
    m_panel->setContentWidth( size );
}

void VBoxContainer::setWidgetMainAxisSize(Widget &child, size_t size)
{
    child.setHeight( size );
}

void VBoxContainer::setWidgetCrossAxisSize(Widget &child, size_t size)
{
    child.setWidth( size );
}

void VBoxContainer::setWidgetPos(Widget &child, size_t main_pos, size_t cross_pos)
{
    child.setStartPosition( cross_pos, main_pos );
}