#include "widget.hpp"

using namespace ccl::cli::ui;

void Widget::drawBorder( ScreenBuffer& buffer ) const
{
    if ( !m_border.m_show ) return;

    Style s = DefaultStyle();
    s.Foreground( m_border.m_color );

    const char32_t* charset = m_border.getCharset().data();
    drawRect( buffer, charset, 0, 0, s );
}

void Widget::drawRect(ScreenBuffer &buffer, const char32_t *charset, 
    size_t x_off, size_t y_off, const Style& style) const
{
    if ( !is_codepoint_valid( charset + 5 ) )
    {
        throw std::runtime_error( "Not enough character to draw the rectangle!" );
    }

    size_t x_pos = m_pos_x + x_off;
    size_t y_pos = m_pos_y + y_off;

    auto [ r, c ] = getWinsize();

    buffer.set( *(charset), y_pos, x_pos, style, false );
    buffer.set( *(charset + 2), y_pos, x_pos + c - 1, style, false );

    for ( size_t c_idx = 1; c_idx < c - 1; ++c_idx )
    {
        buffer.set( *(charset + 1), y_pos, x_pos + c_idx, style, false );
        buffer.set( *(charset + 1), y_pos + r - 1 , x_pos + c_idx, style, false );
    }

    for ( size_t r_idx = 1; r_idx < r - 1; ++r_idx )
    {
        buffer.set( *(charset + 3), y_pos + r_idx, x_pos, style, false );
        buffer.set( *(charset + 3), y_pos + r_idx, x_pos + c - 1, style, false );
    }

    buffer.set( *(charset + 4), y_pos + r - 1, x_pos, style, false );
    buffer.set( *(charset + 5), y_pos + r - 1, x_pos + c - 1, style, false );
}

Widget::Widget(const std::string &id, size_t w, size_t h,
               size_t x, size_t y, bool leaf) 
    : UIElement(w, h, x, y),
      m_name(id),
      m_hidden(false),
      m_leaf(leaf),
      m_parent(nullptr)
{
    // Reset the window size considering also the border
    size_t b_size = static_cast<size_t>(m_border.getBorderWcwidth());
    setWinsize( w + 2*b_size, h );
}

void Widget::setParent(Widget &parent)
{
    if ( m_parent != nullptr )
    {
        std::cerr << "[Widget:setParent] WARNING!! Overriding "
                  << "the existing parent of the current widget "
                  << "with Id: " << getId() << " and Parent ID: "
                  << m_parent->getId()
                  << std::endl;
    }

    m_parent = &parent;
}

void Widget::setBorderStyle(const BorderStyle &style)
{
    m_border = style;
}

void Widget::setVisibility(bool visibility)
{
    m_hidden = !visibility;
}

void Widget::setPadding(const std::array<size_t, 4> &padding)
{
    m_padding = padding;
}

void Widget::setPadding(size_t value, Direction direction)
{
    int direction_i = static_cast<int>(direction);
    m_padding[direction_i] = value;
}

void Widget::setMargin(const std::array<size_t, 4> &margin)
{
    m_margin = margin;
}

void Widget::setMargin(size_t value, Direction direction)
{
    int direction_i = static_cast<int>(direction);
    m_margin[direction_i] = value;
}

bool Widget::isVisible() const
{
    return !m_hidden;
}

bool Widget::isLeaf() const
{
    return m_leaf;
}

bool Widget::hasChildren() const
{
    return !isLeaf();
}

const std::string& Widget::getId() const
{
    return m_name;
}

std::string Widget::getAbsoluteId() const
{
    if ( m_parent == nullptr ) return m_name;
    return m_parent->getAbsoluteId() + "." + m_name;
}

bool Widget::hasContent() const
{
    return false;
}

bool Widget::isColliding( size_t x, size_t y ) const
{
    auto [ x_e, y_s ] = getVertexCoord( Vertex::TR );
    auto [ x_s, y_e ] = getVertexCoord( Vertex::BL );
 
    return (( x >= x_s && x <= x_e ) && ( y >= y_s && y <= y_e ));
}

void Widget::draw( ScreenBuffer& buffer ) const
{
    if (!isVisible()) return; // Draw only if the current widget is visible
    drawBorder( buffer );
}