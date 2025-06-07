#pragma once

#include <iostream>
#include "container.hpp"

namespace ccl::cli::ui
{
    class HBoxContainer : public Container
    {
    public:
        HBoxContainer( size_t, size_t, size_t, size_t );
        virtual ~HBoxContainer() = default;

        void layout() override;
    };
}