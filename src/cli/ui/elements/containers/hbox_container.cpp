#include "hbox_container.hpp"

using namespace ccl::cli::ui;

void HBoxContainer::layout()
{
    // Debug output
    std::cout << "called layout on HBoxContainer (ID: " << m_panel->getId() << ")" << std::endl;

    // Get the current available width and height of the panel. This value corresponds
    // to the panel windowsize minus the left/rigth padding and twice the border size.
    // This area represents the sub-space where children will be drawn.
    auto [ panel_available_w, panel_available_h ] = m_panel->getWinsizeNoPadding();

    size_t total_preferred_width_needed = 0;
    size_t total_min_width_needed       = 0; // To handle shrinking scenarios
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
        m_panel->setWinsize( panel_available_w, max_child_total_height );
    }

    size_t number_of_children = getNofChildren();

    // Once baseline width has been computed and the heigth regulated according
    // to the maximum child number of rows, there are two cases: (1) The total
    // content width ( no padding or border considered ) is less than the baseline
    // width or (2) the total content width is enough and there is remaining space.

}
