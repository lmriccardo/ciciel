add_library( ccl_System 
    concurrent/thread.cpp
)

target_include_directories( ccl_System PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )