#include "screen_buffer.hpp"

using namespace ccl::cli::ui;

ScreenBuffer::ScreenBuffer(size_t height, size_t width )
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

void ScreenBuffer::set(const std::string &content, 
                       size_t s_row, 
                       size_t s_col, 
                       const Style& style, 
                       bool redraw
) {
    size_t flatten_idx = flatten( s_row, s_col );
    size_t remaining_size = content.size();

    // Check if the content fit into the remaining space
    if ( (size() - flatten_idx) < content.size() )
    {
        std::cerr << "[Warning] "
                  << "Content does not fit into remaining screen space " 
                  << std::endl;

        remaining_size = size() - flatten_idx;
    }

    auto content_it = content.begin();
    auto content_end = content_it + remaining_size;
    size_t current_idx = flatten_idx;

    for ( ; content_it != content_end; ++content_it )
    {
        char32_t curr_char = *content_it;
        set(curr_char, current_idx, style, redraw);
        current_idx++;
    }
}

void ScreenBuffer::set(char content, size_t s_row, size_t s_col, const Style& style, bool redraw)
{
    size_t flatten_idx = flatten( s_row, s_col );
    set( content, flatten_idx, style, redraw );
}

void ScreenBuffer::set(char content, size_t pos, const Style &style, bool redraw)
{
    const auto& cell = at( pos );

    // Check that the current cell is different from the new content
    if ( (cell.m_char != content && cell.m_style != style) || redraw )
    {
        set( { content, style, false }, pos );
        m_lastUpdate[ pos ] = m_updateCounter + 1;
    }
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
