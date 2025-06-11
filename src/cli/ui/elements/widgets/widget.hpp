#pragma once

#include <string>
#include <unordered_map>
#include <array>
#include <cli/ui/style/style.hpp>
#include <cli/ui/screen/screen_buffer.hpp>

namespace ccl::cli::ui
{
    enum class Direction { Top, Left, Rigth, Bottom };

    enum class Vertex
    {
        TL, // Top-Left coordinate 
        TR, // Top-Right coordinate
        BL, // Bottom-Left coordinate
        BR  // Bottom-Right coordinate
    };

    class Widget
    {
    protected:
        std::string m_name;    // The unique name identifier in the subtree
        BorderStyle m_border;  // The style of the widget border
        bool        m_hidden;  // If the widget is hidden or not
        bool        m_leaf;    // If the widget is a leaf in the tree or not
        Widget*     m_parent;  // The widget parent in the UI tree

        size_t         m_pos_x;   // Top-left X position
        size_t         m_pos_y;   // Top-left Y position
        struct winsize m_winsize; // The size of the object

        std::array<size_t, 4> m_padding; // Internal space between content and border
        std::array<size_t, 4> m_margin;  // External space between widget and other objects

        int m_flexGrow   = 0; // Flex grow factor ( 0 = can not grow )
        int m_flexShrink = 0; // Flex shrink factor ( 0 = can not shrink )

        std::pair<size_t, size_t> m_preferred_size; // Widget preferred size when no layout applied
        std::pair<size_t, size_t> m_min_size;       // Widget minimum size when shrinking

        virtual void drawBorder( ScreenBuffer& ) const;
        void drawRect( ScreenBuffer&, const char32_t*, size_t, size_t, const Style&) const;
        void forceParentRepack();
    
    public:
        Widget( const std::string&, size_t, size_t, size_t, size_t, bool );
        virtual ~Widget() = default;
        
        void setParent     ( Widget& );
        void setBorderStyle( const BorderStyle& );
        void setVisibility ( bool );

        void setStartPosition( size_t, size_t );

        /**
         * Set the new winsize by overwriting the existing method from
         * UIElement, by considering also padding.
         * 
         * @param cols The new number of columns
         * @param rows The new number of rows
         */
        void setWinsize( size_t, size_t );

        /**
         * Set the padding for all directions. In particular the input array
         * should be organized as follow: ( Top, Left, Rigth, Bottom ) padding.
         * Setting padding operation also increases the width and the height.
         * 
         * @param padding The actual padding to apply
         */
        void setPadding( const std::array<size_t, 4>& );

        /**
         * Set the padding in the given direction.
         * 
         * @param padding The padding size to set
         * @param direction The direction to which apply the padding
         */
        void setPadding( size_t, Direction );

        /**
         * Just like padding, it set the margin for all directions. The array
         * should be organized in the same way, therefore: (Top, Left, Rigth, Bottom).
         * Since the margin does not concern the internal content of the widget, its
         * size will remain the same at the end of the operation.
         * 
         * @param margin The margin to apply
         */
        void setMargin( const std::array<size_t, 4>& );

        /**
         * Set the margin in the given direction.
         * 
         * @param margin The margin size to apply
         * @param direction The direction to which apply the margin
         */
        void setMargin( size_t, Direction );

        /**
         * Set the preferred size for the current widget. The preferred size
         * is the size the layout manager tries to use as a baseline for
         * ordering all its children widget.
         * 
         * @param w The preferred width ( nof columns )
         * @param h The preferred height ( nof rows )
         */
        void setPreferredSize( size_t, size_t );

        /**
         * Sets the minimum size for the current widget. The minimum size
         * is the size the layout manager must respect if there is no enough
         * space to contain all the children.
         * 
         * @param w The minimum width ( nof columns )
         * @param h The minimum height ( nof rows )
         */
        void setMinimumSize( size_t, size_t );

        void setGrowFactor( int );
        void setShrinkFactor( int );

        bool isVisible  () const;
        bool isLeaf     () const;
        bool hasChildren() const;
        bool canGrow    () const;
        bool canShrink  () const;

        std::pair<size_t,size_t> getWinsize() const;
        std::pair<size_t, size_t> getVertexCoord( Vertex ) const;
        
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
         * Returns the padding size of the given direction.
         * @param direction The direction requested
         * @return The padding size
         */
        size_t getPadding( Direction ) const;

        /**
         * Returns the margin size of the given direction
         * @param direction The direction requested
         * @return The margin size
         */
        size_t getMargin( Direction ) const;

        /**
         * Returns a pair (columns, rows) representing respectively
         * the number of columns and rows without any horizontal or
         * vertical padding and border.
         * 
         * @return < cols, rows >
         */
        std::pair<size_t,size_t> getWinsizeNoPadding( ) const;

        int getShrinkFactor() const;
        int getGrowFactor() const;

        const std::pair<size_t, size_t>& getPreferredSize() const;
        const std::pair<size_t, size_t>& getMinimumSize() const;

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
        virtual void draw( ScreenBuffer& ) const;
    };

    template <typename T>
    class ContentWidget : public Widget
    {
    protected:
        T m_content; // The content of the widget

        using Widget::drawBorder;
    
    public:
        using Widget::Widget;
        virtual ~ContentWidget() = default;

        const T& getContent() const; // Returns the content
        bool hasContent() const override;
        
        using Widget::draw;
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