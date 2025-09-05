include(FetchContent)

# Try to find installed packages first
find_package(GTest QUIET)
find_package(benchmark QUIET)

if(NOT GTest_FOUND)
    message(STATUS "Fetching googletest ...")

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )

    set(GTEST_BUILD_GMOCK OFF CACHE BOOL "Disable building GoogleMock")
    set(GTEST_BUILD_TESTS OFF CACHE BOOL "Disable building GoogleTest tests")

    FetchContent_MakeAvailable(googletest)
endif()

if(NOT benchmark_FOUND)
    message(STATUS "Fetching benchmark ...")
    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.1
    )

    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable building Benchmark tests")
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Disable building Benchmark GoogleTest tests")

    FetchContent_MakeAvailable(googlebenchmark)
endif()