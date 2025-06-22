#pragma once

#include <regex>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/concurrent/thread.hpp>
#include <sys/time/times_utils.hpp>
#include <data_structures/buffers/concurrent_ring_buffer.hpp>
#include "keys.hpp"

namespace ccl::cli::ui
{
    enum class InputEventType
    {
        PrintableChar,  // Just simple printable characters
        ControlChar,    // Ctrl-prefixed sequence
        AltKey,         // Alt-prefixed sequence
        FunctionKey,    // Fn keys
        ArrowKey,       // Arrow keys ( up, down, left or rigth )
        NavigationKey,  // Home, End, insert, Delete, Pgup, Pgdown
        MouseEvent,     // Mouse Click
        Unknown         // Unknown event
    };

    enum class MouseButton
    {
        None, Left, Right, Middle, WheelUp, WheelDown
    };

    enum class MouseAction
    {
        None, Press, Release, Move, Drag
    };
    
    /**
     * 
     */
    struct Event
    {
        InputEventType m_type; // The event type
        std::string m_raw;     // The raw content of the event
        double m_timestamp_s;  // The timestamp second
        double m_timestamp_us; // The timestamp microseconds

        // For key events
        char m_key   = 0;     // Actual char if printable of ctrl key
        bool m_ctrl  = false; // CTRL modifier
        bool m_alt   = false; // ALT modifier
        bool m_shift = false; // SHIFT modifier

        // For functional / navigation keys
        int m_keyCode = 0;

        // For Mouse Events
        MouseButton m_mouseButton = MouseButton::None;
        MouseAction m_mouseAction = MouseAction::None;
        int m_mouse_x = -1;
        int m_mouse_y = -1;

        static Event from( const std::string& sequence );
    };

    /**
     * 
     */
    class EventHandler : public sys::concurrent::Thread
    {
    private:
        using thread_type = sys::concurrent::Thread;
        using buffer_type = typename ds::buffers::ConcurrentRingBuffer<std::string>;

        buffer_type m_events; // The buffer of events
        int m_epoll_fd = -1; // File descriptor used for epoll (-1 invalid)
    
    public:
        EventHandler() 
            : thread_type(), m_events(100)
        {
            setCancellationPolicy( 
                sys::concurrent::CancellationPolicy
                               ::AT_CONDITION_CHECK );
        }

        virtual ~EventHandler() = default;

        void run() override;
        void tearDown() override;

        /**
         * Try to pop an event from the event ring buffer.
         */
        bool getEvent(Event& e);
    };
}