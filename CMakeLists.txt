cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME "Task-3-3D-scene")

project(${PROJECT_NAME} CXX)

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
        ${PROJECT_NAME}

        src/main.cpp
        src/settings.h

        src/utils/mesh.h
        src/utils/model.h
        src/utils/opengl.h
        src/utils/shader.cpp

        src/elements/camera.h
        src/elements/car.h
        src/elements/controller.h
        src/elements/skybox.h
        src/elements/torus.h
        src/elements/window.h

        bindings/imgui_impl_glfw.cpp
        bindings/imgui_impl_opengl3.cpp

        assets/skybox/skybox.vs
        assets/skybox/skybox.fs
        assets/nanosuit/nanosuit.vs
        assets/nanosuit/nanosuit.fs
)

add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/assets
)

target_compile_definitions(
        ${PROJECT_NAME}
        PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_GLEW
)

target_link_libraries(
        ${PROJECT_NAME}
        imgui::imgui
        GLEW::glew_s
        glfw::glfw
        fmt::fmt
        glm::glm
        stb::stb
        assimp::assimp
)

install(
        DIRECTORY
        ${PROJECT_SOURCE_DIR}/assets

        DESTINATION ${PROJECT_BINARY_DIR}
)
