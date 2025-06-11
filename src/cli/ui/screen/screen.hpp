#pragma once

#include <memory>
#include <cstring>
#include <cli/ui/elements/widgets/panels/panel_base.hpp>
#include <cli/ui/elements/widgets/panels/panel.hpp>
#include "terminal.hpp"
#include "screen_buffer.hpp"

namespace ccl::cli::ui
{
    struct CursorInfo
    {
        const CursorStyle m_default_style = CursorStyle::SteadyBlock;

        CursorStyle m_style  = m_default_style;
        bool        m_hidden = false;
        size_t      m_xpos   = 0;
        size_t      m_ypos   = 0;
    };

    /**
     * Defines all different layouts for the screen
     */
    enum class Layout
    {
        GridLayout,       // Elements are placed in a grid manner
        HorizontalLayout, // Elements are placed horizontally
        VerticalLayout,   // Elements are placed vertically
        AbsoluteLayout    // Where elements are placed is left to the user
    };

    class Screen
    {
    private:
        std::unique_ptr<PanelBase>    m_panel;    // The main panel of the screen
        std::unique_ptr<ScreenBuffer> m_buffer;   // Controls the buffer of chars
        Terminal&                     m_terminal; // Controls low-level data for the terminal
        Layout                        m_layout;   // The layout of the main panel of the screen
        CursorInfo                    m_cinfo;    // Informations about the current cursor
        struct winsize                m_winsize;  // Window size information

        static int _id; // Screen unique identifier for preventing additional instances

        template <typename PanelT>
        std::unique_ptr<PanelBase> createPanel() const;

        /**
         * This function is called before any throw and in the distructor
         */
        void fallBack();
        void layoutSelection( Layout );

    public:
        Screen(Layout layout = Layout::AbsoluteLayout);
        ~Screen();

        /**
         * Sets the cursor style for the application. 
         * @param style The cursor style identifier
         */
        void setCursorStyle( CursorStyle );

        /**
         * Move the cursor to the given position, if valid, i.e., inside
         * range bounds given by the current window size.
         * 
         * @param x_pos Position on the X axis
         * @param y_pos Position on the Y axis
         */
        void setCursorPosition( int, int );

        /**
         * Set the layout of the main panel of the screen.
         * @param layout The chosen layout
         */
        void setLayout( Layout );
        const Layout& getLayout() const;
        
        /**
         * Move the cursor of the given offset.
         * 
         * @param x_off X axis Offset
         * @param y_off Y axis Offset
         */
        void moveCursor( int, int );

        /**
         * Clear the screen.
         */
        void clear();

        /**
         * Adds a widget to the main panel of the screen
         * @param widget The widget lvalue reference
         */
        void addWidget( Widget& );

        /**
         * Draws the content of all widgets previously added to the
         * screen main panel widget.
         */
        void draw();
    };

    template <typename PanelT>
    inline std::unique_ptr<PanelBase> Screen::createPanel() const
    {
        auto panel_ptr = std::make_unique<PanelT>( "MainPanel",
            m_winsize.ws_col, m_winsize.ws_row, 0, 0 );

        return panel_ptr;
    }
}