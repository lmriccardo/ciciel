#pragma once

#include <cli/ui/elements/widgets/base/content_widgets/string_content_widget.hpp>

namespace ccl::cli::ui
{
    /**
     * A Label is a particular content widget derived from StringContentWidget
     * class. While in the base class the entire content, i.e., actual text +
     * text style, is static, in this case at least the style can change. This
     * is actually the only difference between this and its super class. 
     */
    class Label : public StringContentWidget
    {
    public:
        using StringContentWidget::StringContentWidget;
        virtual ~Label() = default;

        void setContentStyle( const Style& style );
        Style& getContentStyle();
    };
}