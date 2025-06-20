#pragma once

#include <cli/ui/elements/widgets/base/string_content_widget.hpp>

namespace ccl::cli::ui
{
    /**
     * This is the button widget. It is a StringContentWidget given that the text
     * inside of a button if actually a string.
     */
    class Button : public StringContentWidget
    {
    private:
        bool m_clicked = false; // If the button is clicked or not

    public:
        Button( const std::string& id, const std::u32string& content, size_t x, size_t y );
        Button( const std::string& id, const std::string& content, size_t x, size_t y );
        virtual ~Button() = default;

        void setContent( const std::u32string& text ) override;
        void setClicked( bool value );
        bool isClicked() const;
    };
}