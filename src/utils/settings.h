#pragma once

#include <string>
#include "../elements/camera.h"

auto camera = Camera(glm::vec3(0.0f, 0.0f, 0.5f));

struct Settings {
    // Shaders
    inline static const std::string CUBEMAP_SHADER_VERTEX = "assets/cubemap/shaders/vertex.glsl";
    inline static const std::string CUBEMAP_SHADER_FRAGMENT = "assets/cubemap/shaders/fragment.glsl";

    inline static const std::string METABALLS_SHADER_VERTEX = "assets/metaballs/shaders/vertex.glsl";
    inline static const std::string METABALLS_SHADER_GEOMETRY = "assets/metaballs/shaders/geometry.glsl";
    inline static const std::string METABALLS_SHADER_FRAGMENT = "assets/metaballs/shaders/fragment.glsl";

    // Textures
    inline static const unsigned GL_TEXTURE_CUBEMAP = GL_TEXTURE0;

    // Window
    inline static const std::string WINDOW_NAME = "Task-4-metaballs";
    inline static const GLuint WINDOW_WIDTH = 800;
    inline static const GLuint WINDOW_HEIGHT = 600;
    inline static const GLfloat CAMERA_DEGREES = 45.0f;
    inline static const GLfloat Z_NEAR = 0.1f;
    inline static const GLfloat Z_FAR = 40.0f;
    inline static constexpr GLfloat RATIO = GLfloat(WINDOW_WIDTH) / GLfloat(WINDOW_HEIGHT);
};
