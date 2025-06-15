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
        std::u32string m_title;
        Style          m_title_style;

        VerticalAlignment   m_v_align = VerticalAlignment::Top;
        HorizontalAlignment m_h_align = HorizontalAlignment::Left;

        bool m_needs_repacking = true;

        void drawTitle( ScreenBuffer& ) const;

    public:
        PanelBase( const std::string& id, size_t w, size_t h, size_t x, size_t y, bool leaf );
        virtual ~PanelBase() = default;

        virtual void addChild( Widget& widget ) = 0;
        virtual void removeChild( const std::string& id ) = 0;
        virtual void pack() = 0;

        /**
         * If the current panel needs repacking. In case the given value
         * is positive, then repacking is applied to parent widget.
         * 
         * @param value True if needs repacking, False otherwise
         */
        void setRepacking( bool value );
        void setVerticalAlignment( VerticalAlignment value );
        void setHorizontalAlignment( HorizontalAlignment value );

        void setTitle( const std::u32string& title, const Style& style );
        void setTitle( const std::string& title, const Style& style );
        void setTitle( const std::u32string& title );
        void setTitle( const std::string& title );

        const std::u32string& getTitle() const;
        Style& getTitleStyle();

        VerticalAlignment getVerticalAlignment() const;
        HorizontalAlignment getHorizontalAlignment() const;

        bool needsRepacking() const;

        using Widget::draw;
    };
};