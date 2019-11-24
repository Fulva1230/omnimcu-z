CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
project("omnimcu-lib")
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.8.0
)
FetchContent_MakeAvailable(googletest)
FetchContent_GetProperties(googletest)
if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif ()
include(GoogleTest)

add_executable(anTest utest.cpp)
target_link_libraries(anTest gtest gtest_main)

gtest_add_tests(TARGET anTest)

