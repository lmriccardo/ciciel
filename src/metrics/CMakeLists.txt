add_library( ccl_Metrics

    metrics_gather.cpp
    metrics_singl.cpp

)

target_include_directories( ccl_Metrics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )