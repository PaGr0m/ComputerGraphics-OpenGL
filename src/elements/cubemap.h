#include <glm/gtc/type_ptr.hpp>
#include "../utils/shader.h"
#include "../utils/model.h"
#include "../settings.h"


class Cubemap {
private:
    Shader shader_;
    GLuint cubemap_texture_;
    GLuint cubemap_vao;
    GLuint cubemap_vbo;

public:
    explicit Cubemap(const Shader &shader, const std::vector<std::string> &cube_textures)
            : shader_(shader),
              cubemap_vao(0),
              cubemap_vbo(0) {
        cubemap_texture_ = load_cubemap(cube_textures);
        init_buffers(cubemap_vbo, cubemap_vao);
    }

    ~Cubemap() {
        glDeleteVertexArrays(1, &cubemap_vao);
        glDeleteBuffers(1, &cubemap_vbo);
    }

    void render(glm::mat4 &view, glm::mat4 &projection) {
        glDepthFunc(GL_LEQUAL);
        shader_.use();
        shader_.set_uniform("u_skybox", 0);
        shader_.set_uniform("u_view", glm::value_ptr(view));
        shader_.set_uniform("u_projection", glm::value_ptr(projection));

        glBindVertexArray(cubemap_vao);
        glActiveTexture(Settings::GL_TEXTURE_CUBEMAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture_);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

private:
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

    void init_buffers(GLuint &skyboxVBO, GLuint &skyboxVAO) {
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
    const std::vector<GLfloat> skybox_vertices = {
            -10.0f, 10.0f, -10.0f,
            -10.0f, -10.0f, -10.0f,
            10.0f, -10.0f, -10.0f,
            10.0f, -10.0f, -10.0f,
            10.0f, 10.0f, -10.0f,
            -10.0f, 10.0f, -10.0f,

            -10.0f, -10.0f, 10.0f,
            -10.0f, -10.0f, -10.0f,
            -10.0f, 10.0f, -10.0f,
            -10.0f, 10.0f, -10.0f,
            -10.0f, 10.0f, 10.0f,
            -10.0f, -10.0f, 10.0f,

            10.0f, -10.0f, -10.0f,
            10.0f, -10.0f, 10.0f,
            10.0f, 10.0f, 10.0f,
            10.0f, 10.0f, 10.0f,
            10.0f, 10.0f, -10.0f,
            10.0f, -10.0f, -10.0f,

            -10.0f, -10.0f, 10.0f,
            -10.0f, 10.0f, 10.0f,
            10.0f, 10.0f, 10.0f,
            10.0f, 10.0f, 10.0f,
            10.0f, -10.0f, 10.0f,
            -10.0f, -10.0f, 10.0f,

            -10.0f, 10.0f, -10.0f,
            10.0f, 10.0f, -10.0f,
            10.0f, 10.0f, 10.0f,
            10.0f, 10.0f, 10.0f,
            -10.0f, 10.0f, 10.0f,
            -10.0f, 10.0f, -10.0f,

            -10.0f, -10.0f, -10.0f,
            -10.0f, -10.0f, 10.0f,
            10.0f, -10.0f, -10.0f,
            10.0f, -10.0f, -10.0f,
            -10.0f, -10.0f, 10.0f,
            10.0f, -10.0f, 10.0f
    };
};
