#include "hbox_container.hpp"

using namespace ccl::cli::ui;

HBoxContainer::HBoxContainer(PanelBase *panel)
    : FlowContainer( FlowDirection::Horizontal, panel )
{
}

size_t HBoxContainer::getMainAxisMinSize(const Widget &child) const
{
    return child.getMinimumSize().first;
}

size_t HBoxContainer::getMainAxisSize(const Widget &child) const
{
    return child.getWinsize().second;
}

size_t HBoxContainer::getCrossAxisSize(const Widget &child) const
{
    return child.getWinsize().first;
}

size_t HBoxContainer::getMainAxisSizeWithMargin(const Widget &child) const
{
    return child.getWinsizeWithMargin().first;
}

size_t HBoxContainer::getCrossAxisSizeWithMargin(const Widget &child) const
{
    return child.getWinsizeWithMargin().second;
}

size_t HBoxContainer::getMainMarginSizeStart(const Widget &child) const
{
    return child.getMargin( Direction::Left );
}

size_t HBoxContainer::getMainMarginSizeEnd(const Widget &child) const
{
    return child.getMargin( Direction::Rigth );
}

size_t HBoxContainer::getCrossMarginSizeStart(const Widget &child) const
{
    return child.getMargin( Direction::Top );
}

size_t HBoxContainer::getCrossMarginSizeEnd(const Widget &child) const
{
    return child.getMargin( Direction::Bottom );
}

size_t HBoxContainer::getPanelAvailableMainAxis() const
{
    return m_panel->getContentWinsize().first;
}

size_t HBoxContainer::getPanelAvailableCrossAxis() const
{
    return m_panel->getContentWinsize().second;
}

size_t HBoxContainer::getPanelMainPos() const
{
    return m_panel->getX();
}

size_t HBoxContainer::getPanelCrossPos() const
{
    return m_panel->getY();
}

int HBoxContainer::getAlignment() const
{
    return static_cast<int>( m_panel->getVerticalAlignment() );
}

void HBoxContainer::setPanelContentMainAxis(size_t size)
{
    m_panel->setContentWidth( size );
}

void HBoxContainer::setPanelContentCrossAxis(size_t size)
{
    m_panel->setContentHeight( size );
}

void HBoxContainer::setWidgetMainAxisSize(Widget &child, size_t size)
{
    child.setWidth( size );
}

void HBoxContainer::setWidgetCrossAxisSize(Widget &child, size_t size)
{
    child.setHeight( size );
}

void HBoxContainer::setWidgetPos(Widget &child, size_t main_pos, size_t cross_pos)
{
    child.setStartPosition( main_pos, cross_pos );
}
