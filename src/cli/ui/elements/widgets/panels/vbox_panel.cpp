#include "vbox_panel.hpp"

using namespace ccl::cli::ui;

void VBoxPanel::prepack()
{
    auto [ panel_w, panel_h ] = getContentWinsize();

    size_t total_children_min_h = 0;
    size_t max_children_w = 0;

    for ( auto& child: m_container->getAllChildren() )
    {
        if ( auto ptr = dynamic_cast<PanelBase*>(child) )
        {
            ptr->prepack();
        }

        const size_t child_w = child->getWinsizeWithMargin().first;
        const size_t min_child_h = child->getMinimumSize().second;

        total_children_min_h += min_child_h;

        if ( max_children_w < child_w )
        {
            max_children_w = child_w;
        }
    }

    if ( panel_w < max_children_w )
    {
        setContentWidth( max_children_w );
    }

    if ( panel_h < total_children_min_h )
    {
        setContentHeight( total_children_min_h );
    }
}