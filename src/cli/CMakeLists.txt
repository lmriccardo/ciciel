add_library(ccl_Cli
    # ARGPARSER SOURCE FILES
    argparse/cli_argument_base.cpp
    argparse/cli_arguments.cpp
    argparse/argument_parser.cpp

    # UI CLI SOURCE FILES
    ui/style/colors.cpp
    ui/screen/terminal.cpp
    ui/screen/screen_buffer.cpp
    ui/screen/screen.cpp
)

target_link_libraries( ccl_Cli PRIVATE ccl_DataStructures ${CURSES_TARGET_LIBRARIES} )
target_include_directories( ccl_Cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    PRIVATE ${CURSES_INCLUDE_DIR} )