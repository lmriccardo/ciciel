#include "widget.hpp"
#include <cli/ui/elements/widgets/panels/panel_base.hpp>

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

void Widget::forceParentRepack()
{
    if ( m_parent != nullptr )
    {
        reinterpret_cast<PanelBase*>(m_parent)->setRepacking( true );
    }
}

void Widget::drawMargin(ScreenBuffer & buffer) const
{
    auto draw = [&buffer]( size_t w, size_t h, size_t x, size_t y )
    {
        Style m_style = DefaultStyle();
        std::u32string margin = std::u32string( w, U' ' );
        for ( size_t m_idx = 0; m_idx < h; ++m_idx )
        {
            buffer.set( margin, y + m_idx, x, m_style, false );
        }
    };

    size_t t_margin, b_margin, l_margin, r_margin;

    if ( ( t_margin = getMargin( Direction::Top ) ) != 0 )
    {
        draw( getWinsizeWithMargin().first, t_margin, m_pos_x - 1, m_pos_y - t_margin );
    }

    if ( ( b_margin = getMargin( Direction::Bottom ) ) != 0 )
    {
        draw( getWinsizeWithMargin().first, b_margin, m_pos_x - 1, m_pos_y + getWinsize().first);
    }

    if ( ( l_margin = getMargin( Direction::Left ) ) != 0 )
    {
        draw( l_margin, getWinsize().first, m_pos_x - l_margin, m_pos_y);
    }

    if ( ( r_margin = getMargin( Direction::Rigth ) ) != 0 )
    {
        draw( r_margin, getWinsize().first, m_pos_x + getWinsize().second, m_pos_y);
    }
}

Widget::Widget(const std::string &id, size_t w, size_t h, size_t x, size_t y, bool leaf) 
    : m_name(id), m_hidden(false), m_leaf(leaf),
      m_parent(nullptr), m_pos_x( x ), m_pos_y( y )
{
    w = std::max( static_cast<int>(w), 2 * m_border.getBorderWcwidth() );
    h = std::max( static_cast<int>(h), 2 );

    setWinsize( w, h );

    m_padding.fill( 0 );
    m_margin.fill( 0 );
}

void Widget::setParent(Widget &parent)
{
    // Check that the parent is convertible to ParentBase class
    if ( !(dynamic_cast<PanelBase*>(&parent)) )
    {
        throw std::invalid_argument( "Input parent must be a panel!!" );
    }

    if ( m_parent != nullptr )
    {
        // When changing the parent of the current widget
        // we also need to remove it from the previous subtree
        reinterpret_cast<PanelBase*>(m_parent)->removeChild( getId() );
    }

    m_parent = &parent;
}

void Widget::setBorderStyle(const BorderStyle &style)
{
    m_border = style;
}

void Widget::setVisibility(bool visibility)
{
    if ( m_hidden == !visibility ) return;

    m_hidden = !visibility;
    
    // When changing the visibility of the widget, it also changes
    // the way in which children of the widget parent are organized
    // by the layout manager (container), therefore we need to
    // force the repack.
    forceParentRepack();
}

void Widget::setWinsize( size_t width, size_t height )
{
    m_winsize.ws_row = height;
    m_winsize.ws_col = width;
}

void Widget::setContentWinsize(size_t width, size_t height)
{
    size_t b_size = static_cast<size_t>(m_border.getBorderWcwidth());
    size_t w_size = width + 2 * b_size + m_padding[1] + m_padding[2];
    size_t h_size = height + 2 + m_padding[0] + m_padding[3];

    m_winsize.ws_row = h_size;
    m_winsize.ws_col = w_size;
}

void Widget::setStartPosition(size_t s_x, size_t s_y)
{
    m_pos_x = s_x;
    m_pos_y = s_y;
}


void Widget::setPadding(const std::array<size_t, 4> &padding)
{
    setPadding( padding.at(0), Direction::Top    );
    setPadding( padding.at(1), Direction::Left   );
    setPadding( padding.at(2), Direction::Rigth  );
    setPadding( padding.at(3), Direction::Bottom );
}

void Widget::setPadding(size_t value, Direction direction)
{
    int direction_i = static_cast<int>(direction);
    m_padding[direction_i] = value;

    if (direction == Direction::Top || direction == Direction::Bottom)
    {
        m_winsize.ws_row = m_winsize.ws_row + value;
        return;
    }

    m_winsize.ws_col = m_winsize.ws_col + value;

    // We need to force the parent repacking because the
    // size of the current label has changed
    forceParentRepack();
}

