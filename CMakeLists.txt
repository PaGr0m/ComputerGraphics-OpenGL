cmake_minimum_required(VERSION 3.0)
project(Lesson-1-window CXX)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

# CONFIG option is important so that CMake doesnt search for modules into the default modules directory
find_package(glfw CONFIG)
find_package(glew CONFIG)

add_executable(
        Lesson-1-window

        src/main.cpp
)

target_link_libraries(
        Lesson-1-window
        GLEW::glew_s
        glfw::glfw
)
