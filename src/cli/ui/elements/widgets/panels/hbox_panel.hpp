#pragma once

#include <cli/ui/elements/containers/hbox_container.hpp>
#include "panel.hpp"

namespace ccl::cli::ui
{
    class HBoxPanel : public PanelWidget<HBoxContainer>
    {
    public:
        using PanelWidget::PanelWidget;
        virtual ~HBoxPanel() = default;

        void prepack() override;
    };
}