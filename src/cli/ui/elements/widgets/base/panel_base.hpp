#pragma once

#include <unordered_map>
#include "widget.hpp"

namespace ccl::cli::ui
{
    enum class LayoutAlignment { Start, Center, End, Streatch };

    /**
     * Base class for all panels. It is used only for pointer
     * casting and downcasting.
     */
    class PanelBase : public Widget
    {
    private:
        constexpr static size_t TITLE_OFFSET = 3;    

    protected:
        std::unordered_map<std::string, Widget*> m_childIds;

        std::u32string m_title;
        Style          m_title_style;
        size_t         m_parent_idx;
        
        int m_children_idx = -1;

        LayoutAlignment m_v_align = LayoutAlignment::Start;
        LayoutAlignment m_h_align = LayoutAlignment::Start;

        bool m_needs_repacking = true;

        void drawTitle( ScreenBuffer& ) const;

    public:
        PanelBase( const std::string& id, size_t w, size_t h, size_t x, size_t y, bool leaf );
        virtual ~PanelBase() = default;

        virtual void addChild( Widget& widget ) = 0;
        virtual void removeChild( const std::string& id ) = 0;
        virtual void pack() = 0;
        virtual void prepack() = 0;

        /**
         * If the current panel needs repacking. In case the given value
         * is positive, then repacking is applied to parent widget.
         * 
         * @param value True if needs repacking, False otherwise
         */
        void setRepacking( bool value );
        void setVerticalAlignment( LayoutAlignment value );
        void setHorizontalAlignment( LayoutAlignment value );

        void setTitle( const std::u32string& title, const Style& style );
        void setTitle( const std::string& title, const Style& style );
        void setTitle( const std::u32string& title );
        void setTitle( const std::string& title );

        void setParent( Widget& parent );

        const std::u32string& getTitle() const;
        Style& getTitleStyle();

        LayoutAlignment getVerticalAlignment() const;
        LayoutAlignment getHorizontalAlignment() const;

        int getCurrentChildIndex() const;

        /**
         * Returns the first colliding widget to position (x, y) of the
         * cursor. If the widget is a panel is search between its children.
         * That is, on success, this function will always returns a valid
         * pointer to leaf widget, otherwise nullptr.
         * 
         * @param x The X position of the cursor
         * @param y The Y position of the cursor
         * 
         * @return A pointer to a leaf or null pointer.
         */
        Widget* getCollidingWidget( size_t x, size_t y ) const;

        bool needsRepacking() const;

        using Widget::draw;
    };
};