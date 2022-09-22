#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../utils/shader.h"
#include "../utils/settings.h"

class Cubemap {
    Shader shader_;
    GLuint texture_id_;
    GLuint vao_;
    GLuint vbo_;

public:
    explicit Cubemap(const Shader &shader)
            : shader_(shader),
              vao_(0),
              vbo_(0) {
        texture_id_ = load_cubemap(cube_textures);
        init_buffers(vbo_, vao_);
    }

    ~Cubemap() {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
    }

    void render(glm::mat4 &view, glm::mat4 &projection) {
        glDepthFunc(GL_LEQUAL);
        shader_.use();
        shader_.set_uniform("u_skybox", 0);
        shader_.set_uniform("u_view", glm::value_ptr(view));
        shader_.set_uniform("u_projection", glm::value_ptr(projection));

        glBindVertexArray(vao_);
        glActiveTexture(Settings::GL_TEXTURE_CUBEMAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

    static GLuint load_cubemap(const std::vector<std::string> &maps) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrComponents;
        for (GLuint i = 0; i < maps.size(); i++) {
            GLubyte *data = stbi_load(maps[i].c_str(), &width, &height, &nrComponents, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            } else {
                std::cout << "Cubemap texture failed to load at path: " << maps[i] << std::endl;
                stbi_image_free(data);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    static void init_buffers(GLuint &skyboxVBO, GLuint &skyboxVAO) {
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     skybox_vertices.size() * sizeof(skybox_vertices[0]),
                     skybox_vertices.data(),
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    }

private:
    inline static const std::vector<GLfloat> skybox_vertices = {
            -20.0f, 20.0f, -20.0f,
            -20.0f, -20.0f, -20.0f,
            20.0f, -20.0f, -20.0f,
            20.0f, -20.0f, -20.0f,
            20.0f, 20.0f, -20.0f,
            -20.0f, 20.0f, -20.0f,

            -20.0f, -20.0f, 20.0f,
            -20.0f, -20.0f, -20.0f,
            -20.0f, 20.0f, -20.0f,
            -20.0f, 20.0f, -20.0f,
            -20.0f, 20.0f, 20.0f,
            -20.0f, -20.0f, 20.0f,

            20.0f, -20.0f, -20.0f,
            20.0f, -20.0f, 20.0f,
            20.0f, 20.0f, 20.0f,
            20.0f, 20.0f, 20.0f,
            20.0f, 20.0f, -20.0f,
            20.0f, -20.0f, -20.0f,

            -20.0f, -20.0f, 20.0f,
            -20.0f, 20.0f, 20.0f,
            20.0f, 20.0f, 20.0f,
            20.0f, 20.0f, 20.0f,
            20.0f, -20.0f, 20.0f,
            -20.0f, -20.0f, 20.0f,

            -20.0f, 20.0f, -20.0f,
            20.0f, 20.0f, -20.0f,
            20.0f, 20.0f, 20.0f,
            20.0f, 20.0f, 20.0f,
            -20.0f, 20.0f, 20.0f,
            -20.0f, 20.0f, -20.0f,

            -20.0f, -20.0f, -20.0f,
            -20.0f, -20.0f, 20.0f,
            20.0f, -20.0f, -20.0f,
            20.0f, -20.0f, -20.0f,
            -20.0f, -20.0f, 20.0f,
            20.0f, -20.0f, 20.0f
    };

    const std::vector<std::string> cube_textures = {
            "assets/cubemap/textures/simple/right.jpg",
            "assets/cubemap/textures/simple/left.jpg",
            "assets/cubemap/textures/simple/top.jpg",
            "assets/cubemap/textures/simple/bottom.jpg",
            "assets/cubemap/textures/simple/front.jpg",
            "assets/cubemap/textures/simple/back.jpg"
    };
};
