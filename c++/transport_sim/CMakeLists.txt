cmake_minimum_required(VERSION 3.30)
project(transport_sim)

set(CMAKE_CXX_STANDARD 26)


add_executable(transport_sim
        src/main.cpp
        src/core/Graph.cpp
        src/io/MapLoader.cpp
)

target_include_directories(transport_sim
        PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/rapidjson/include
        ${CMAKE_SOURCE_DIR}/src
)
