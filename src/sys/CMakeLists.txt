add_library( ccl_System 

    # Concurrent module
    concurrent/thread.cpp

    # IO module
    io/file.cpp

    # Time module
    time/times_utils.cpp
)

target_include_directories( ccl_System PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )