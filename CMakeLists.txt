
cmake_minimum_required(VERSION 3.23)

project(2dRenderer)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_compile_options(-g -fno-omit-frame-pointer)

add_executable(out)
add_executable(unit_tests)

add_library(2d-renderer)
add_library(xdg-shell)
add_library(doctest)

set_target_properties(doctest PROPERTIES LINKER_LANGUAGE CXX)

find_package(Wayland REQUIRED)

target_sources(xdg-shell
    PRIVATE 
        external/xdg-shell/xdg-shell-protocol.c

    PUBLIC 
        FILE_SET HEADERS 
        BASE_DIRS
            external/xdg-shell  
        FILES
            external/xdg-shell/xdg-shell-client-protocol.h
)

target_sources(doctest
    PRIVATE 
        test/doctest.cpp

    PUBLIC 
        FILE_SET HEADERS 
        BASE_DIRS
            test
        FILES
            test/doctest.h
)

file(GLOB_RECURSE RENDERER-SOURCES "./src/*.cpp")
file(GLOB_RECURSE RENDERER-HEADERS "./include/*.h")

target_sources(2d-renderer
    PRIVATE
        ${RENDERER-SOURCES}
    PUBLIC 
        FILE_SET HEADERS 
        BASE_DIRS 
            include
        FILES
            ${RENDERER_HEADERS}
)

file(GLOB_RECURSE RENDERER-TEST-SOURCES "./test/*.cpp")

target_sources(unit_tests
    PRIVATE 
        ${RENDERER-TEST-SOURCES}
        ${WAYLAND_CLIENT_INCLUDE_DIR}
)

target_sources(out
    PRIVATE 
        ${WAYLAND_CLIENT_INCLUDE_DIR}
        main.cpp
)

target_link_libraries(out
    PRIVATE
        2d-renderer
        xdg-shell
        ${WAYLAND_CLIENT_LIBRARY}
)

target_link_libraries(unit_tests
    PRIVATE
        2d-renderer
        xdg-shell
        doctest
        ${WAYLAND_CLIENT_LIBRARY}
)

enable_testing()

add_test(
  NAME unit_tests
  WORKING_DIRECTORY ../test
  COMMAND $<TARGET_FILE:unit_tests>
)