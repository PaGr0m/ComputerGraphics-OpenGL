#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <utility>
#include <vector>
#include <string>

#include "shader.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh {
private:
    GLuint vbo_{}, vao_{}, ebo_{};
    std::vector<Vertex> vertices_;
    std::vector<GLuint> indices_;
    std::vector<Texture> textures_;

public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
            : vertices_(std::move(vertices)),
              indices_(std::move(indices)),
              textures_(std::move(textures)) {
        setup_mesh();
    }

    void draw(Shader &shader) {
        GLint diffuse_counter = 1;
        GLint specular_counter = 1;
        GLint normal_counter = 1;
        GLint height_counter = 1;
        for (GLint i = 0; i < textures_.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string number;
            std::string name = textures_[i].type;
            if (name == "texture_diffuse") number = std::to_string(diffuse_counter++);
            else if (name == "texture_specular") number = std::to_string(specular_counter++);
            else if (name == "texture_normal") number = std::to_string(normal_counter++);
            else if (name == "texture_height") number = std::to_string(height_counter++);

            shader.set_uniform(name + number, i);
            glBindTexture(GL_TEXTURE_2D, textures_[i].id);
        }

        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

private:
    void setup_mesh() {
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), &indices_[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void *>(offsetof(Vertex, normal)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void *>(offsetof(Vertex, texture_coords)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void *>(offsetof(Vertex, tangent)));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void *>(offsetof(Vertex, bitangent)));

        glBindVertexArray(0);
    }
};
