#pragma once

#include <patterns/signals_slot/signals_slot.hpp>
#include <cli/ui/elements/widgets/base/string_content_widget.hpp>

namespace ccl::cli::ui
{
    /**
     * This is the button widget. It is a StringContentWidget given that the text
     * inside of a button if actually a string.
     */
    class Button : public StringContentWidget
    {
    public: // Signals definition
        dp::signals::Signal<const std::string&> onClick; // When button is clicked

    private:
        bool m_clicked = false; // If the button is clicked or not

        void onMouseEnterCallback();
        void onMouseExitCallback();

    public:
        Button( const std::string& id, const std::u32string& content, size_t x, size_t y );
        Button( const std::string& id, const std::string& content, size_t x, size_t y );
        Button( const std::string& id, const std::u32string& content );
        Button( const std::string& id, const std::string& content );
        virtual ~Button() = default;

        void setContent( const std::u32string& text ) override;
        void setClicked( bool value );
        bool isClicked() const;
    };
}