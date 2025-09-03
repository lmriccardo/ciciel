add_library( ccl_Logging

    formatting.cpp

)

target_link_libraries( ccl_Logging PRIVATE ccl_Io )
target_include_directories( ccl_Logging PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )