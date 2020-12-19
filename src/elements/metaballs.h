#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "../utils/shader.h"
#include "../utils/buffers.h"

class Metaballs {
    constexpr static int GRID_SIZE = 40;
    constexpr static float GRID_MINI_CUBE_SIZE = 0.05f;

    Shader shader_;

    GLuint vao_;
    GLuint vbo_;

    std::vector<float> grid_;

public:
    explicit Metaballs(const Shader &shader)
            : shader_(shader),
              vao_(0),
              vbo_(0),
              grid_(init_grid()) {
        init_metaballs(vbo_, vao_);
        init_buffer_edge_table();
        init_buffer_triangle_table();
    }

    void render(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection) {
        shader_.use();
        shader_.set_uniform("u_model", glm::value_ptr(model));
        shader_.set_uniform("u_view", glm::value_ptr(view));
        shader_.set_uniform("u_projection", glm::value_ptr(projection));
        shader_.set_uniform("u_camera_pos", camera.position());

        glBindVertexArray(vao_);
        glDrawArrays(GL_POINTS, 0, grid_.size() / 3);
        glBindVertexArray(0);
    }

private:
    static std::vector<float> init_grid() {
        std::vector<float> grid;

        for (int i = -GRID_SIZE; i < GRID_SIZE; ++i) {
            for (int j = -GRID_SIZE; j < GRID_SIZE; ++j) {
                for (int k = -GRID_SIZE; k < GRID_SIZE; ++k) {
                    grid.push_back(float(i) * GRID_MINI_CUBE_SIZE);
                    grid.push_back(float(j) * GRID_MINI_CUBE_SIZE);
                    grid.push_back(float(k) * GRID_MINI_CUBE_SIZE);
                }
            }
        }

        return grid;
    }

    void init_metaballs(GLuint &vbo, GLuint &vao) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     grid_.size() * sizeof(grid_[0]),
                     grid_.data(),
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    }

    static void init_buffer_edge_table() {
        auto buffer_data = Buffers::TABLE_EDGES;
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                     buffer_data.size() * sizeof(buffer_data[0]),
                     buffer_data.data(),
                     GL_STATIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);
    }

    static void init_buffer_triangle_table() {
        auto buffer_data = Buffers::TABLE_TRIANGLE;
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                     buffer_data.size() * sizeof(buffer_data[0]),
                     buffer_data.data(),
                     GL_STATIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, buffer);
    }
};