void Widget::setMargin(const std::array<size_t, 4> &margin)
{
    setMargin( margin.at(0), Direction::Top    );
    setMargin( margin.at(1), Direction::Left   );
    setMargin( margin.at(2), Direction::Rigth  );
    setMargin( margin.at(3), Direction::Bottom );
}

void Widget::setMargin(size_t value, Direction direction)
{
    int direction_i = static_cast<int>(direction);
    m_margin[direction_i] = value;

    // We need to force the parent repacking because the
    // size of the current label has changed
    forceParentRepack();
}

void Widget::setMinimumSize(size_t w, size_t h)
{
    m_min_size = { w, h };
}

void Widget::setContentMinimumSize(size_t width, size_t height)
{
    size_t b_size = static_cast<size_t>( m_border.getBorderWcwidth() );
    m_min_size.first = width + 2 * b_size + m_padding[1] + m_padding[2];
    m_min_size.second = height + 2 + m_padding[0] + m_padding[3];
}

void Widget::setGrowFactor(int factor)
{
    if ( factor < 0 )
    {
        throw std::invalid_argument( "Grow factor cannot be negative" );
    }

    m_flexGrow = factor;
}

void Widget::setShrinkFactor(int factor)
{
    if ( factor < 0 )
    {
        throw std::invalid_argument( "Shrink factor cannot be negative" );
    }

    m_flexShrink = factor;
}

std::pair<size_t, size_t> Widget::getWinsize() const
{
    return { m_winsize.ws_row, m_winsize.ws_col };
}

std::pair<size_t, size_t> Widget::getVertexCoord(Vertex v) const
{
    switch (v)
    {
        case Vertex::TL: return { m_pos_x, m_pos_y };
        case Vertex::TR: return { m_pos_x + m_winsize.ws_col - 1, m_pos_y };
        case Vertex::BL: return { m_pos_x, m_pos_y + m_winsize.ws_row - 1 };
        case Vertex::BR: return {
            m_pos_x + m_winsize.ws_col - 1,
            m_pos_y + m_winsize.ws_row - 1
        };
        
        default:
            throw std::invalid_argument("Unknow vertex direction");
            return {};
    }
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

bool Widget::canGrow() const
{
    return m_flexGrow > 0;
}

bool Widget::canShrink() const
{
    return m_flexShrink > 0;
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

size_t Widget::getPadding(Direction direction) const
{
    int di = static_cast<int>( direction );
    return m_padding[ di ];
}

size_t Widget::getMargin(Direction direction) const
{
    int di = static_cast<int>( direction );
    return m_margin[ di ];
}

std::pair<size_t, size_t> Widget::getContentWinsize() const
{
    size_t rpad = m_padding[2], lpad = m_padding[1];
    size_t tpad = m_padding[0], bpad = m_padding[3];
    size_t b_size = (size_t)m_border.getBorderWcwidth();
    size_t col_size = m_winsize.ws_col - 2 * b_size - rpad - lpad;
    size_t row_size = m_winsize.ws_row - 2 - tpad - bpad;

    return { col_size, row_size };
}

int Widget::getShrinkFactor() const
{
    return m_flexShrink;
}

int Widget::getGrowFactor() const
{
    return m_flexGrow;
}

const std::pair<size_t, size_t> &Widget::getMinimumSize() const
{
    return m_min_size;
}

std::pair<size_t, size_t> Widget::getWinsizeWithMargin() const
{
    size_t tmarg = getMargin( Direction::Top    );
    size_t lmarg = getMargin( Direction::Left   );
    size_t rmarg = getMargin( Direction::Rigth  );
    size_t bmarg = getMargin( Direction::Bottom );

    return { m_winsize.ws_col + lmarg + rmarg, m_winsize.ws_row + tmarg + bmarg };
}

const BorderStyle &Widget::getBorderStyle() const
{
    return m_border;
}

size_t Widget::getX() const
{
    return m_pos_x + getPadding( Direction::Left ) + m_border.getBorderWcwidth();
}

size_t Widget::getY() const
{
    return m_pos_y + getPadding( Direction::Top ) + m_border.getBorderWcwidth();
}

void Widget::setBorderVisibility(bool value)
{
    m_border.Show( value );
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
    drawMargin( buffer );
}