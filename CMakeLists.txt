cmake_minimum_required(VERSION 3.10)

project(ctd VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(ASSETS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_DEST_DIR ${CMAKE_BINARY_DIR}/assets)

add_subdirectory(external/raylib)
set(CMAKE_BUILD_TYPE Debug)
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
)


set(SOURCES 
    src/main.cpp
    src/path.cpp
    src/path_follower.cpp
    src/enemy.cpp
)

add_executable(ctd ${SOURCES})
target_include_directories(ctd PRIVATE ${CMAKE_SOURCE_DIR}/external/raylib/src)

target_link_libraries(ctd raylib)

add_dependencies(ctd copy_assets)
