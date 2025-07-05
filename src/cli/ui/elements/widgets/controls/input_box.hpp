#pragma once

#include <cli/ui/elements/widgets/base/content_widgets/input_content_widget.hpp>

namespace ccl::cli::ui
{
    /**
     * The InputBox widget is a user interface element that allows users to enter and 
     * edit a line of text. It handles character input, cursor movement, and text 
     * rendering, often with support for features like: text editing (insertion, deletion, 
     * backspacing and overwriting), cursor control (arrow nagivation), input constraints
     * (maximum number of characters, or masking for e.g. passwords) and focus management 
     * (indicates whether the input box is currently active and ready to receive input).
     */
    class InputBox : public InputContentWidget
    {
    private:
        Style          m_label_style;             // The style of the additional label
        std::u32string m_label;                   // An additional label description
        char32_t       m_replacement;             // Replacement for the input content
        size_t         m_max_length;              // Maximum number of chars in the content
        bool           m_has_replacement = false; // True if the replacement has been set

        std::pair<size_t,size_t> getStartCursorPosition() const override;
        std::pair<size_t, size_t> getActualContentSize() const override;

        void drawBorder( ScreenBuffer& buffer ) const override;
        void addChar( char ch ) override;
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

        /**
         * Set the replacement value for the input box content.
         * @param value The replacement value
         */
        void setReplacement( char32_t value );

        void draw( ScreenBuffer& buffer ) override;
    };

    /**
     * This function returns an InputBox pointer constructed to be used as
     * password input, i.e., masked user input.
     * 
     * @param id The Id of the widget
     * @param mask The character to be used as masking
     * @param p_length The password length
     */
    std::shared_ptr<InputBox> PasswordInputBox( const std::string& id, 
                                                char32_t mask,
                                                size_t p_length = 16 );
        
    /**
     * Creates an input box for inserting the username.
     * 
     * @param id The ID of the widget
     * @param size Maximum dimension of the username (default 30)
     */
    std::shared_ptr<InputBox> UsernameInputBox( const std::string& id, size_t size = 30 );

    /**
     * Creates an input box for inserting the email.
     * 
     * @param id The ID of the widget
     * @param size Maximum dimension of the email (default 50)
     */
    std::shared_ptr<InputBox> EmailInputBox( const std::string& id, size_t size = 50 );
}