#pragma once

#include <cli/ui/elements/widgets/base/widget.hpp>
#include <cli/ui/screen/screen.hpp>
#include <cli/ui/events/event.hpp>

namespace ccl::cli::ui
{
    class UIApplication
    {
    private:
        std::string m_app_name; // The application name
        std::unique_ptr<Screen> m_screen; // The unique screen of the application
        EventHandler m_event_handler; // The event handler

        void tearDown();
    
    public:
        UIApplication( const std::string& name = "ccl_UI_Application" );
        ~UIApplication() = default;

        /**
         * Returns the Screen reference of the Ui application.
         * @return The Screen object reference
         */
        Screen& getContentPanel();

        /**
         * The main application loop
         */
        virtual void run();
    };
}