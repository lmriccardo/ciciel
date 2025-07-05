#pragma once

#include <vector>
#include <patterns/signals_slot/signals_slot.hpp>
#include <cli/ui/events/keys.hpp>
#include "../widget.hpp"

namespace ccl::cli::ui
{
    /**
     * 
     */
    class InputContentWidget : public ContentWidget<std::u32string>
    {
    private:
        void onMouseEnterCallback();
        void onMouseExitCallback();

        void handleCtrlPressed( char c ) override;
        void handleNavKPressed( char c ) override;

    protected:
        using base_type = ContentWidget<std::u32string>;
        using base_type::drawBorder;
        
        std::vector<Style> m_content_style; // Style for each char in the content
        std::u32string     m_placeholder;   // The placeholder string ( if any )
        
        bool   m_insert_toggle  = false; // Enable/Disable insert mode
        bool   m_is_placeholder = true;  // True if the content is still the placeholder
        size_t m_curr_length    = 0;     // The actual number of valid char in the string

        void setContent( const std::u32string& content );
        void setContent( const std::string& content );

        virtual void addChar( char ch ) = 0;
        virtual void onResizeCallback() = 0;

        void deleteChar( int x_neg_off );

        /**
         * This function should returns the actual dimension where the text
         * should be written. Notice that it can differs from the content
         * window size, which instead points to the section of the window
         * that does not comprehend border and margin.
         */
        virtual std::pair<size_t, size_t> getActualContentSize() const = 0;

        /**
         * Returns the starting position where the text should start to be
         * written. This is not the start x and y coordinates from which
         * the actual widget window starts to be drawn. 
         */
        virtual std::pair<size_t, size_t> getStartCursorPosition() const;

        /**
         * This function is used when arrows keys are pressed to check whether
         * it is possible to set the cursor in the new position. Therefore, the 
         * role of this function differ from the isColliding method, which
         * instead checks if the mouse click position collides with the widget
         * window.
         */
        bool isCursorColliding( size_t x, size_t y ) const;
        void handleArrow( int v_direction, int h_direction ) override;

    public:
        InputContentWidget( const std::string& id, const std::u32string& placeholder, size_t x, size_t y );
        InputContentWidget( const std::string& id, const std::string& placeholder, size_t x, size_t y );
        
        InputContentWidget( const std::string& id, const std::u32string& placeholder );
        InputContentWidget( const std::string& id, const std::string& placeholder );

        InputContentWidget( const std::string& id, size_t x, size_t y );
        InputContentWidget( const std::string& id );

        virtual ~InputContentWidget() = default;

        virtual void draw( ScreenBuffer& buffer ) = 0;
    };
}