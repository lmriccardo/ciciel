function(create_test exe_name cpp_file test_name)
    add_executable( ${exe_name} ${cpp_file})
    target_link_libraries( ${exe_name} PRIVATE ${ARGN} )
    add_test( NAME ${test_name} COMMAND ${exe_name} )
endfunction()

create_test( ccl_Argparser argparser.cpp ArgparserTest ccl_Cli )
create_test( ccl_RingBuffer ring_buffer_test.cpp RingBufferTest ccl_DataStructures )
create_test( ccl_Thread thread_test.cpp ThreadTest ccl_System )
create_test( ccl_Array2D array2d_test.cpp Array2DTest ccl_DataStructures )
create_test( ccl_DynArray2D dyn_array2d_test.cpp DynArray2DTest ccl_DataStructures )
create_test( ccl_ScreenBuffer screen_buffer_test.cpp ScreenBufferTest ccl_Cli )
create_test( ccl_Vec2 vec2_test.cpp Vec2Test ccl_DataStructures )
create_test( ccl_Color color_test.cpp ColorTest ccl_Cli )