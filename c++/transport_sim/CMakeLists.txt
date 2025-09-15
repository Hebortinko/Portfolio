cmake_minimum_required(VERSION 3.30)
project(transport_sim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#core library
add_library(transport_sim_core
    src/core/Graph.cpp
    src/io/MapLoader.cpp
)

target_include_directories(transport_sim_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/libs/rapidjson/include
)

#google test via fetchcontent
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
find_package(Threads REQUIRED)





#main executable
add_executable(transport_sim
    src/main.cpp
)
target_link_libraries(transport_sim PRIVATE transport_sim_core)

#tests executable
add_executable(transport_sim_tests
    tests/core/test_graph.cpp
    tests/io/test_MapLoader.cpp
)


target_link_libraries(transport_sim_tests PRIVATE
        transport_sim_core
        GTest::gtest
        GTest::gtest_main
        Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(transport_sim_tests)
