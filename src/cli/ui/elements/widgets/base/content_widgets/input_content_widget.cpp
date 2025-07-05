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

void InputContentWidget::handleCtrlPressed(char c)
{
    if ( c == CCL_KEY_CTRL_I )
    {
        m_insert_toggle = !m_insert_toggle;
        return;
    }
}

void InputContentWidget::handleNavKPressed(char c)
{
    if ( c == CCL_KEY_BACKSPACE || c == CCL_KEY_DELETE )
    {
        int input = ( c == CCL_KEY_BACKSPACE ) ? -1 : 0;
        deleteChar( input );
        return;
    }
}

void InputContentWidget::deleteChar( int x_neg_off )
{
    size_t x_size   = getActualContentSize().first;
    size_t start_x  = getStartCursorPosition().second;
    size_t x_relpos = m_cursor_info.m_pos.m_x - start_x;
    int pos2del     = x_relpos + x_neg_off;
    size_t y_pos    = m_cursor_info.m_pos.m_y;

    if ( x_neg_off == -1 )
    {
        // This is the BACKSPACE case
        if ( pos2del < 0 )
        {
            size_t new_y_pos = y_pos - 1;
            size_t new_x_pos = x_size - 1;
            
            if ( !isCursorColliding( new_x_pos - 1, new_y_pos ) )
            {
                return;
            }
            
            // remove
            m_cursor_info.setPosition( new_x_pos, new_y_pos );
            pos2del = new_x_pos;
        }
        
        // Remove the previous character and shift
        // all the remaining one position to the left
        // (handle the new line for TextBox)
        remove_shift( m_content, pos2del, m_curr_length-- );
        m_cursor_info.m_pos.m_x--;
    }
    else
    {
        // This is the CANC case: remove the char from the
        // current position and shift all the remaining
        // one position to the left (handle the new line for
        // multiline input box, i.e., TextBox)
        remove_shift( m_content, pos2del, m_curr_length-- );
    }

    m_need_clear = true;
}

std::pair<size_t, size_t> InputContentWidget::getStartCursorPosition() const
{
    // By default the starting cursor position is at the relative
    // (0, 0) position of the content window
    return { m_pos_y + 1, m_pos_x + 1 };
}

bool InputContentWidget::isCursorColliding(size_t x, size_t y) const
{
    // Check that applying the directions the cursor does not exit
    // the current widget focus window.
    auto [ y_start, x_start ] = getStartCursorPosition();
    auto [ x_size,  y_size  ] = getActualContentSize();
    if (   ( y >= y_start && y < y_start + y_size )
        && ( x >= x_start && x < x_start + x_size )
    ) {
        return true;
    }

    return false;
}

void InputContentWidget::handleArrow(int v_direction, int h_direction)
{
    size_t& y_pos = m_cursor_info.m_pos.m_y;
    size_t& x_pos = m_cursor_info.m_pos.m_x;

    if ( isCursorColliding( x_pos + h_direction, y_pos + v_direction ) )
    {
        y_pos += v_direction;
        x_pos += h_direction;
    }
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
    onCharPressed.connect( [this]( char c ){ this->addChar(c); } );
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