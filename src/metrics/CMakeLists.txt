add_library( ccl_Metrics

    metrics_utils.cpp
    metrics_gather.cpp
    metrics_stack.cpp

)

target_include_directories( ccl_Metrics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )
target_link_libraries( ccl_Metrics PRIVATE ccl_DataStructures ccl_Concurrent )