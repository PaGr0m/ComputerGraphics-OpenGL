#pragma once

#include <GL/glew.h>

struct Settings {
    // Textures
    const static unsigned TEXTURE_CUBEMAP = GL_TEXTURE0;

    const static unsigned TEXTURE_TORUS_1 = GL_TEXTURE1;
    const static unsigned TEXTURE_TORUS_2 = GL_TEXTURE2;
    const static unsigned TEXTURE_TORUS_3 = GL_TEXTURE3;
    const static unsigned TEXTURE_TORUS_4 = GL_TEXTURE4;

    const static unsigned TEXTURE_MODEL = GL_TEXTURE5;

    // Window
    const static GLuint WINDOW_WIDTH = 800;
    const static GLuint WINDOW_HEIGHT = 600;
    constexpr const static GLfloat RATIO = GLfloat(WINDOW_WIDTH) / GLfloat(WINDOW_HEIGHT);
};
