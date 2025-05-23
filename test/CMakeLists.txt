add_executable(ccl_Argparser argparser.cpp)

target_link_libraries(ccl_Argparser PRIVATE ccl_Cli)

add_test(NAME ArgparserTest COMMAND ccl_Argparser)