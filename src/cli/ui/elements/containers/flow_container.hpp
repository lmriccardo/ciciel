#pragma once

#include "container.hpp"

namespace ccl::cli::ui
{
    enum class FlowDirection { Horizontal, Vertical };

    class FlowContainer : public Container
    {
    protected:
        FlowDirection m_direction;

        static size_t computeSize( size_t factor, size_t total_factor, 
            size_t child_size, int space );

    public:
        FlowContainer( FlowDirection dir, PanelBase* panel );
        virtual ~FlowContainer() = default;

        virtual size_t getMainAxisMinSize( const Widget& child ) const = 0;
        virtual size_t getMainAxisSize( const Widget& child ) const = 0;
        virtual size_t getCrossAxisSize( const Widget& child ) const = 0;
        virtual size_t getMainAxisSizeWithMargin( const Widget& child ) const = 0;
        virtual size_t getCrossAxisSizeWithMargin( const Widget& child ) const = 0;
        virtual size_t getMainMarginSizeStart( const Widget& child ) const = 0;
        virtual size_t getMainMarginSizeEnd( const Widget& child ) const = 0;
        virtual size_t getCrossMarginSizeStart( const Widget& child ) const = 0;
        virtual size_t getCrossMarginSizeEnd( const Widget& child ) const = 0;

        virtual size_t getPanelAvailableMainAxis() const = 0;
        virtual size_t getPanelAvailableCrossAxis() const = 0;
        virtual size_t getPanelMainPos() const = 0;
        virtual size_t getPanelCrossPos() const = 0;

        virtual int getAlignment() const = 0;

        virtual void setPanelContentMainAxis(size_t size) = 0;
        virtual void setPanelContentCrossAxis(size_t size) = 0;

        virtual void setWidgetMainAxisSize( Widget& child, size_t size ) = 0;
        virtual void setWidgetCrossAxisSize( Widget& child, size_t size ) = 0;
        virtual void setWidgetPos( Widget& child, size_t main_pos, size_t cross_pos ) = 0;

        void layout() override;
    };
}