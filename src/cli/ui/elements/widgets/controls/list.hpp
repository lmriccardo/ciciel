#pragma once

#include <vector>
#include <string>
#include <cli/ui/elements/widgets/base/content_widgets/vector_content_widget.hpp>

namespace ccl::cli::ui
{
    /**
     * This widget represents a dynamic list of labels. None of the element is 
     * focusable or clickable. It is dynamic since the list can have a variadic 
     * number of elements. However, it is also possible to decide when constructing
     * the maximum number of elements of the list and upon exceeding it acts like
     * a circular buffer.
     */
    class List : public VectorContentWidget<std::u32string, List>
    {
    private:
        TextAlignment m_content_alignment; // Text alignment global to each element
        size_t        m_max_nof_elems;     // Maximum capacity of the list

        void drawPadding( ScreenBuffer& ) const;

    public:
        List( const std::string& id, const content_type& elems, size_t x, size_t y, size_t max_size);
        List( const std::string& id, size_t x, size_t y, size_t max_size );
        List( const std::string& id, const content_type& elems, size_t max_size );
        List( const std::string& id, size_t max_size );

        ~List() = default;

        List& addElement( const std::u32string& element, const Style& style ) override;
        List& addElement( const std::u32string& element ) override;
        List& addElement( const std::string& element, const Style& style );
        List& addElement( const std::string& element );

        void setTextAlignment( TextAlignment value );
        size_t getMaxNofElements() const;

        void draw( ScreenBuffer& buffer ) override;
    };
}