#pragma once

#include <cli/ui/elements/containers/vbox_container.hpp>
#include "panel.hpp"

namespace ccl::cli::ui
{
    class VBoxPanel : public PanelWidget<VBoxContainer>
    {
    public:
        using PanelWidget::PanelWidget;
        virtual ~VBoxPanel() = default;

        void prepack() override;
    };
}