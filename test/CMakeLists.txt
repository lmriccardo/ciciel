add_executable(ccl_Argparser argparser.cpp)
add_executable(ccl_RingBuffer ring_buffer_test.cpp)

target_link_libraries(ccl_Argparser PRIVATE ccl_Cli)
target_link_libraries(ccl_RingBuffer PRIVATE ccl_DataStructures)

add_test(NAME ArgparserTest COMMAND ccl_Argparser)
add_test(NAME RingBufferTest COMMAND ccl_RingBuffer)