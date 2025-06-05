message( STATUS "Installing utf8cpp ..." )

include(FetchContent)

FetchContent_Declare(
    utf8cpp
    GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
    GIT_TAG        master
)

FetchContent_MakeAvailable(utf8cpp)

set( UTF8CPP_INCLUDE_DIR ${utf8cpp_SOURCE_DIR}/source )
message( STATUS ${UTF8CPP_INCLUDE_DIR} )

message( STATUS "utf8cpp Installed!" )