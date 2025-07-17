function(create_test exe_name cpp_file test_name)
    add_executable( ${exe_name} ${cpp_file})
    target_link_libraries( ${exe_name} PRIVATE ${ARGN} )
    add_test( NAME ${test_name} COMMAND ${exe_name} )
endfunction()

create_test( ccl_Argparser argparser.cpp ArgparserTest ccl_Cli )
create_test( ccl_RingBuffer ring_buffer_test.cpp RingBufferTest ccl_DataStructures )
create_test( ccl_Array2D array2d_test.cpp Array2DTest ccl_DataStructures )
create_test( ccl_DynArray2D dyn_array2d_test.cpp DynArray2DTest ccl_DataStructures )
create_test( ccl_Screen screen_test.cpp ScreenTest ccl_Cli )
create_test( ccl_Vec2 vec2_test.cpp Vec2Test ccl_DataStructures )
create_test( ccl_Color color_test.cpp ColorTest ccl_Cli )
create_test( ccl_Ui ui_test.cpp UiTest ccl_Cli )
create_test( ccl_Signal signal_test.cpp SignalTest ccl_Patterns )
create_test( ccl_UserInput user_input.cpp UserInputTest ccl_Cli )
create_test( ccl_File io_file_test.cpp IoFileTest ccl_System )
create_test( ccl_MapFile mmap_io_file_test.cpp MapIoFileTest ccl_System )

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
create_gtest_test( ccl_ThreadUnitTest thread_gtest.cpp ccl_System )
