#pragma once

#include <cli/ui/utils/string.hpp>
#include <cli/ui/style/style.hpp>
#include "../widget.hpp"

namespace ccl::cli::ui
{
    /**
     * A Label is a particular content widget that shows a static
     * text that cannot be changed at runtime by the user using
     * user input. It can change as side effects of other operations
     * but does not accept user input. Its width and heigth are
     * decided based on the current text content of the widget. 
     */
    class Label : public ContentWidget<std::u32string>
    {
    private:
        Style m_content_style; // The content style

        void drawPadding( ScreenBuffer& ) const;

    public:
        Label( const std::string& id, const std::u32string& content, size_t x, size_t y );
        Label( const std::string& id, const std::string& content, size_t x, size_t y );
        virtual ~Label() = default;

        void setContentStyle( const Style& style );
        void setContent     ( const std::string& content, const Style& style );
        void setContent     ( const std::u32string& content, const Style& style );
        void setContent     ( const std::string& content );
        void setContent     ( const std::u32string& content );

        Style& getContentStyle();

        void draw( ScreenBuffer& buffer ) const override;
    };
}