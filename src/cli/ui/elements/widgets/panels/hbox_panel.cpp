#include "hbox_panel.hpp"

using namespace ccl::cli::ui;

void HBoxPanel::prepack()
{
    auto [ panel_w, panel_h ] = getContentWinsize();

    size_t total_children_min_w = 0;
    size_t max_children_h = 0;

    for ( auto& child: m_container->getAllChildren() )
    {
        if ( auto ptr = dynamic_cast<PanelBase*>(child) )
        {
            ptr->prepack();
        }

        const size_t child_h = child->getWinsizeWithMargin().second;
        const size_t min_child_w = child->getMinimumSize().first;

        total_children_min_w += min_child_w;

        if ( max_children_h < child_h )
        {
            max_children_h = child_h;
        }
    }

    if ( panel_w < total_children_min_w )
    {
        setContentWidth( total_children_min_w );
    }

    if ( panel_h < max_children_h )
    {
        setContentHeight( max_children_h );
    }
}
