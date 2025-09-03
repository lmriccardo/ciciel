add_library( ccl_Io

    base/io_handle.cpp
    base/stream_io.cpp
    file/file_io.cpp
    file/buffered_io.cpp
    file/mmap_file_io.cpp 
)

target_link_libraries( ccl_Io PRIVATE ccl_DataStructures )
target_include_directories( ccl_Io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )