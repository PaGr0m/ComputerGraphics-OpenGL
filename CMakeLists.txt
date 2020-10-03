cmake_minimum_required(VERSION 3.0)
project(Lesson-9-multiple-lighting-sources CXX)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

# CONFIG option is important so that CMake doesnt search for modules into the default modules directory
find_package(imgui CONFIG)
find_package(glfw CONFIG)
find_package(glew CONFIG)
find_package(fmt CONFIG)
find_package(glm CONFIG)
find_package(stb CONFIG)

add_executable(
        Lesson-9-multiple-lighting-sources

        src/main.cpp
        src/shader.cpp
        src/camera.h

        bindings/imgui_impl_glfw.cpp
        bindings/imgui_impl_opengl3.cpp

        assets/cube.vs
        assets/cube.fs
)

add_custom_command(
        TARGET Lesson-9-multiple-lighting-sources
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/assets
)

target_compile_definitions(
        Lesson-9-multiple-lighting-sources
        PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_GLEW
)

target_link_libraries(
        Lesson-9-multiple-lighting-sources
        imgui::imgui
        GLEW::glew_s
        glfw::glfw
        fmt::fmt
        glm::glm
        stb::stb
)
