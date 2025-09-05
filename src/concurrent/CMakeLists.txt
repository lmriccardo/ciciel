add_library( ccl_Concurrent

    thread.cpp
    
)

target_include_directories( ccl_Concurrent PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )