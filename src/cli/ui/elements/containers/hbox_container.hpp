#pragma once

#include <iostream>
#include <stdexcept>
#include "container.hpp"

namespace ccl::cli::ui
{
    /**
     * An HBox (Horizontal Box) Container is a particular Layout Manager
     * that organizes children horizontally inside the panel. That is, all
     * children are placed from left to rigth with not wrap at the end.
     * The width of the panel migth be completely or partially filled depending
     * on the children total width and their flexibility factor. If none of them
     * is flex, then they will be placed according to their native resolution.
     * On the other hand, flex widgets migth grow or shrink based on the sum
     * of all children width and the actual available space provided by the panel.
     * On the other hand, the heigth of the panel is resized according to the
     * maximum heigth among all the children.
     * 
     * Children are placed vertically according to the Vertical Alignment type
     * of the panel widget. There are four possible values: Top, Center, Bottom
     * and Stretch. 
     */
    class HBoxContainer : public Container
    {
    public:
        using Container::Container;
        virtual ~HBoxContainer() = default;

        void layout() override;
    };
}