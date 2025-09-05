#pragma once

#include <iostream>
#include <stdexcept>
#include "flow_container.hpp"

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
    class HBoxContainer : public FlowContainer
    {
    public:
        HBoxContainer( PanelBase* panel );
        virtual ~HBoxContainer() = default;

        size_t getMainAxisMinSize( const Widget& child ) const override;
        size_t getMainAxisSize( const Widget& child ) const override;
        size_t getCrossAxisSize( const Widget& child ) const override;
        size_t getMainAxisSizeWithMargin( const Widget& child ) const override;
        size_t getCrossAxisSizeWithMargin( const Widget& child ) const override;
        size_t getMainMarginSizeStart( const Widget& child ) const override;
        size_t getMainMarginSizeEnd( const Widget& child ) const override;
        size_t getCrossMarginSizeStart( const Widget& child ) const override;
        size_t getCrossMarginSizeEnd( const Widget& child ) const override;

        size_t getPanelAvailableMainAxis() const override;
        size_t getPanelAvailableCrossAxis() const override;
        size_t getPanelMainPos() const override;
        size_t getPanelCrossPos() const override;

        int getAlignment() const override;

        void setPanelContentMainAxis(size_t size) override;
        void setPanelContentCrossAxis(size_t size) override;

        void setWidgetMainAxisSize( Widget& child, size_t size ) override;
        void setWidgetCrossAxisSize( Widget& child, size_t size ) override;
        void setWidgetPos( Widget& child, size_t main_pos, size_t cross_pos ) override;
    };
}