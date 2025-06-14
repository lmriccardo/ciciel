#pragma once

#include <unordered_map>
#include <cli/ui/elements/widgets/widget.hpp>

namespace ccl::cli::ui
{
    // Vertical component alignment in a panel that uses HBox Layout management
    enum class VerticalAlignment { Top, Center, Bottom, Stretch };

    // Horizontal component alignment in a panel that uses VBox Layout management
    enum class HorizontalAlignment { Left, Center, Right, Stretch };

    /**
     * Base class for all panels. It is used only for pointer
     * casting and downcasting.
     */
    class PanelBase : public Widget
    {
    protected:
        std::unordered_map<std::string, Widget*> m_childIds;

        VerticalAlignment   m_v_align = VerticalAlignment::Top;
        HorizontalAlignment m_h_align = HorizontalAlignment::Left;

        bool m_needs_repacking = true;

    public:
        PanelBase( const std::string&, size_t, size_t, size_t, size_t, bool );
        virtual ~PanelBase() = default;

        virtual void addChild( Widget& ) = 0;
        virtual void removeChild( const std::string& ) = 0;
        virtual void pack() = 0;

        /**
         * If the current panel needs repacking. In case the given value
         * is positive, then repacking is applied to parent widget.
         * 
         * @param value True if needs repacking, False otherwise
         */
        void setRepacking( bool );
        void setVerticalAlignment( VerticalAlignment );
        void setHorizontalAlignment( HorizontalAlignment );

        VerticalAlignment getVerticalAlignment() const;
        HorizontalAlignment getHorizontalAlignment() const;

        using Widget::draw;
    };
};