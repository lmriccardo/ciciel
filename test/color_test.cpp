#include <iostream>
#include <cli/ui/style/style.hpp>
#include <cli/ui/screen/screen.hpp>

using namespace ccl::cli::ui;

int main()
{
    std::cout << Colors::Black << std::endl;

    Screen window;

    Style s = Style().Background(Colors::Aqua).Foreground(Colors::Black).Italic(true);

    return 0;
}