#include <cli/argparse/argument_parser.hpp>

namespace argparse = ccl::cli::argparse;

int main()
{
    argparse::ArgumentParser ap("argparser");

    ap.addArgument             ({"argument1", "a1", "Argument 1", false}, false);
    ap.addArgument<std::string>({"argument2", "a2", "Argument 2", false});
    ap.addArgument             ({"argument3", "", "Argument 3", false}, 100);
    ap.addArgument<std::string>({"argument4", "", "Argument 4", true});
    ap.addArgument             ({"argument5", "a5", "Argument 5", false}, 2.0);

    const char *elements[] = {"argparse-test", "--argument3=200", "-a5", "2.7", "../conf"};

    ap.parse(5, elements);

    int arg3         = ap.getValue<int>("argument3");
    double arg5      = ap.getValue<double>("argument5");
    std::string arg4 = ap.getValue<std::string>("argument4");

    std::cout << "Argument 3: " << arg3 << " " 
              << "Argument 4: " << arg4 << " "
              << "Argument 5: " << arg5 << std::endl;

    ap.clean();

    return 0;
}