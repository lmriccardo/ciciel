#include "flow_container.hpp"
#include <cli/ui/elements/widgets/panels/panel_base.hpp>

using namespace ccl::cli::ui;

size_t FlowContainer::computeSize(
    size_t factor, size_t total_factor, size_t child_size, int space
) {
    // Compute the grow or shrink amount as the total remaining space * its
    // portion of the grow/shrink factor with respect to the total computed
    // among all children
    double factor_d = static_cast<double>( factor );
    double amount = space * ( factor_d / total_factor );

    // Amount will be negative for space < 0
    return child_size + static_cast<size_t>( amount );
}

FlowContainer::FlowContainer(FlowDirection dir, PanelBase *panel)
    : Container( panel ), m_direction( dir )
{
}

void FlowContainer::layout()
{
    // Get the current available width and height of the panel. This value corresponds
    // to the panel windowsize minus the left/rigth padding and twice the border size.
    // This area represents the sub-space where children will be drawn.
    size_t panel_available_main = getPanelAvailableMainAxis();
    size_t panel_available_cross = getPanelAvailableCrossAxis();

    size_t total_preferred_main_size_needed = 0;
    size_t max_child_cross_size = 0;

    int total_flex_grow_factor   = 0;
    int total_flex_shrink_factor = 0;

    // First we need to compute the total size given by all children
    for ( const auto& child: m_children )
    {
        if ( !child->isVisible() ) continue; // do not account for hidden children

        // Compute the baseline main size of all children
        size_t child_main_ax_size = getMainAxisSizeWithMargin( *child );
        size_t child_cross_ax_size = getCrossAxisSizeWithMargin( *child );

        total_preferred_main_size_needed += child_main_ax_size;

        // We also need to collect the minimum possible cross size that
        // the panel should have to correctly contain the children
        if ( max_child_cross_size < child_cross_ax_size )
        {
            max_child_cross_size = child_cross_ax_size;
        }

        // Accumulate flex factors
        total_flex_grow_factor += child->getGrowFactor();
        total_flex_shrink_factor += child->getShrinkFactor();
    }

    // Now we need to set the new cross size if the it is grater then
    // the current one, otherwise children will not be displayed well
    if ( panel_available_cross < max_child_cross_size )
    {
        setPanelContentCrossAxis( max_child_cross_size );
        panel_available_cross = max_child_cross_size;
    }

    // Once the baseline size has been computed along the main axis and the cross axis size 
    // regulated according to the maximum child cross size there are two cases: (1) the 
    // total content main size ( no padding or border considered ) is less then the baseline
    // size or (2) the total content along the main axis is enough and there is remaining space
    size_t current_main_pos = getPanelMainPos();
    size_t current_cross_pos = getPanelCrossPos();

    int space_to_distribute = panel_available_main - total_preferred_main_size_needed;
    size_t total_occupied_space = 0;

    if ( (space_to_distribute > 0 && total_flex_grow_factor > 0) || space_to_distribute < 0 )
    {
        if ( space_to_distribute < 0 && total_flex_shrink_factor == 0 )
        {
            // For now just throw
            throw std::runtime_error( "Not enough space for children" );
        }

        for ( auto& child: m_children )
        {
            if ( !child->isVisible() ) continue;

            // Here we need to take the content size along the main axis, since the
            // padding must remain the same even if the widget grows
            size_t child_content_main_size = getMainAxisSize( *child );
            
            // If the child is not flex in the case we are currently focused on
            // then we need to skip this widget and jumpt to the next one
            if ( 
                   (space_to_distribute < 0 && child->getShrinkFactor() == 0)
                || (space_to_distribute > 0 && child->getGrowFactor() == 0)
            ) {
                total_occupied_space += getMainAxisSizeWithMargin( *child );
                continue;
            }

            // Get the current total factor ( either grow or shrink ) and the child one.
            size_t curr_total_factor, child_factor;

            if ( space_to_distribute > 0 )
            {
                curr_total_factor = total_flex_grow_factor;
                child_factor = child->getGrowFactor();
            } else {
                curr_total_factor = total_flex_shrink_factor;
                child_factor = child->getShrinkFactor();
            }

            size_t new_child_main_size = computeSize( 
                child_factor, curr_total_factor, child_content_main_size, space_to_distribute
            );

            // Ensure that using the new size the child does not overlap or overpass the
            // border of the panel.
            size_t margin_s = getMainMarginSizeEnd( *child );
            int overflow_space = (total_occupied_space + new_child_main_size + margin_s) - panel_available_main;
            if ( overflow_space > 0 )
            {
                new_child_main_size = new_child_main_size - overflow_space;
            }

            // Ensure that, in case of shrink scenario the minimum main size for each
            // children must be respected, otherwise strange clipping behavior will appear
            if ( space_to_distribute < 0 )
            {
                new_child_main_size = std::max( new_child_main_size, getMainAxisMinSize(*child) );
            }

            setWidgetMainAxisSize( *child, new_child_main_size );
            total_occupied_space += getMainAxisSizeWithMargin( *child );
        }
    }

    // Now, we can position the child according to the layout computing the new start x 
    // and y position for each of them. Obviously, this calculation must take into
    // account the margin of every child and the padding of the panel widget itself.
    for ( auto& child: m_children )
    {
        if ( !child->isVisible() ) continue;

        size_t child_main_ax_size = getMainAxisSizeWithMargin( *child );
        size_t child_cross_ax_size = getCrossAxisSizeWithMargin( *child );
        size_t child_cross_pos;

        size_t cross_margin_start = getCrossMarginSizeStart( *child );
        size_t cross_margin_end = getCrossMarginSizeEnd( *child );

        // Compute the cross position according to alignment
        int alignment = getAlignment();
        switch ( static_cast<LayoutAlignment>( alignment ) )
        {
            case LayoutAlignment::Start: 
                child_cross_pos = current_cross_pos + cross_margin_start; 
                break;
            case LayoutAlignment::Center:
                child_cross_pos = current_cross_pos + ( panel_available_cross - child_cross_ax_size ) / 2;
                break;
            case LayoutAlignment::End:
                child_cross_pos = current_cross_pos + ( panel_available_cross - child_cross_ax_size ) - cross_margin_end;
                break;
            case LayoutAlignment::Streatch:
                setWidgetCrossAxisSize( *child, panel_available_cross );
                child_cross_pos = current_cross_pos;
                break;
            default:
                child_cross_pos = current_cross_pos; 
                break;
        }

        size_t child_final_main_pos = current_main_pos + getMainMarginSizeStart( *child );
        setWidgetPos( *child, child_final_main_pos, child_cross_pos );
        current_main_pos = current_main_pos + child_main_ax_size;
    }
}