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
    
    ui/elements/widgets/base/widget.cpp
    ui/elements/widgets/base/panel_base.cpp
    ui/elements/widgets/base/string_content_widget.cpp
    ui/elements/widgets/panels/hbox_panel.cpp
    ui/elements/widgets/panels/vbox_panel.cpp
    ui/elements/widgets/controls/label.cpp
    ui/elements/widgets/controls/list.cpp
    ui/elements/widgets/controls/button.cpp

    ui/elements/containers/container.cpp
    ui/elements/containers/flow_container.cpp
    ui/elements/containers/hbox_container.cpp
    ui/elements/containers/vbox_container.cpp

    ui/screen/screen.cpp
    ui/events/event.cpp
    ui/ui_app.cpp
)

target_link_libraries( ccl_Cli PRIVATE 
    ccl_DataStructures
    ccl_Patterns
    ccl_System
    ${CURSES_TARGET_LIBRARIES} 
    utf8proc )


target_include_directories( ccl_Cli 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    PRIVATE ${CURSES_INCLUDE_DIR}
    PUBLIC ${UTF8PROC_INCLUDE_DIR} )