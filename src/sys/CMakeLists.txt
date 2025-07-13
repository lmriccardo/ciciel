add_library( ccl_System 

    # Concurrent module
    concurrent/thread.cpp

    # IO module
    io/base/io_handle.cpp
    io/base/stream_io.cpp
    io/file/file_io.cpp
    io/file/buffered_io.cpp
    io/file/mmap_file_io.cpp

    # Time module
    time/times_utils.cpp
)

target_link_libraries( ccl_System PRIVATE ccl_DataStructures )
target_include_directories( ccl_System PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )