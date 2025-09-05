#pragma once

#include "flow_container.hpp"

namespace ccl::cli::ui
{
    class VBoxContainer : public FlowContainer
    {
    public:
        VBoxContainer( PanelBase* panel );
        virtual ~VBoxContainer() = default;

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