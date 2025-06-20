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

    /**
     * A Widget is the most basic UI element. I have divided widgets into
     * to groups: Container widgets ( i.e., Panels ) and Content/Controls Widgets
     * ( e.g., Labels, TextBox and so on ). Each widget has its own identifier
     * which must be unique in the subtree to which it belongs to. In general
     * all content widgets are leaf in the UI tree, while container widgets
     * are generic node. That is, only container widgets can have children.
     * 
     * Each widget can have a border, defined by its Border Style that can be
     * either shown or not. In both cases, border width and position is always
     * taken into account when computing widget window size and position. 
     * Obviously, each widget has its own window size and position, however these
     * values migth be changed by its parent container during layout organization.
     * 
     * Along with the border and its content, a widget can also have padding and
     * margin values in all 4 directions ( top, bottom, left and rigth ). In
     * particular: padding is the space between the content and the border, while
     * margin is the space between the border and other widgets. That is, a widget
     * total size is always computed as: margin + border + padding + content. 
     * 
     * Finally, widgets placed inside panel containers can grow and shrink in size.
     * How much it is mutable in size depends on the grow and shrink factor. A
     * factor different from 0 it means that the overall size of the widget can be
     * decided by its panel container when performing layout operations. Altought 
     * it can shrink, a minimum size can be set to limit this behavior and keep
     * visualization consistent.
     */
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

        int m_flexGrow    = 0; // Flex grow factor ( 0 = can not grow )
        int m_flexShrink  = 0; // Flex shrink factor ( 0 = can not shrink )
        bool m_need_clear = true; // The first time ever the widget is drawn it must be cleared

        std::pair<size_t, size_t> m_min_size; // Widget minimum size when shrinking

        virtual void drawBorder( ScreenBuffer& ) const;
        void drawRect( ScreenBuffer&, const char32_t*, size_t, size_t, const Style&) const;
        void forceParentRepack();
        void drawMargin( ScreenBuffer& ) const;
    
    public:
        Widget( const std::string& id, size_t w, size_t h, size_t x, size_t y, bool leaf );
        virtual ~Widget() = default;
        
        /**
         * Set a parent for the current widget. The parent must be a PanelBase
         * derived class, i.e., a Panel object otherwise an exception will
         * be raised. In the case the widget has a previous parent, it will be
         * removed from the previous subtree and added to the new one.
         * 
         * @param parent The new parent of the widget
         */
        void setParent( Widget& parent );

        /**
         * Set the border style of the current widget
         * @param border The border style
         */
        void setBorderStyle( const BorderStyle& border );

        /**
         * Set the visibility flag to the widget.
         * @param value True if visible, False otherwise
         */
        void setVisibility( bool value );

        /**
         * Set the starting (x,y) position of the widget.
         * @param x The X starting position
         * @param y The Y starting position
         */
        void setStartPosition( size_t x, size_t y );

        /**
         * Set the new winsize. This method aims to set the entire width
         * and height of the widget window. Therefore, input parameters
         * must already take into account top/left/rigth/bottom padding.
         * 
         * @param width The new number of columns
         * @param height The new number of rows
         */
        void setWinsize( size_t width, size_t height );

        void setWidth( size_t w );
        void setHeight( size_t h );

        /**
         * Set the new content window size. Therefore, input values
         * must represent the two dimension of the content, not the
         * entire widget window. This is important to consider,
         * otherwise undefined behaviour will happen.
         * 
         * @param width The size in columns of the content window
         * @param height The size in lines/rows of the content window
         */
        void setContentWinsize( size_t width, size_t height );

        /**
         * Set the content width of the widget.
         * @param width The width to set with no padding and border
         */
        void setContentWidth( size_t width );

        /**
         * Set the content height of the widget.
         * @param height The height to set with no padding and border
         */
        void setContentHeight( size_t height );

        /**
         * Set the padding for all directions. In particular the input array
         * should be organized as follow: ( Top, Left, Rigth, Bottom ) padding.
         * Setting padding operation also increases the width and the height.
         * 
         * @param padding The actual padding to apply
         */
        void setPadding( const std::array<size_t, 4>& padding );

        /**
         * Set the padding in the given direction.
         * 
         * @param padding The padding size to set
         * @param direction The direction to which apply the padding
         */
        void setPadding( size_t padding, Direction direction );

        /**
         * Just like padding, it set the margin for all directions. The array
         * should be organized in the same way, therefore: (Top, Left, Rigth, Bottom).
         * Since the margin does not concern the internal content of the widget, its
         * size will remain the same at the end of the operation.
         * 
         * @param margin The margin to apply
         */
        void setMargin( const std::array<size_t, 4>& margin );

        /**
         * Set the margin in the given direction.
         * 
         * @param margin The margin size to apply
         * @param direction The direction to which apply the margin
         */
        void setMargin( size_t margin, Direction direction );

        /**
         * Sets the minimum size for the current widget. The minimum size
         * is the size the layout manager must respect if there is no enough
         * space to contain all the children.
         * 
         * @param width The minimum width ( nof columns )
         * @param height The minimum height ( nof rows )
         */
        void setMinimumSize( size_t width, size_t height );

        /**
         * Sets the minimum content size for the current widget.
         * 
         * @param width The minimum content width ( nof columns )
         * @param height The minimum content height ( nof rows )
         */
        void setContentMinimumSize( size_t width, size_t height );

        /**
         * Sets its grow factor.
         * @param factor The grow factor
         */
        void setGrowFactor( int factor );

        /**
         * Sets its shrink factor
         * @param factor The shrink factor
         */
        void setShrinkFactor( int factor );

        bool isVisible  () const;
        bool isLeaf     () const;
        bool hasChildren() const;
        bool canGrow    () const;
        bool canShrink  () const;

        /**
         * Returns the overall window size, i.e., border +
         * padding + content. Margin is taken into account
         * in another method.
         */
        std::pair<size_t,size_t> getWinsize() const;
        std::pair<size_t, size_t> getVertexCoord( Vertex v ) const;
        
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
        size_t getPadding( Direction direction ) const;

        /**
         * Returns the margin size of the given direction
         * @param direction The direction requested
         * @return The margin size
         */
        size_t getMargin( Direction direction ) const;

        /**
         * Returns a pair (columns, rows) representing respectively
         * the number of columns and rows without any horizontal or
         * vertical padding and border.
         * 
         * @return < cols, rows >
         */
        std::pair<size_t,size_t> getContentWinsize() const;

        int getShrinkFactor() const;
        int getGrowFactor() const;

        /**
         * Returns the minimum size for this widget
         * @return ( width, height )
         */
        const std::pair<size_t, size_t>& getMinimumSize() const;

        /**
         * Returns widget window dimension accounting also top, left,
         * rigth and bottom margin.
         * 
         * @return ( width + margin, height + margin )
         */
        std::pair<size_t, size_t> getWinsizeWithMargin() const;

        const BorderStyle& getBorderStyle() const;
        size_t getX() const;
        size_t getY() const;

        void setBorderVisibility( bool value );

        /**
         * Clear the widget.
         */
        void clear( ScreenBuffer& buffer );

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
        virtual void draw( ScreenBuffer& );
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