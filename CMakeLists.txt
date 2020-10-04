cmake_minimum_required(VERSION 3.0)
project(Task-2-3Dobject CXX)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)

# CONFIG option is important so that CMake doesnt search for modules into the default modules directory
find_package(imgui CONFIG)
find_package(glfw CONFIG)
find_package(glew CONFIG)
find_package(fmt CONFIG)
find_package(glm CONFIG)
find_package(stb CONFIG)
find_package(assimp CONFIG)

add_executable(
        Task-2-3Dobject

        src/main.cpp
        src/initials.h
        src/buffers.h

        src/utils/shader.cpp
        src/utils/camera.h
        src/utils/model.h
        src/utils/mesh.h

        bindings/imgui_impl_glfw.cpp
        bindings/imgui_impl_opengl3.cpp

        assets/skybox/skybox.vs
        assets/skybox/skybox.fs
        assets/nanosuit/nanosuit.vs
        assets/nanosuit/nanosuit.fs
)

add_custom_command(
        TARGET Task-2-3Dobject
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/assets
)

target_compile_definitions(
        Task-2-3Dobject
        PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_GLEW
)

target_link_libraries(
        Task-2-3Dobject
        imgui::imgui
        GLEW::glew_s
        glfw::glfw
        fmt::fmt
        glm::glm
        stb::stb
        assimp::assimp
)
