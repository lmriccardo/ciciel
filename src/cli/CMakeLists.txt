add_library(ccl_Cli
    # ARGPARSER SOURCE FILES
    argparse/cli_argument_base.cpp
    argparse/cli_arguments.cpp
    argparse/argument_parser.cpp

    # UI CLI SOURCE FILES
    ui/screen/terminal.cpp
)

target_include_directories( ccl_Cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )