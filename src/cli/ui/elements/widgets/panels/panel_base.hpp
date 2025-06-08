#pragma once

#include <unordered_map>
#include <cli/ui/elements/widgets/widget.hpp>

namespace ccl::cli::ui
{
    /**
     * Base class for all panels. It is used only for pointer
     * casting and downcasting.
     */
    class PanelBase : public Widget
    {
    protected:
        std::unordered_map<std::string, Widget*> m_childIds;

    public:
        using Widget::Widget;
        virtual ~PanelBase() = default;

        virtual void addChild( Widget& ) = 0;
        virtual void pack() = 0;
    };
};