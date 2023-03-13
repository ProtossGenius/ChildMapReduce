cmake_minimum_required(VERSION 3.14)
project(mapreduce)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_FLAGS
        -g
        # -DVALGRIND
        -DCHECK_PTHREAD_RETURN_VALUE
        -D_FILE_OFFSET_BITS=64
        -Wall
        -Wextra
        -Werror
        -Wconversion
        -Wno-unused-parameter
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpointer-arith
        -Wshadow
        -Wwrite-strings
        -march=native
        # -MMD
        -rdynamic 
        )


include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(. ./third_part/)
add_library(mapreduce "")
target_sources(mapreduce
    PRIVATE
    ./mapreduce.h
    ./mapreduce.cpp
    ./param_parser.h
    ./param_parser.cpp
    ./file_spliter.h
    ./file_spliter.cpp
    ./file_reader.h
    ./file_reader.cpp
    ./mapper_result.h
    ./mapper_result.cpp
    ./worker_impl.h 
    ./worker_impl.cpp
    ./master_impl.h 
    ./master_impl.cpp
    ./defer.h
    ./file.h
    )
target_link_libraries(mapreduce nlohmann_json::nlohmann_json)
add_executable(master "")
target_sources(master
    PRIVATE
    ./master.cpp
    )

target_link_libraries(
    master
    mapreduce
    )
add_executable(worker "")
target_sources(worker
    PRIVATE
    ./master.cpp
    )

target_link_libraries(
    worker
    mapreduce
    )

add_executable(random_size ./random_size.cpp)
target_link_libraries(
    random_size
    mapreduce
    )


# google test
enable_testing()

add_executable(    unit_test
    ./unit_test/param_parser_test.cc
    ./unit_test/mapper_result_test.cc
    )
target_link_libraries(
    unit_test
    GTest::gtest_main
    mapreduce
    )

include(GoogleTest)
gtest_discover_tests(unit_test)
