# Simple executable
add_executable( ccl_Ui ui_test.cpp)
target_link_libraries( ccl_Ui PRIVATE ccl_Cli )

add_executable( ccl_UserInput user_input.cpp)
target_link_libraries( ccl_UserInput PRIVATE ccl_Cli )

add_executable( ccl_File io_file_test.cpp)
target_link_libraries( ccl_File PRIVATE ccl_Io )

add_executable( ccl_MapFile mmap_io_file_test.cpp)
target_link_libraries( ccl_MapFile PRIVATE ccl_Io )

# Google Automatic Test
set(UNIT_TEST_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/test/unittest")
include(GoogleTest)

function(create_gtest_test test_name cpp_file)
    add_executable( ${test_name} ${cpp_file})
    target_link_libraries( ${test_name} PRIVATE ${ARGN} GTest::gtest_main )

    set_target_properties( ${test_name} 
        PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY "${UNIT_TEST_OUTPUT_DIR}" )
    
    gtest_discover_tests( ${test_name} 
        TEST_PREFIX "${test_name}." 
        WORKING_DIRECTORY ${UNIT_TEST_OUTPUT_DIR} )
endfunction()

# Create GTest tests
create_gtest_test( ccl_ThreadUnitTest unittest/thread_gtest.cpp ccl_Concurrent )
create_gtest_test( ccl_ArgparserUnitTest unittest/argparser_gtest.cpp ccl_Cli )
create_gtest_test( ccl_SignalSlotUnitTest unittest/signal_and_slot_gtest.cpp ccl_Patterns )
create_gtest_test( ccl_Array2DTest unittest/array2d_gtest.cpp ccl_DataStructures )
create_gtest_test( ccl_DynArray2DTest unittest/dyn_array2d_gtest.cpp ccl_DataStructures )
create_gtest_test( ccl_VecNTest unittest/vecn_gtest.cpp ccl_DataStructures )
create_gtest_test( ccl_RingBufferTest unittest/ring_buffer_gtest.cpp ccl_DataStructures )
create_gtest_test( ccl_ConcurrentRingBufferTest unittest/concurrent_ring_buffer_gtest.cpp ccl_DataStructures )
create_gtest_test( ccl_ByteBufferTest unittest/byte_buffer_gtest.cpp ccl_DataStructures )
create_gtest_test( ccl_CircularQueueTest unittest/circular_queue_gtest.cpp ccl_DataStructures )
