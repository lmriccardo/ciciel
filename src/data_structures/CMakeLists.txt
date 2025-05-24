add_library( ccl_DataStructures
    buffers/byte_buffer.cpp
)

target_include_directories( ccl_DataStructures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )