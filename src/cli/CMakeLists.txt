add_library(ccl_Cli
    # ARGPARSER SOURCE FILES
    argparse/cli_argument_base.cpp
    argparse/cli_arguments.cpp
    argparse/argument_parser.cpp

    # UI CLI SOURCE FILES
    ui/style/colors.cpp
    ui/style/style.cpp
    ui/utils/string.cpp
    ui/screen/terminal.cpp
    ui/screen/screen_buffer.cpp
    ui/screen/screen.cpp
    ui/elements/widgets/panels/panel_base.cpp
    ui/elements/containers/container.cpp
    ui/elements/containers/hbox_container.cpp
    ui/elements/widgets/widget.cpp
    ui/elements/widgets/controls/label.cpp
    ui/elements/widgets/controls/list.cpp

)

target_link_libraries( ccl_Cli PRIVATE 
    ccl_DataStructures 
    ${CURSES_TARGET_LIBRARIES} 
    utf8proc )


target_include_directories( ccl_Cli 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    PRIVATE ${CURSES_INCLUDE_DIR}
    PUBLIC ${UTF8PROC_INCLUDE_DIR} )