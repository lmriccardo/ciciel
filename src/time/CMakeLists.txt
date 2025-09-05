add_library( ccl_Time

    times_utils.cpp
    
)

target_include_directories( ccl_Time PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )