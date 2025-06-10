#pragma once

#include <cli/ui/utils/string.hpp>
#include <cli/ui/style/style.hpp>
#include "../widget.hpp"

namespace ccl::cli::ui
{
    class Label : public ContentWidget<std::u32string>
    {
    private:
        Style m_content_style; // The content style

    public:
        Label( const std::string&, const std::u32string&, size_t, size_t );
        Label( const std::string&, const std::string&, size_t, size_t );
        virtual ~Label() = default;

        void setContentStyle( const Style& );
        void setContent     ( const std::string&, const Style& );
        void setContent     ( const std::u32string&, const Style& );
        void setContent     ( const std::string& );
        void setContent     ( const std::u32string& );

        void draw( ScreenBuffer& ) const override;
    };
}