#pragma once

#include <iostream>
#include "container.hpp"

namespace ccl::cli::ui
{
    class HBoxContainer : public Container
    {
    public:
        using Container::Container;
        virtual ~HBoxContainer() = default;

        void layout() override;
    };
}