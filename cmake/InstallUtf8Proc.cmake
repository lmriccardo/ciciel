include(FetchContent)

FetchContent_Declare(
    utf8proc
    GIT_REPOSITORY https://github.com/JuliaStrings/utf8proc.git
    GIT_TAG        v2.10.0
)

message( STATUS "Fetching and building utf8proc" )
FetchContent_MakeAvailable(utf8proc)
set( UTF8PROC_INCLUDE_DIR ${utf8proc_SOURCE_DIR} )