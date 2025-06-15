#pragma once

#include <vector>
#include <string>
#include <cli/ui/elements/widgets/widget.hpp>

namespace ccl::cli::ui
{
    class List : public ContentWidget<std::vector<std::u32string>>
    {
    private:
        std::vector<Style> m_content_style;     // Vector of styles for each element
        TextAlignment      m_content_alignment; // Text alignment global to each element
        size_t             m_max_nof_elems;     // Maximum capacity of the list

        size_t m_curr_idx = 0; // Current index of the inserted element

        using elem_t = std::vector<std::u32string>;

        void drawPadding( ScreenBuffer& ) const;

    public:
        List( const std::string& id, const elem_t& elems, size_t x, 
              size_t y, size_t max_size
        );

        List( const std::string& id, size_t x, size_t y, size_t max_size );
        ~List() = default;

        List& addElement( const std::u32string& element, const Style& style );
        List& addElement( const std::string& element, const Style& style );
        List& addElement( const std::u32string& element );
        List& addElement( const std::string& element );

        void setTextAlignment( TextAlignment value );
        size_t getMaxNofElements() const;

        void draw( ScreenBuffer& buffer ) const override;
    };
}