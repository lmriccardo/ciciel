#include "hbox_container.hpp"

using namespace ccl::cli::ui;

void HBoxContainer::layout()
{
    // Get the current available width and height of the panel. This value corresponds
    // to the panel windowsize minus the left/rigth padding and twice the border size.
    // This area represents the sub-space where children will be drawn.
    auto [ panel_available_w, panel_available_h ] = m_panel->getContentWinsize();

    size_t total_preferred_width_needed = 0;
    size_t max_child_total_height       = 0; // Child's content + its own vertical padding/margin
    
    int total_flex_grow_factor   = 0;
    int total_flex_shrink_factor = 0;

    // First we need to compute the total size given by all children
    for ( const auto& child: m_children )
    {
        // First we need to add to the baseline width the actual width
        // of the current widget child, which the entire child plus
        // left and right margin
        auto [ child_preferred_w, child_preferred_h ] = child->getWinsizeWithMargin();

        total_preferred_width_needed += child_preferred_w;

        // We also need to collect the minimum possible heigth that
        // the panel should have to correctly contain the children
        if ( max_child_total_height < child_preferred_h )
        {
            max_child_total_height = child_preferred_h;
        }

        // Accumulate flex factors
        total_flex_grow_factor += child->getGrowFactor();
        total_flex_shrink_factor += child->getShrinkFactor();
    }

    // Now we need to set the new heigth if it is grater then the current one
    // otherwise, children will not be displayed well
    if ( panel_available_h < max_child_total_height )
    {
        m_panel->setContentWinsize( panel_available_w, max_child_total_height );
        panel_available_h = max_child_total_height;
    }

    // Once baseline width has been computed and the heigth regulated according
    // to the maximum child number of rows, there are two cases: (1) The total
    // content width ( no padding or border considered ) is less than the baseline
    // width or (2) the total content width is enough and there is remaining space.
    size_t current_x_pos = m_panel->getX();
    size_t current_y_pos = m_panel->getY();

    int space_to_distribute = panel_available_w - total_preferred_width_needed;

    auto ComputeNewWidth = [&]( size_t factor, size_t total_factor, 
        size_t child_w, int space )
    {
        // Compute the grow or shrink amount as the total remaining space * its portion 
        // of the grow/shrink factor with respect to the total computed among all children.
        double factor_d = static_cast<double>(factor);
        double amount = space * ( factor_d / total_factor );

        // Amount will be negative for space < 0
        size_t new_child_width = child_w + static_cast<size_t>(amount);
        return new_child_width;
    };

    if ( (space_to_distribute > 0 && total_flex_grow_factor > 0) || space_to_distribute < 0 )
    {
        if ( space_to_distribute < 0 && total_flex_shrink_factor == 0 )
        {
            // For now just throw
            throw std::runtime_error( "Not enough space for children" );
        }

        for ( auto& child: m_children )
        {
            // Here we need to take the width of the content, since the padding
            // must remain the same even if the widget grows
            auto [width_no_pad, height_no_pad] = child->getContentWinsize();

            // If the child is not flex in the case we are currently focused on
            // then we need to skip this widget and jumpt to the next one
            if ( 
                   (space_to_distribute < 0 && child->getShrinkFactor() == 0)
                || (space_to_distribute > 0 && child->getGrowFactor() == 0)
            ) {
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

            // Compute the new child width and set it
            size_t new_child_width = ComputeNewWidth( child_factor, 
                                                      curr_total_factor, 
                                                      width_no_pad, 
                                                      space_to_distribute );

            // Ensure that, in case of shrink scenario the minimum width for each
            // children must be respected, otherwise strange clipping behaviour will appear
            if ( space_to_distribute < 0 )
            {
                new_child_width = std::max( new_child_width, child->getMinimumSize().first );
            }
            
            child->setContentWinsize( new_child_width, height_no_pad );
        }
    }

    // Now, we can position the child according to the layout computing the new start x 
    // and y position for each of them. Obviously, this calculation must take into
    // account the margin of every child and the padding of the panel widget itself.
    for ( auto& child: m_children )
    {
        auto [ child_w, child_h ] = child->getWinsizeWithMargin();

        // Compute Y position according to the vertical alignment
        size_t child_final_y_pos;
        size_t panel_h = m_panel->getContentWinsize().second;
        
        switch ( m_panel->getVerticalAlignment() )
        {
            case VerticalAlignment::Top: child_final_y_pos = current_y_pos; break;
            case VerticalAlignment::Center:
                child_final_y_pos = current_y_pos + ( panel_available_h - child_h ) / 2;
                break;
            case VerticalAlignment::Bottom:
                child_final_y_pos = current_y_pos + ( panel_available_h - child_h );
                break;
            case VerticalAlignment::Stretch:
                child->setContentWinsize( child->getContentWinsize().first, panel_h );
                child_final_y_pos = current_y_pos;
                break;
            default:
                child_final_y_pos = current_y_pos;
                break;
        }

        // Set the start absolute X and Y position. When setting the starting
        // x position we also need to take into account the child Left margin
        size_t child_final_x_pos = current_x_pos + child->getMargin( Direction::Left );
        child->setStartPosition( child_final_x_pos, child_final_y_pos );
        current_x_pos = current_x_pos + child_w;
    }
}
