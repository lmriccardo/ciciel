#pragma once

#include <cli/ui/elements/widgets/base/content_widgets/input_content_widget.hpp>

namespace ccl::cli::ui
{
    /**
     * 
     */
    class InputBox : public InputContentWidget
    {
    private:
        Style          m_label_style; // The style of the additional label
        std::u32string m_label;       // An additional label description
        char32_t       m_replacement; // Replacement for the input content
        size_t         m_max_length;  // Maximum number of chars in the content

        std::pair<size_t,size_t> getStartCursorPosition() const override;

        void drawBorder( ScreenBuffer& buffer ) const override;

        void addChar( char32_t c, size_t x, size_t y ) override {};
        void addChar( char c, size_t x, size_t y ) override {};
        void addChar( char32_t c ) override {};
        void addChar( char c ) override {};

        void onResizeCallback() override {};

    public:
        using InputContentWidget::InputContentWidget;
        virtual ~InputBox() = default;

        /**
         * Sets the new maximum content size if it actual grater
         * then or equalt to the current minimum content winsize.
         * 
         * @param value The new content maximum size
         */
        void setMaxLength( size_t value );

        /**
         * Sets the label to the input box
         */
        void setLabel( const std::u32string& label, const Style& style );
        void setLabel( const std::string& label, const Style& style );
        void setLabel( const std::u32string& label );
        void setLabel( const std::string& label );

        void draw( ScreenBuffer& buffer ) override;
    };
}