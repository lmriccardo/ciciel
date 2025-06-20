add_library( ccl_Patterns INTERFACE
    signals_slot/signals_slot.hpp
)

target_include_directories( ccl_Patterns INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/.. )