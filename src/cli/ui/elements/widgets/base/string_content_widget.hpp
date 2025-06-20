#pragma once

#include <cli/ui/utils/string.hpp>
#include <cli/ui/style/style.hpp>
#include "widget.hpp"

namespace ccl::cli::ui
{
    /**
     * A String content widget is a derived class of ContentWidget specialized
     * for string content. It uses 32-bit ( 4 bytes ) string in order to easily
     * represents encoded UTF8 and extended ASCII characters. Its content is
     * static meaning that cannot be changed at runtime except when it is
     * completely overwritten.
     */
    class StringContentWidget : public ContentWidget<std::u32string>
    {
    protected:
        Style m_content_style;

        using base_type = ContentWidget<std::u32string>;
    
    public:
        StringContentWidget( const std::string& id, const std::u32string& content, size_t x, size_t y );
        StringContentWidget( const std::string& id, const std::string& content, size_t x, size_t y );

        StringContentWidget( const std::string& id, const std::u32string& content );
        StringContentWidget( const std::string& id, const std::string& content );

        virtual ~StringContentWidget() = default;

        virtual void setContent( const std::string& content, const Style& style );
        virtual void setContent( const std::u32string& content, const Style& style );
        virtual void setContent( const std::string& content );
        virtual void setContent( const std::u32string& content );

        virtual void draw( ScreenBuffer& buffer ) override;
    };
}