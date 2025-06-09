#include "screen_buffer.hpp"

using namespace ccl::cli::ui;

ScreenBuffer::ScreenBuffer( size_t width, size_t height )
    : DynamicArray2D( height, width ), m_updateCounter( 0 )
{
    for ( size_t i = 0; i < size(); ++i )
    {
        m_lastUpdate.insert( { i, 0 } );
    }
}

size_t ScreenBuffer::getWidth() const
{
    return size( static_cast<size_t>( ds::Selector2D::COLUMNS ) );
}

size_t ScreenBuffer::getHeight() const
{
    return size( static_cast<size_t>( ds::Selector2D::ROWS ) );
}

void ScreenBuffer::resize(size_t new_h, size_t new_w)
{
    if ( new_h == getHeight() ) new_h = 0;
    if ( new_w == getWidth() ) new_w = 0;
    extend( new_h, new_w );
}

size_t ScreenBuffer::set(const std::string &content, size_t s_row, size_t s_col, 
    const Style& style, bool redraw) 
{
    std::u32string u32_content;
    if ( utf8to32( content, u32_content ) < 0 ) return 0;
    return set( u32_content, s_row, s_col, style, redraw );
}

size_t ScreenBuffer::set(char32_t content, size_t s_row, size_t s_col, const Style& style, bool redraw)
{
    size_t flatten_idx = flatten( s_row, s_col );
    return set( content, flatten_idx, style, redraw );
}

size_t ScreenBuffer::set(char32_t content, size_t pos, const Style &style, bool redraw)
{
    int content_wc;

    if ( (content_wc = charwidth( &content )) == 0 )
    {
        return 0;
    }

    // Check that the content is inside buffer bound
    if ( pos + content_wc - 1 >= size() ) return 0;

    const auto& cell = at( pos );

    // Check that the current cell is different from the new content
    if ( (cell.m_char != content && cell.m_style != style) || redraw )
    {
        set( { content, style, false }, pos );
        m_lastUpdate[ pos ] = m_updateCounter + 1;

        for ( size_t off_i = 1; off_i < (size_t)content_wc; ++off_i )
        {
            set( { U'\000', style, false }, pos + off_i );
        }
    }

    return content_wc;
}

size_t ScreenBuffer::set(const std::u32string &content, size_t s_row, size_t s_col, 
    const Style & style, bool redraw)
{
    size_t content_size_cols = u32swidth( content );

    // Check that emojis and other type of UTF8 encoded are supported
    if (content_size_cols < 1)
    {
        std::cerr << "Detected some unicode characters that are not supported. "
                  << "Consider running `setlocale(LC_ALL, '')`"
                  << std::endl;
        
        return 0;
    }

    size_t flatten_idx = flatten( s_row, s_col );

    if ( (size() - flatten_idx) < content_size_cols )
    {
        std::cerr << "[Warning] "
                  << "Content does not fit into remaining screen space " 
                  << std::endl;
    }

    auto content_it = content.begin();
    size_t current_idx = flatten_idx;

    for ( ; content_it != content.end(); ++content_it )
    {
        // Check if exceeds the maximum buffer dimension
        if ( current_idx > size() ) break;

        char32_t curr_char = *content_it;
        current_idx += set( curr_char, current_idx, style, redraw );
    }

    return current_idx;
}

void ScreenBuffer::flush(Terminal &t_out)
{
    auto this_it = begin();

    for ( ; this_it != end(); ++this_it )
    {
        size_t current_pos = this_it.pos();
        int curr_element_counter = m_lastUpdate[current_pos];

        // Put the element only if it has been changed from the
        // previous state of the screen buffer.
        if ( curr_element_counter > m_updateCounter )
        {
            const struct CellChar& elem = *this_it;
            size_t row_idx, col_idx;
            getRowCol( row_idx, col_idx, current_pos );
            t_out.put( elem.m_char, row_idx, col_idx, elem.m_style );
        }
    }

    m_updateCounter++;
}
