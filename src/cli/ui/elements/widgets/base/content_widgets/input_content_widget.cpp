#include "input_content_widget.hpp"

using namespace ccl::cli::ui;

void InputContentWidget::onMouseEnterCallback()
{
    // Set the visibility to true once entered the input section
    m_cursor_info.m_hidden = false;

    // Once the mouse has entered the input section we can clear the placeholder
    if ( m_is_placeholder )
    {
        // Clear its content without reducing its size
        std::fill( m_content.begin(), m_content.end(), U'\0' );
        std::fill( m_content_style.begin(), m_content_style.end(), DefaultStyle() );

        // Set the starting cursor position
        const auto [ c_y, c_x ] = getStartCursorPosition();
        m_cursor_info.setPosition( c_x, c_y );
        m_need_clear = true;
        return;
    }
}

void InputContentWidget::onMouseExitCallback()
{
    // Set the visibility to true once exited the input section
    m_cursor_info.m_hidden = true;

    if ( m_is_placeholder )
    {
        setContent( m_placeholder );
    }
}

std::pair<size_t, size_t> InputContentWidget::getStartCursorPosition() const
{
    // By default the starting cursor position is at the relative
    // (0, 0) position of the content window
    return { m_pos_y + 1, m_pos_x + 1 };
}

InputContentWidget::InputContentWidget(
    const std::string &id, const std::u32string &placeholder, size_t x, size_t y
) : base_type(id, (size_t)u32swidth(placeholder), 1, x, y, true),
    m_placeholder( placeholder )
{
    setContent( placeholder );

    // Sets preferred size and minimum size
    setMinimumSize( m_winsize.ws_col, m_winsize.ws_row );

    // Hide the border
    m_border.Show( false );

    // Connect the onMouseEnter and onMouseExit signal to callbacks
    onMouseEnter.connect( [this](){ this->onMouseEnterCallback(); } );
    onMouseExit.connect( [this](){ this->onMouseExitCallback(); } );
}

InputContentWidget::InputContentWidget(
    const std::string &id, const std::string &placeholder, size_t x, size_t y
) : InputContentWidget( id, utf8to32(placeholder), x, y )
{}

InputContentWidget::InputContentWidget(const std::string &id, const std::u32string &placeholder)
    : InputContentWidget( id, placeholder, 0, 0)
{}

InputContentWidget::InputContentWidget(const std::string &id, const std::string &placeholder)
    : InputContentWidget( id, placeholder, 0, 0)
{}

InputContentWidget::InputContentWidget(const std::string &id, size_t x, size_t y)
    : InputContentWidget( id, U"", x, y)
{}

InputContentWidget::InputContentWidget(const std::string &id)
    : InputContentWidget( id, 0, 0 )
{}

void InputContentWidget::setContent( const std::u32string& content )
{
    if ( m_content_style.size() <= content.size() )
    {
        m_content_style.resize( content.size() );
    }

    if ( m_content.size() < content.size() )
    {
        m_content.resize( content.size() );
    }

    // Copy the input content into the content string
    std::copy( content.begin(), content.end(), m_content.begin() );

    Style c_style = DefaultStyle();
    c_style.Italic( true );

    size_t idx = 0;
    std::for_each( m_content.begin(), m_content.end(), 
        [&]( [[maybe_unused]] char32_t _ )
        {
            m_content_style[idx++] = c_style;
        });
}

void InputContentWidget::setContent( const std::string& content )
{
    setContent( utf8to32( content ) );
}