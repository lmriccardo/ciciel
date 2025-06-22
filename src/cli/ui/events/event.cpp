#include "event.hpp"

using namespace ccl::cli::ui;

void EventHandler::run()
{
    if ( ( m_epoll_fd = epoll_create1(0) ) < 0 )
    {
        std::cerr << std::strerror( errno ) << std::endl;
        m_cancelled.store(true);
        return;
    }

    struct epoll_event event;
    event.events = EPOLLIN; // ready to read
    event.data.fd = STDIN_FILENO; // We need to stdin

    if ( epoll_ctl( m_epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event ) < 0 )
    {
        std::cerr << std::strerror( errno ) << std::endl;
        m_cancelled.store(true);
        return;
    }

    struct epoll_event event_;
    int n_events;

    while (!isCancelled())
    {
        if ( ( n_events = epoll_wait( m_epoll_fd, &event_, 1, -1 ) ) < 0 )
        {
            std::cerr << std::strerror( errno ) << std::endl;
            m_cancelled.store(true);
            break;
        }

        for ( int i = 0; i < n_events; ++i )
        {
            if ( event_.data.fd == STDIN_FILENO )
            {
                char buff[100];
                int r = read( STDIN_FILENO, buff, sizeof(buff) - 1 );
                if ( r <= 0 ) continue;
                buff[r] = 0;

                m_events.put( std::string(buff) );
            }
        }
    }
}

void EventHandler::tearDown()
{
    if ( m_epoll_fd >= 0 )
    {
        close( m_epoll_fd );
    }
}

bool EventHandler::getEvent(Event& e)
{
    std::string event_sequence;
    
    if ( m_events.tryPopFront( event_sequence ) )
    {
        e = std::move( Event::from( event_sequence ) );
        return true;
    }

    return false;
}

Event Event::from(const std::string &sequence)
{
    Event e; // Initialize the event

    // For an empty sequence we can return an unknown event
    if ( sequence.empty() )
    {
        e.m_type = InputEventType::Unknown;
        return e;
    } 

    // Get the current ntp time in second and microseconds and copy the
    // input sequence into the resulting Event object
    sys::time::getNtpTime( e.m_timestamp_s, e.m_timestamp_us );
    e.m_raw = sequence;

    // Single-bytes sequence often refers to printable characters or
    // ctrl-modified characters, but still chars
    if ( sequence.size() == 1 && sequence[0] != CCL_KEY_ESCAPE )
    {
        e.m_key = sequence[0];
        if ( e.m_key >= CCL_KEY_SPACE && e.m_key <= CCL_KEY_TILDE )
        {
            e.m_type = InputEventType::PrintableChar;
            e.m_shift = e.m_key >= CCL_KEY_SHIFT_A && e.m_key <= CCL_KEY_SHIFT_Z;
        }
        else if ( e.m_key == CCL_KEY_ENTER || e.m_key == CCL_KEY_BACKSPACE )
        {
            e.m_type = InputEventType::NavigationKey;
            e.m_keyCode = e.m_key;
        }
        else
        {
            e.m_type = InputEventType::ControlChar;
            e.m_ctrl = true;
        }

        return e;
    }

    // If the first byte identifies the escape sequence char then
    // it is possible that this event releates to a complex event
    // or a terminal capability
    if ( sequence[0] == CCL_KEY_ESCAPE )
    {
        // Check for the ALT modifier
        if (   sequence.size() == 2 
            && sequence[1] >= CCL_KEY_SPACE && sequence[1] <= CCL_KEY_TILDE
        ) {
            e.m_type = InputEventType::AltKey;
            e.m_key  = sequence[1];
            e.m_alt  = true;
            return e;
        }

        // Check for key up, down, left or right
        if (   sequence == CCL_ESCAPE_KEY_UP   || sequence == CCL_ESCAPE_KEY_DOWN
            || sequence == CCL_ESCAPE_KEY_LEFT || sequence == CCL_ESCAPE_KEY_RIGHT
        ) {
            e.m_type    = InputEventType::ArrowKey;
            e.m_keyCode = sequence[2] & 0x1F;
            return e;
        }

        // Check if it is a functional key
        for ( size_t i = 1; i < 13; ++i )
        {
            if ( sequence == CCL_ESCAPE_KEY_FN( i ) )
            {
                e.m_type    = InputEventType::FunctionKey;
                e.m_keyCode = CCL_KEY_FN( i );
                return e;
            }
        }

        // Check for navigation key
        if (    sequence == CCL_ESCAPE_KEY_INSERT || sequence == CCL_ESCAPE_KEY_DELETE
             || sequence == CCL_ESCAPE_KEY_HOME   || sequence == CCL_ESCAPE_KEY_END
             || sequence == CCL_ESCAPE_KEY_PG_UP  || sequence == CCL_ESCAPE_KEY_PG_DOWN 
        ) {
            e.m_type = InputEventType::NavigationKey;
            e.m_keyCode = sequence[2];
            return e;
        }

        // Check for mouse actions
        if ( sequence.size() > 4 && sequence.substr( 0, 3 ) == CCL_ESCAPE_MOUSE_ENTER )
        {
            e.m_type = InputEventType::MouseEvent;

            std::smatch match;
            std::regex mouse_re( R"(\x1B\[<(\d+);(\d+);(\d+)([mM]))" );

            if ( std::regex_match( sequence, match, mouse_re ) )
            {
                int button = std::stoi(match[1]);
                int x = std::stoi(match[2]);
                int y = std::stoi(match[3]);
                char action = match[4].str()[0];

                e.m_mouse_x = x - 1;
                e.m_mouse_y = y - 1;
                e.m_mouseButton = static_cast<MouseButton>(button & 0x03);
                e.m_mouseAction = (action == 'M') ? 
                      MouseAction::Press 
                    : MouseAction::Release;

                return e;
            }

            e.m_mouseAction = MouseAction::None;
            return e;
        }
    }

    e.m_type = InputEventType::Unknown;
    return e;
}
