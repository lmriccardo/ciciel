add_executable(ccl_Argparser argparser.cpp)
add_executable(ccl_RingBuffer ring_buffer_test.cpp)
add_executable(ccl_Thread thread_test.cpp)
add_executable(ccl_Array2D array2d_test.cpp)

target_link_libraries(ccl_Argparser PRIVATE ccl_Cli)
target_link_libraries(ccl_RingBuffer PRIVATE ccl_DataStructures)
target_link_libraries(ccl_Thread PRIVATE ccl_System)
target_link_libraries(ccl_Array2D PRIVATE ccl_DataStructures)

add_test(NAME ArgparserTest COMMAND ccl_Argparser)
add_test(NAME RingBufferTest COMMAND ccl_RingBuffer)
add_test(NAME ThreadTest COMMAND ccl_Thread)
add_test(NAME Array2D COMMAND ccl_Array2D)