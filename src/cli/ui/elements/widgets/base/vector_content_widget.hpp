#pragma once

#include <vector>
#include <cli/ui/utils/string.hpp>
#include <cli/ui/style/style.hpp>
#include "widget.hpp"

namespace ccl::cli::ui
{
    template <typename T, typename Derived> // Curiously Recurring Template Pattern
    class VectorContentWidget : public ContentWidget<std::vector<T>>
    {
    protected:
        std::vector<Style> m_content_style;     // Vector of styles for each element

        size_t m_curr_idx = 0; // Current index of the inserted element

        using base_type = ContentWidget<std::vector<T>>;
        using content_type = std::vector<T>;

    public:
        VectorContentWidget( const std::string& id, const content_type& elems, size_t x, size_t y );
        VectorContentWidget( const std::string& id, size_t x, size_t y );
        VectorContentWidget( const std::string& id, const content_type& elems );
        VectorContentWidget( const std::string& id );

        virtual ~VectorContentWidget() = default;

        virtual Derived& addElement( const T& element, const Style& style ) = 0;
        virtual Derived& addElement( const T& element ) = 0;
    };

    template <typename T, typename Derived>
    inline VectorContentWidget<T, Derived>::VectorContentWidget(
        const std::string &id, const content_type &elems, size_t x, size_t y
    ) : base_type( id, 1, 1, x, y, true )
    {
        m_content_style.resize( elems.size() );
        this->m_content = elems;
        m_curr_idx = elems.size() - 1;
    }

    template <typename T, typename Derived>
    inline VectorContentWidget<T, Derived>::VectorContentWidget(
        const std::string &id, size_t x, size_t y
    ) : VectorContentWidget( id, {}, x, y ) {}

    template <typename T, typename Derived>
    inline VectorContentWidget<T, Derived>::VectorContentWidget(
        const std::string &id, const content_type &elems
    ) : VectorContentWidget( id, elems, 0, 0 ) {}

    template <typename T, typename Derived>
    inline VectorContentWidget<T, Derived>::VectorContentWidget(const std::string &id)
        : VectorContentWidget(id, 0, 0) {};
}