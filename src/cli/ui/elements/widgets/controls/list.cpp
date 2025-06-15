#include "list.hpp"

using namespace ccl::cli::ui;

void List::drawPadding( ScreenBuffer& buffer ) const
{
    size_t t_pad = getPadding( Direction::Top   );
    size_t b_pad = getPadding( Direction::Bottom );
    size_t max_v_pad = ( t_pad > b_pad ) ? t_pad : b_pad;

    size_t b_size = static_cast<size_t>( m_border.getBorderWcwidth() );
    size_t size_w = getWinsize().second - 2 * b_size;
    std::u32string pad = std::u32string( size_w, U' ' );
    Style pad_style = DefaultStyle();

    size_t b_pos_y = m_pos_y + 1 + t_pad + getContentWinsize().second;
    size_t t_pos_y = m_pos_y + 1;

    for ( size_t p_idx = 0; p_idx < max_v_pad; ++p_idx )
    {
        if ( p_idx < t_pad )
        {
            buffer.set( pad, t_pos_y + p_idx, m_pos_x + b_size, pad_style, false );
        }

        if ( p_idx < b_pad )
        {
            buffer.set( pad, b_pos_y + p_idx, m_pos_x + b_size, pad_style, false );
        }
    }
}

List::List(const std::string &id, const elem_t &elems, size_t x, size_t y, size_t max_size)
    : ContentWidget( id, 1, 1, x, y, true ), m_max_nof_elems( max_size )
{
    m_content_style.resize( max_size );
    m_content.resize( max_size );
    std::copy( elems.begin(), elems.end(), m_content.begin() );

    // Select the maximum element size to set the content size
    size_t max_elem_size = 0;
    size_t idx = 0;
    for ( ; idx < elems.size(); ++idx )
    {
        size_t curr_size = static_cast<size_t>(u32swidth(elems[idx]));
        if ( curr_size > max_elem_size ) max_elem_size = curr_size;
        m_content_style[idx] = DefaultStyle();
    }

    m_curr_idx = idx;

    setContentWinsize( max_elem_size, elems.size() );
    setContentMinimumSize( max_elem_size, elems.size() );

    m_content_alignment = TextAlignment::Left;
}

List::List(const std::string &id, size_t x, size_t y, size_t max_size)
    : List( id, {}, x, y, max_size )
{
}

List &List::addElement(const std::u32string &element, const Style &style)
{
    size_t content_idx = ( m_curr_idx + 1 ) % m_max_nof_elems;
    m_content[content_idx] = element;
    m_content_style[content_idx] = style;

    size_t content_w = getContentWinsize().first;
    size_t element_w;
    bool w_changed = false;

    if ( (element_w = static_cast<size_t>(u32swidth(element))) > content_w )
    {
        content_w = element_w;
        w_changed = true;
    }

    setContentWinsize( content_w, m_content.size() );
    setContentMinimumSize( content_w, m_content.size() );

    if ( m_curr_idx + 1 < m_max_nof_elems || w_changed )
    {
        forceParentRepack(); // window size of the widget has changed
    }

    m_curr_idx = ( m_curr_idx + 1 ) % m_max_nof_elems; // Step to the next insert position

    return *this;
}

List &List::addElement(const std::string &element, const Style &style)
{
    return addElement( utf8to32( element ), style );
}

List &List::addElement(const std::u32string &element)
{
    return addElement( element, DefaultStyle() );
}

List &List::addElement(const std::string &element)
{
    return addElement( element, DefaultStyle() );
}

void List::setTextAlignment(TextAlignment value)
{
    m_content_alignment = value;
}

size_t List::getMaxNofElements() const
{
    return m_max_nof_elems;
}

void List::draw( ScreenBuffer& buffer ) const
{
    if (!isVisible()) return;
    this->ContentWidget<elem_t>::draw( buffer ); // Draw the border if visible

    size_t b_size = static_cast<size_t>( m_border.getBorderWcwidth() );
    size_t r_pad = getPadding( Direction::Rigth );
    size_t l_pad = getPadding( Direction::Left  );
    size_t t_pad = getPadding( Direction::Top   );

    // Draw the content
    auto [content_w, content_h] = getContentWinsize();
    for ( size_t idx = 0; idx < m_content.size(); ++idx )
    {
        const std::u32string& content_i = m_content[idx];
        const Style& style_i = m_content_style[idx];

        std::u32string aligned_i = u32align( content_i, m_content_alignment, content_w );
        std::u32string padded_i  = u32pad( aligned_i, r_pad, l_pad );

        buffer.set( padded_i, m_pos_y + t_pad + 1 + idx, 
            m_pos_x + b_size, style_i, false );
    }

    drawPadding( buffer );
}