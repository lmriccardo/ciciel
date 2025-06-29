#include "ui_app.hpp"

using namespace ccl::cli::ui;

UIApplication::UIApplication(const std::string &name)
    : m_app_name( name ), 
      m_screen( std::make_unique<Screen>( Layout::HorizontalLayout ) )
{
    m_focused_widget = nullptr;
}

Screen &UIApplication::getScreen()
{
    return *m_screen;
}

void UIApplication::tearDown()
{
    // Stop the event handler
    m_event_handler.sendInterrupt();
    m_event_handler.stop();

    // Tear down operations
    Terminal::getInstance().reset();
    m_screen->clear();
    m_screen->setCursorPosition( 0, 0 );
    m_screen->setCursorVisibility( true );
}

void UIApplication::handleKeyPressedEvent(const Event &event)
{
}

void UIApplication::handleMouseEvent(const Event &event)
{
    if ( ! ( event.m_type == InputEventType::MouseEvent ) ) return;

    MouseButton mb = event.m_mouseButton;
    MouseAction ma = event.m_mouseAction;
    int mouse_x    = event.m_mouse_x;
    int mouse_y    = event.m_mouse_y;

    // These actions referred to button 1 ( Mouse button left ) being clicked
    if ( mb == MouseButton::Left )
    {
        if (    m_focused_widget == nullptr 
            || !m_focused_widget->isColliding( mouse_x, mouse_y)
        ) {
            // If it is clickable it does not have focus, therefore we should
            // not be worried that onClick is emitted twice. It should not have the focus
            // since for each mouse button press it follows a mouse button release.
            // The only case in which this does not happen, happens when the mouse is
            // being dragged while btn1 is still pressed. In this case is safe to remove
            // the focus previously taken. 
            if ( m_focused_widget != nullptr && m_focused_widget->hasFocus() )
            {
                m_focused_widget->setFocus( false );
            }

            m_focused_widget = m_screen->getCollidingWidget( mouse_x, mouse_y );
        }

        if ( m_focused_widget == nullptr ) return; // Get widget can also returns nullptr

        // Check if the widget is a clickable one
        if ( m_focused_widget->isClickable() )
        {
            m_focused_widget->setFocus( ma == MouseAction::Press );
            return;
        }

        m_focused_widget->setFocus( true );
    }
}

void UIApplication::run()
{
    // Start the event handler thread. Stop if early exit
    m_event_handler.start();
    m_screen->setCursorVisibility( false );
    m_running = true;

    // Initialize the error string
    const char* error_str = nullptr;

    while ( m_running )
    {
        // Check if the event handler is still alive. If no,
        // break the loop and exit the application
        if ( m_event_handler.isCancelled() )
        {
            break;
        }

        try
        {
            // Refresh the screen content
            m_screen->draw();

            // Now we need to set the cursor position
            if ( m_focused_widget != nullptr )
            {
                const auto& c_info = m_focused_widget->getCursorInfo();
                const auto& c_pos  = c_info.m_pos;
                m_screen->setCursorVisibility( !c_info.m_hidden );
                m_screen->setCursorPosition( c_pos.m_x, c_pos.m_y );
            }
            else
            {
                // If nullptr the default cursor visibility is false
                m_screen->setCursorVisibility( false );
            }
        }
        catch ( const std::exception& e )
        {
            error_str = e.what();
            break;
        }

        // Get the escape sequence or normal sequence of chars of the
        // last input event received by the handler.
        if ( !m_event_handler.getEvent(m_last_event) ) continue;

        // Here we should get the actual event type
        if ( m_last_event.m_type == InputEventType::ControlChar )
        {
            if ( m_last_event.m_key == CCL_KEY_CTRL_C )
            {
                m_running = false;
                break;
            }
        }

        // Perform operations based on the event
        handleKeyPressedEvent( m_last_event );
        handleMouseEvent( m_last_event );
        handleUserDefinedEvents( m_last_event ); 
    }

    tearDown();

    if ( error_str != nullptr )
    {
        std::cerr << "Application exited with error: "
                  << error_str
                  << std::endl;
    }
    else
    {
        std::cout << "Application exited successfully" << std::endl;
    }
}

void UIApplication::quit( [[maybe_unused]] const std::string& )
{
    m_event_handler.sendInterrupt();
    m_running = false;
}
