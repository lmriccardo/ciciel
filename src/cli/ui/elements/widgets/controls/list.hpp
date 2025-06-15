#pragma once

#include <vector>
#include <string>
#include <cli/ui/elements/widgets/widget.hpp>

namespace ccl::cli::ui
{
    class List : public ContentWidget<std::vector<std::u32string>>
    {
    private:
        std::vector<Style> m_content_style;
        TextAlignment      m_content_alignment;

        using elem_t = std::vector<std::u32string>;

        void drawPadding( ScreenBuffer& ) const;

    public:
        List( const std::string& id, const elem_t& elems, size_t x, size_t y );
        List( const std::string& id, size_t x, size_t y );
        ~List() = default;

        List& addElement( const std::u32string& element, const Style& style );
        List& addElement( const std::string& element, const Style& style );
        List& addElement( const std::u32string& element );
        List& addElement( const std::string& element );

        void setTextAlignment( TextAlignment value );

        void draw( ScreenBuffer& buffer ) const override;
    };
}