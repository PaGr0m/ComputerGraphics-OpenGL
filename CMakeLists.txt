cmake_minimum_required(VERSION 3.0)
project(OpenGL-Fractal CXX)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

# CONFIG option is important so that CMake doesnt search for modules into the default modules directory
find_package(imgui CONFIG)
find_package(glfw CONFIG)
find_package(glew CONFIG)
find_package(fmt CONFIG)
find_package(glm CONFIG)

add_executable(
        OpenGL-Fractal

        src/main.cpp
        src/opengl_shader.cpp

        bindings/imgui_impl_glfw.cpp
        bindings/imgui_impl_opengl3.cpp

        lib/stb_image.cpp

        assets/simple-shader.vs
        assets/simple-shader.fs
)

add_custom_command(
        TARGET OpenGL-Fractal
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.vs ${PROJECT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.fs ${PROJECT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/resources ${PROJECT_BINARY_DIR}
)

target_compile_definitions(
        OpenGL-Fractal
        PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_GLEW
)

target_link_libraries(
        OpenGL-Fractal
        imgui::imgui
        GLEW::glew_s
        glfw::glfw
        fmt::fmt
        glm::glm
)

install(
        FILES
        ${PROJECT_SOURCE_DIR}/assets/simple-shader.vs
        ${PROJECT_SOURCE_DIR}/assets/simple-shader.fs

        DESTINATION ${PROJECT_BINARY_DIR}
)

install(
        DIRECTORY
        ${PROJECT_SOURCE_DIR}/resources

        DESTINATION ${PROJECT_BINARY_DIR}
)
