#include <iostream>
#include <cli/ui/style/style.hpp>
#include <cli/ui/screen/screen.hpp>

using namespace ccl::cli::ui;

int main()
{
    std::cout << Colors::Black << std::endl;

    try
    {
        Screen window;
    }
    catch(const std::exception& e)
    {
        Terminal::getInstance().disableRawMode();
        Terminal::getInstance().reset();
        throw std::runtime_error( e.what() );
    }
    

    Style s = Style().Background(Colors::Aqua).Foreground(Colors::Black).Italic(true);

    return 0;
}