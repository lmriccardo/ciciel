#pragma once

#include <string>
#include <unordered_map>
#include <array>
#include <cli/ui/style/style.hpp>
#include <cli/ui/screen/screen_buffer.hpp>
#include "../ui_element.hpp"

namespace ccl::cli::ui
{
    enum class Direction { Top, Left, Rigth, Bottom };

    class Widget: public UIElement
    {
    protected:
        std::string m_name;    // The unique name identifier in the subtree
        BorderStyle m_border;  // The style of the widget border
        bool        m_hidden;  // If the widget is hidden or not
        bool        m_leaf;    // If the widget is a leaf in the tree or not
        Widget*     m_parent;  // The widget parent in the UI tree

        std::array<size_t, 4> m_padding; // Internal space between content and border
        std::array<size_t, 4> m_margin;  // External space between widget and other objects

        virtual void drawBorder( ScreenBuffer& ) const = 0;
    
    public:
        Widget( const std::string&, size_t, size_t, size_t, size_t, bool );
        virtual ~Widget() = default;
        
        void setParent     ( Widget& );
        void setBorderStyle( const BorderStyle& );
        void setVisibility ( bool );
        void setPadding    ( const std::array<size_t, 4>& );
        void setMargin     ( const std::array<size_t, 4>& );
        void setPadding    ( size_t, Direction );
        void setMargin     ( size_t, Direction );

        bool isVisible  () const;
        bool isLeaf     () const;
        bool hasChildren() const;
        
        const std::string& getId() const;

        /**
         * Returns the entire Id starting from the root node up to the current widget. 
         * Assuming this node is at depth level N, then the fully qualified (absolute) 
         * ID will be: <id1>.<id2>....<currId>.
         * 
         * @return The absolute ID from the root node.
         */
        std::string getAbsoluteId() const;

        /**
         * Check whether the input coordinates collides with the current object. 
         * In particular if the are inside the bounds of the current render.
         * 
         * @param x The input X coordinate
         * @param y The input Y coordinate
         * @return True if collision, False otherwise
         */
        virtual bool isColliding( size_t, size_t ) const;
        virtual bool hasContent() const;

        /**
         * Write the widget into the Screen Buffer
         */
        virtual void draw( ScreenBuffer& ) const = 0;
    };

    template <typename T>
    class ContentWidget : public Widget
    {
    protected:
        T m_content; // The content of the widget
    
    public:
        using Widget::Widget;
        virtual ~ContentWidget() = default;

        const T& getContent() const; // Returns the content
        bool hasContent() const override;
    };

    template <typename T>
    inline const T &ContentWidget<T>::getContent() const
    {
        return m_content;
    }

    template <typename T>
    inline bool ContentWidget<T>::hasContent() const
    {
        return true;
    }
}