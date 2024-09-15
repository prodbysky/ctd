cmake_minimum_required(VERSION 3.10)

project(ctd VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(external/raylib)


set(SOURCES src/main.cpp)

add_executable(ctd ${SOURCES})
target_include_directories(ctd PRIVATE ${CMAKE_SOURCE_DIR}/external/raylib/src)

target_link_libraries(ctd raylib)
