#include "hbox_container.hpp"

using namespace ccl::cli::ui;

HBoxContainer::HBoxContainer(size_t w, size_t h, size_t x, size_t y)
    : Container( w, h, x, y )
{
}

void HBoxContainer::layout()
{
    std::cout << "called layout on HBoxContainer" << std::endl;
}
