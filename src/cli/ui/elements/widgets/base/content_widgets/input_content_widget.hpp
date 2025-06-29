#pragma once

#include <patterns/signals_slot/signals_slot.hpp>
#include <vector>
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

    protected:
        using base_type = ContentWidget<std::u32string>;
        using base_type::drawBorder;
        
        std::vector<Style> m_content_style; // Style for each char in the content
        std::u32string     m_placeholder;   // The placeholder string ( if any )
        
        bool m_is_placeholder = true; // True if the content is still the placeholder

        void setContent( const std::u32string& content );
        void setContent( const std::string& content );

        virtual void addChar( char32_t c, size_t x, size_t y ) = 0;
        virtual void addChar( char c, size_t x, size_t y ) = 0;
        virtual void addChar( char32_t c ) = 0;
        virtual void addChar( char c ) = 0;

        virtual void onResizeCallback() = 0;
        virtual std::pair<size_t, size_t> getStartCursorPosition() const;

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