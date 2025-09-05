#pragma once

#include <patterns/signals_slot/signals_slot.hpp>
#include <cli/ui/elements/widgets/base/content_widgets/string_content_widget.hpp>

namespace ccl::cli::ui
{
    /**
     * This is the button widget. It can be clicked. That's it ...
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