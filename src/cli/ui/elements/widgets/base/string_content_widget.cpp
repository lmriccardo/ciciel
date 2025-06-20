#include "string_content_widget.hpp"

using namespace ccl::cli::ui;

StringContentWidget::StringContentWidget( 
    const std::string& id, const std::u32string& content, size_t x, size_t y
) : ContentWidget( id, (size_t)u32swidth(content), 1, x, y, true )
{
    setContent( content );

    // Sets preferred size and minimum size
    setMinimumSize( m_winsize.ws_col, m_winsize.ws_row );

    // Hide the border
    m_border.Show( false );
}

StringContentWidget::StringContentWidget(
    const std::string &id, const std::string &text, size_t x, size_t y
) : StringContentWidget(id, utf8to32(text), x, y)
{
}

void StringContentWidget::setContent(const std::string &content, const Style &style)
{
    std::u32string u32content;
    utf8to32( content, u32content );
    setContent( u32content, style );
}

void StringContentWidget::setContent(const std::u32string &content, const Style &style)
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

void StringContentWidget::setContent(const std::string &content)
{
    setContent( content, DefaultStyle() );
}

void StringContentWidget::setContent(const std::u32string &content)
{
    setContent( content, DefaultStyle() );
}

void StringContentWidget::draw( ScreenBuffer& buffer )
{
    if (!isVisible()) return;
    this->base_type::draw( buffer ); // Draw the border if visible

    size_t content_w = getContentWinsize().first;
    size_t rpad = getPadding( Direction::Rigth );
    size_t lpad = getPadding( Direction::Left );
    size_t tpad = getPadding( Direction::Top );

    // Align and pad the content
    std::u32string aligned_c = u32align( m_content, 
                                         m_content_style.m_alignment, 
                                         content_w );

    std::u32string padded_c = u32pad( aligned_c, rpad, lpad );

    // Write the content into the buffer
    buffer.set( padded_c, m_pos_y + tpad + 1, m_pos_x + 1, m_content_style, false );
}