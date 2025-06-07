#include <cli/ui.hpp>

using namespace ccl::cli::ui;

class A {};

int main()
{
    HBoxPanel p( "Panel1", 10, 10, 0, 0 );
    HBoxPanel p2( "Panel2", 10, 10, 1, 1 );
    HBoxPanel p3( "Panel2", 10, 10, 1, 2 );

    p.addChild( p2 );
    p.addChild( p3 );

    return 0;
}