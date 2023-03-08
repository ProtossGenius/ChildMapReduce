cmake_minimum_required(VERSION 3.14)
project(myproj)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(myproj main.cpp)

# target_link_libraries(myproj)


# google test
enable_testing()
add_executable(
    unit_test
    ./unit_tests/hello_test.cc
    ./unit_tests/world_test.cpp
    )
target_link_libraries(
    unit_test
    GTest::gtest_main
    )

include(GoogleTest)
gtest_discover_tests(unit_test)
