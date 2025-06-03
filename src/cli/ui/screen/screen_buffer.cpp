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

void ScreenBuffer::set(const std::string &content, size_t s_row, size_t s_col, const Style& style)
{
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
        set({ curr_char, style }, current_idx);
        m_lastUpdate[current_idx] = m_updateCounter + 1;
        current_idx++;
    }
}

void ScreenBuffer::set(const char32_t& content, size_t s_row, size_t s_col, const Style& style)
{
    size_t flatten_idx = flatten( s_row, s_col );
    set( { content, style }, flatten_idx );
    m_lastUpdate[ flatten_idx ] = m_updateCounter + 1;
}

void ScreenBuffer::flush(Terminal &t_out) const
{
    for ( const auto& pair: m_lastUpdate )
    {
        size_t pos  = pair.first;
        int counter = pair.second;

        if ( counter <= m_updateCounter ) continue;

        struct CellChar buffer_element = at( pos );
        t_out.put( buffer_element.m_char, buffer_element.m_style );
    }
}
