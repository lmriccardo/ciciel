#pragma once

#include <cli/ui/elements/widgets/base/widget.hpp>
#include <cli/ui/screen/screen.hpp>
#include <cli/ui/events/event.hpp>

namespace ccl::cli::ui
{
    class UIApplication
    {
    private:
        void tearDown();

        // Event handlers functions
        void handleKeyPressedEvent( const Event& event );
        void handleMouseEvent( const Event& event );

    protected:
        std::string             m_app_name;       // The application name
        std::unique_ptr<Screen> m_screen;         // The unique screen of the application
        EventHandler            m_event_handler;  // The event handler
        Event                   m_last_event;     // The last occurred event
        Widget*                 m_focused_widget; // The current focused child
        
        bool m_running = false;

        virtual void handleUserDefinedEvents( [[maybe_unused]] const Event& event ) {};
    
    public:
        UIApplication( const std::string& name = "ccl_UI_Application" );
        ~UIApplication() = default;

        /**
         * Returns the Screen reference of the Ui application.
         * @return The Screen object reference
         */
        Screen& getScreen();

        /**
         * The main application loop
         */
        virtual void run();

        /**
         * Stop the main loop and sends a fake signal to the
         * event handler.
         */
        void quit( const std::string& );
    };
}