#include "input_box.hpp"

using namespace ccl::cli::ui;

std::pair<size_t, size_t> InputBox::getStartCursorPosition() const
{
    // The starting cursor position, i.e., when there is still the
    // placeholder instead of the actual content, is the relative (0,0)
    // plus the space occupied by the label
    size_t b_width = (size_t)m_border.getBorderWcwidth();
    size_t x_pos = m_pos_x + 1 + (size_t)u32swidth( m_label ) + b_width + 1; 
    return { m_pos_y + 1, x_pos };
}

void InputBox::drawBorder(ScreenBuffer &buffer) const
{
    if ( !m_border.m_show ) return;

    Style s = DefaultStyle();
    s.Foreground( m_border.m_color );

    const char32_t* charset = m_border.getCharset().data();

    size_t lpad = getPadding( Direction::Left );
    size_t x_off = 1 + lpad + (size_t)u32swidth(m_label) + 1;
    size_t w_size = m_content.size() + 2*(size_t)m_border.getBorderWcwidth();
    size_t h_size = getWinsize().first;

    drawRect( buffer, charset, h_size, w_size, x_off, 0, s );
}

void InputBox::setMaxLength(size_t value)
{
    // First check that the input length is grater than the
    // current content length. If it is not the case then
    // we just return.
    if ( value <= m_content.size() ) return;
    
    // Otherwise, we need to resize the content and the styles
    m_content.resize( value );
    m_content_style.resize( value );

    // Compute the total horizontal size and set it as the min one
    size_t b_width = (size_t)m_border.getBorderWcwidth();
    size_t w_size = value;

    if ( m_label.size() > 0 )
    {
        w_size += (size_t)u32swidth( m_label ) + 1 + b_width;
    }

    // Set the new minimum content winsize
    setContentMinimumSize( w_size, 1 );
    setContentWidth( w_size );
}

void InputBox::setLabel(const std::u32string &label, const Style &style)
{
    if ( label.size() < 1 ) return; // The label should have at least one element

    m_label = label;
    m_label_style = style;

    size_t b_width = (size_t)m_border.getBorderWcwidth();
    size_t w_size = getContentWinsize().first;
    w_size += (size_t)u32swidth( m_label ) + 1 + b_width;

    // Set the new minimum content winsize
    setContentMinimumSize( w_size, 1 );
    setContentWidth( w_size );
}

void InputBox::setLabel(const std::string &label, const Style &style)
{
    setLabel( utf8to32( label ), style );
}

void InputBox::setLabel(const std::u32string &label)
{
    setLabel( label, DefaultStyle() );
}

void InputBox::setLabel(const std::string &label)
{
    setLabel( utf8to32( label ) );
}

void InputBox::draw(ScreenBuffer &buffer)
{
    if (!isVisible()) return;
    clear( buffer ); // Clear the buffer
    this->Widget::drawMargin( buffer );

    // First we need to set the label at the correct position. To do this
    // we need to take into accout the left padding.
    size_t lpad = getPadding( Direction::Left );
    std::u32string pad_label = u32pad( m_label, 0, lpad );
    buffer.set( pad_label, m_pos_y + 1, m_pos_x + 1, m_label_style, false );

    // Draw the border around the actual input section
    drawBorder( buffer );

    // Draw the actual content
    size_t b_width = (size_t)m_border.getBorderWcwidth();
    size_t x_pos = m_pos_x + 1 + (size_t)u32swidth( pad_label ) + 1 + b_width;
    
    for ( size_t idx = 0; idx < m_content.size(); ++idx )
    {
        const char32_t c_elem = m_content[ idx ];
        const Style& c_style = m_content_style[ idx ];

        if ( c_elem == '\0' ) continue;
        buffer.set( c_elem, m_pos_y + 1, x_pos + idx, c_style, false );
    }
}