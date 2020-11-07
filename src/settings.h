#pragma once

#include <GL/glew.h>


struct Settings {
    // Shaders
    inline static const std::string TORUS_SHADER_VERTEX = "assets/shaders/torus.vs";
    inline static const std::string TORUS_SHADER_FRAGMENT = "assets/shaders/torus.fs";

    inline static const std::string CUBEMAP_SHADER_VERTEX = "assets/shaders/cubemap.vs";
    inline static const std::string CUBEMAP_SHADER_FRAGMENT = "assets/shaders/cubemap.fs";

    inline static const std::string OBJECT_SHADER_VERTEX = "assets/shaders/object.vs";
    inline static const std::string OBJECT_SHADER_FRAGMENT = "assets/shaders/object.fs";

    inline static const std::string LIGHT_SHADER_VERTEX = "assets/shaders/light.vs";
    inline static const std::string LIGHT_SHADER_FRAGMENT = "assets/shaders/light.fs";

    // Textures
    static const unsigned GL_TEXTURE_CUBEMAP = GL_TEXTURE0;

    static const unsigned GL_TEXTURE_TORUS_1 = GL_TEXTURE1;
    static const unsigned GL_TEXTURE_TORUS_2 = GL_TEXTURE2;
    static const unsigned GL_TEXTURE_TORUS_3 = GL_TEXTURE3;
    static const unsigned GL_TEXTURE_TORUS_4 = GL_TEXTURE4;

    static const unsigned GL_TEXTURE_DEPTH_MAP = GL_TEXTURE5;

    static const unsigned GL_TEXTURE_MODEL = GL_TEXTURE6;

    // Window
    inline static const std::string WINDOW_NAME = "Task-3-3D-scene";

    static const GLuint WINDOW_WIDTH = 800;
    static const GLuint WINDOW_HEIGHT = 600;

    static constexpr GLfloat RATIO = GLfloat(WINDOW_WIDTH) / GLfloat(WINDOW_HEIGHT);
    static constexpr GLfloat Z_NEAR = 0.1f;
    static constexpr GLfloat Z_FAR = 100.0f;

    // Shadow
    static const GLuint SHADOW_WIDTH = 4096;
    static const GLuint SHADOW_HEIGHT = 4096;

    static constexpr GLfloat LIGHT_Z_NEAR = 8.0f;
    static constexpr GLfloat LIGHT_Z_FAR = 19.0f;
    static constexpr glm::vec3 LIGHT_POSITION = glm::vec3(8.0f, 8.0f, 8.0f);

    // Paths
//    inline static const std::string PATH_TO_OBJECT = "assets/objects/aircraft/piper_pa18.obj";
    inline static const std::string PATH_TO_OBJECT = "assets/objects/OBJ/Soccer_Ball.obj";

    inline static const std::string LANDSCAPE_TEXTURE_LEVEL_1 = "assets/objects/torus/textures/soil.jpg";
    inline static const std::string LANDSCAPE_TEXTURE_LEVEL_2 = "assets/objects/torus/textures/plaster_wall.jpg";
    inline static const std::string LANDSCAPE_TEXTURE_LEVEL_3 = "assets/objects/torus/textures/grass_stone.jpg";
    inline static const std::string LANDSCAPE_TEXTURE_LEVEL_4 = "assets/objects/torus/textures/grass_flower.jpg";
    inline static const std::string LANDSCAPE_TEXTURE_HEIGHT_MAP = "assets/objects/torus/textures/height_map.jpg";

    inline static const std::vector<std::string> CUBEMAP_TEXTURES = {
            "assets/objects/skybox/textures/simple/right.jpg",
            "assets/objects/skybox/textures/simple/left.jpg",
            "assets/objects/skybox/textures/simple/top.jpg",
            "assets/objects/skybox/textures/simple/bottom.jpg",
            "assets/objects/skybox/textures/simple/front.jpg",
            "assets/objects/skybox/textures/simple/back.jpg"
    };
};
