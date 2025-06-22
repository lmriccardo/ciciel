#include "ui_app.hpp"

using namespace ccl::cli::ui;

UIApplication::UIApplication(const std::string &name)
    : m_app_name( name ), 
      m_screen( std::make_unique<Screen>( Layout::HorizontalLayout ) )
{}

Screen &UIApplication::getContentPanel()
{
    return *m_screen;
}

void UIApplication::tearDown()
{
    // Tear down operations
    Terminal::getInstance().reset();
    m_screen->clear();
    m_screen->setCursorPosition( 0, 0 );
    std::cout << "CTRL+C Pressed: Application Exiting" << std::endl;
}

void UIApplication::run()
{
    // Start the event handler thread. Stop if early exit
    m_event_handler.start();

    while ( true )
    {
        // Check if the event handler is still alive. If no,
        // break the loop and exit the application
        if ( m_event_handler.isCancelled() )
        {
            break;
        }

        // Refresh the screen content
        m_screen->draw();

        // Get the escape sequence or normal sequence of chars of the
        // last input event received by the handler.
        Event curr_event;
        if ( !m_event_handler.getEvent(curr_event) ) continue;

        // Here we should get the actual event type
        if ( curr_event.m_type == InputEventType::ControlChar )
        {
            if ( curr_event.m_key == CCL_KEY_CTRL_C )
            {
                m_event_handler.stop();
                break;
            }
        }

        // Perform operations based on the event
    }

    tearDown();
}
