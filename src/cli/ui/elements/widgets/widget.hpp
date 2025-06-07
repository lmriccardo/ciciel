#pragma once

#include <string>
#include <unordered_map>
#include <cli/ui/style/style.hpp>
#include <cli/ui/screen/screen_buffer.hpp>
#include "../ui_element.hpp"

namespace ccl::cli::ui
{
    class Widget: public UIElement
    {
    protected:
        std::string m_name;   // The unique name identifier in the subtree
        BorderStyle m_border; // The style of the widget border
        bool        m_hidden; // If the widget is hidden or not
        bool        m_leaf;   // If the widget is a leaf in the tree or not
        Widget*     m_parent; // The widget parent in the UI tree
    
    public:
        Widget( const std::string&, size_t, size_t, size_t, size_t, bool );
        virtual ~Widget() = default;
        
        void setParent( Widget& );
        void setBorderStyle( const BorderStyle& );
        void setVisibility(bool);

        bool isVisible() const;
        bool isLeaf() const;
        bool hasChildren() const;

        const std::string& getId() const;

        /**
         * Returns the entire Id starting from the root node
         * up to the current widget. Assuming this node is at
         * depth level N, then the fully qualified (absolute) 
         * ID will be: <id1>.<id2>....<currId>.
         * 
         * @return The absolute ID from the root node.
         */
        std::string getAbsoluteId() const;

        /**
         * Check whether the input coordinates collides with
         * the current object. In particular if the are inside
         * the bounds of the current render.
         * 
         * @param x_pos The input X coordinate
         * @param y_pos The input Y coordinate
         * @return True if collision, False otherwise
         */
        virtual bool isColliding( size_t, size_t ) const = 0;

        virtual void draw( const ScreenBuffer& ) const = 0;
    };
}