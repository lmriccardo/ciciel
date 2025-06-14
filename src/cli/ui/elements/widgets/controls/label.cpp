#include "label.hpp"

using namespace ccl::cli::ui;

void ccl::cli::ui::Label::drawPadding(ScreenBuffer & buffer) const
{
    size_t rpad = m_padding[2], lpad = m_padding[1];
    size_t tpad = m_padding[0], bpad = m_padding[3];
    size_t maxvpad = ( tpad > bpad ) ? tpad : bpad;

    auto [ cols, rows ] = getContentWinsize();
    std::u32string v_pad_content( cols, U' ' );
    v_pad_content = u32pad( v_pad_content, rpad, lpad );

    Style pad_style = DefaultStyle();

    // Draw the vertical top and bottom padding into the screen Buffer
    for ( size_t p_idx = 0; p_idx < maxvpad; ++p_idx )
    {
        if ( p_idx < tpad )
        {
            buffer.set( v_pad_content, m_pos_y + 1 + p_idx, m_pos_x + 1, pad_style, false );
        }

        if ( p_idx < bpad )
        {
            buffer.set( v_pad_content, m_pos_y + 1 + tpad + 1 + p_idx, m_pos_x + 1, pad_style, false );
        }
    }

    // Draw the horizontal padding into the screen buffer
    std::u32string r_pad_content( rpad, U' ' );
    std::u32string l_pad_content( lpad, U' ' );

    buffer.set( l_pad_content, m_pos_x + 1, m_pos_y + tpad + 1, pad_style, false );
    buffer.set( r_pad_content, m_pos_x + 1 + cols, m_pos_y + tpad + 1, pad_style, false );
}

Label::Label(const std::string &id, const std::u32string &text, size_t x, size_t y)
    : ContentWidget(id, (size_t)u32swidth(text), 1, x, y, true)
{
    setContent( text );

    // Sets preferred size and minimum size
    setMinimumSize( m_winsize.ws_col, m_winsize.ws_row );
}

Label::Label(const std::string &id, const std::string &text, size_t x, size_t y)
    : Label(id, utf8to32(text), x, y)
{
}

void Label::setContentStyle(const Style &style)
{
    m_content_style = style;
}

void Label::setContent(const std::string &content, const Style &style)
{
    std::u32string u32content;
    utf8to32( content, u32content );
    setContent( u32content, style );
}

void Label::setContent(const std::u32string &content, const Style &style)
{
    m_content = content;
    m_content_style = style;
    setContentWinsize( (size_t)u32swidth( m_content ), 1 );
    
    // Sets preferred size and minimum size
    setMinimumSize( m_winsize.ws_col, m_winsize.ws_row );

    // We need to force the parent repacking because the
    // size of the current label has changed
    forceParentRepack();
}

void Label::setContent(const std::string &content)
{
    setContent( content, DefaultStyle() );
}

void Label::setContent(const std::u32string &content)
{
    setContent( content, DefaultStyle() );
}

Style &Label::getContentStyle()
{
    return m_content_style;
}

void Label::draw( ScreenBuffer& buffer ) const
{
    if (!isVisible()) return;
    this->ContentWidget<std::u32string>::draw( buffer ); // Draw the border if visible

    // Get the window size and the actual padding to remove
    auto [ _, w ] = getWinsize();
    size_t rpad = getPadding( Direction::Rigth );
    size_t lpad = getPadding( Direction::Left );
    size_t tpad = getPadding( Direction::Top );

    // Add alignment to the label content
    size_t b_with = static_cast<size_t>( m_border.getBorderWcwidth() );
    std::u32string content = u32align( m_content, 
        m_content_style.m_alignment, 
        w - 2*b_with - (rpad + lpad) );

    // Draw the vertical and horizontal padding into the Screen Buffer
    drawPadding( buffer );

    // Draw the label into the Screen Buffer
    buffer.set( content, m_pos_y + tpad + 1, m_pos_x + lpad + 1, m_content_style, false );
}