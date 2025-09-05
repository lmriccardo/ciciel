add_library( ccl_Patterns
    # Signal and Slot Design Pattern
    signals_slot/signals_slot.hpp

    # PubSub Design Pattern
    pub_sub/subscriber.cpp
    pub_sub/publisher.cpp
    pub_sub/broker.cpp
)

target_include_directories( ccl_Patterns INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/.. )
target_link_libraries( ccl_Patterns PRIVATE ccl_Concurrent ccl_DataStructures )