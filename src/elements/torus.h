#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "../utils/mesh.h"
#include "../utils/model.h"

struct TorusCoordinateTable {
    using POSITION = std::vector<std::vector<glm::vec3>>;
    using NORMAL = std::vector<std::vector<glm::vec3>>;

    POSITION position;
    NORMAL normal;

private:
    glm::ivec2 vec_size_{};

public:
    TorusCoordinateTable() = default;

    explicit TorusCoordinateTable(glm::ivec2 vec_size) : vec_size_(vec_size) {
        // Init table (rectangle pos -> torus pos)
        resize_buffer(position);
        resize_buffer(normal);
    }

private:
    void resize_buffer(std::vector<std::vector<glm::vec3>> &table) const {
        table.resize(vec_size_.x);
        for (auto &elem : table) {
            elem.resize(vec_size_.y);
        }
    }
};


class Torus {
// Const values
private:
    // Torus settings
    const float radius_major_ = 5.0f;
    const float radius_minor_ = 1.0f;

    const glm::ivec2 plane = {1024, 256};

// Variables
private:
    using Point = glm::ivec2;

    // Buffers
    GLuint vao_;
    std::vector<glm::vec3> vertices;
    std::vector<int> indices;
    TorusCoordinateTable rectangle2torus;

    // Height map
    GLubyte *height_map_{};
    GLint height_map_width_{};
    GLint height_map_height_{};
    GLint height_map_components_{};

    // Shader
    Shader shader_;

    float height_min_{};
    float height_max_{};

public:
    explicit Torus(const Shader &shader) : shader_(shader), vao_(0), rectangle2torus(plane) {
        load_textures();
        load_height_map();
        fill_buffers();
        bind_buffers();

        std::vector<float> heights;
        for (auto &raw: rectangle2torus.position) for (auto &elem: raw) heights.push_back(elem.z);

        height_min_ = *std::min_element(heights.begin(), heights.end());
        height_max_ = *std::max_element(heights.begin(), heights.end());
    }

    void render(glm::mat4 &view, glm::mat4 &projection, glm::mat4 &light_view, glm::mat4 &light_projection) {
        shader_.use();
        shader_.set_uniform("u_view", glm::value_ptr(view));
        shader_.set_uniform("u_projection", glm::value_ptr(projection));
        shader_.set_uniform("u_light_view", glm::value_ptr(light_view));
        shader_.set_uniform("u_light_projection", glm::value_ptr(light_projection));

        shader_.set_uniform("u_texture1", 1);
        shader_.set_uniform("u_texture2", 2);
        shader_.set_uniform("u_texture3", 3);
        shader_.set_uniform("u_texture4", 4);
        shader_.set_uniform("u_texture_depth", 5);

        // TODO: rework
//        shader_.set_uniform("u_height_min", height_min_);
        shader_.set_uniform("u_height_min", 1.0f);
        shader_.set_uniform("u_height_max", height_max_);
        shader_.set_uniform("u_height_shift", (std::abs(height_max_) + std::abs(height_min_)) / 4);

        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, nullptr);
    }

    [[nodiscard]]
    auto table() const {
        return rectangle2torus;
    }

private:
    float evaluate_height(Point point) {
        auto x = point.x * height_map_width_ * 6 / plane.x;
        x %= height_map_width_;

        auto y = point.y * height_map_height_ / plane.y;
        y %= height_map_height_;

        auto idx = x * height_map_height_ + y;

        return float(height_map_[3 * idx]) / 255.0f;
    }

    glm::vec3 evaluate_point(Point point) {
        auto phi = float((M_PI * 2.0 * point.x) / (plane.x));
        auto psi = float(M_PI * (2.0 * point.y / (plane.y)));
        float height = evaluate_height(point) * 0.2f;

        return glm::vec3(
                (radius_major_ + (radius_minor_ + height) * cos(psi)) * cos(phi),
                (radius_major_ + (radius_minor_ + height) * cos(psi)) * sin(phi),
                (radius_minor_ + height) * sin(psi)
        );
    }

    [[nodiscard]]
    glm::vec3 evaluate_normal(Point point) const {
        auto phi = float((M_PI * 2.0 * point.x) / (plane.x - 1));
        auto psi = float(M_PI * (2.0 * point.y / (plane.y - 1)));

        return glm::vec3(
                cos(psi) * cos(phi),
                cos(psi) * sin(phi),
                sin(psi)
        );
    }

    glm::vec3 evaluate_texture_coordinate(Point point) {
        return glm::vec3(evaluate_point(point));
    }

    static GLuint load_texture_from_file(const std::string &filename) {
        GLuint textureID;
        glGenTextures(1, &textureID);

        GLint width, height, nrComponents;
        GLubyte *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1) format = GL_RED;
            else if (nrComponents == 3) format = GL_RGB;
            else if (nrComponents == 4) format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        } else {
            throw std::runtime_error("textures failed to load at path: " + filename);
        }

        stbi_image_free(data);

        return textureID;
    }

    static void load_textures() {
        glActiveTexture(Settings::GL_TEXTURE_TORUS_1);
        glBindTexture(GL_TEXTURE_2D, load_texture_from_file(Settings::LANDSCAPE_TEXTURE_LEVEL_1));

        glActiveTexture(Settings::GL_TEXTURE_TORUS_2);
        glBindTexture(GL_TEXTURE_2D, load_texture_from_file(Settings::LANDSCAPE_TEXTURE_LEVEL_2));

        glActiveTexture(Settings::GL_TEXTURE_TORUS_3);
        glBindTexture(GL_TEXTURE_2D, load_texture_from_file(Settings::LANDSCAPE_TEXTURE_LEVEL_3));

        glActiveTexture(Settings::GL_TEXTURE_TORUS_4);
        glBindTexture(GL_TEXTURE_2D, load_texture_from_file(Settings::LANDSCAPE_TEXTURE_LEVEL_4));

        glActiveTexture(GL_TEXTURE0);
    }

    void load_height_map() {
        std::string filename = std::string(Settings::LANDSCAPE_TEXTURE_HEIGHT_MAP);

        height_map_ = stbi_load(filename.c_str(),
                                &height_map_width_,
                                &height_map_height_,
                                &height_map_components_,
                                0);
        if (!height_map_) {
            throw std::runtime_error("Error load image");
        }
    }

    void fill_buffers() {
        // Generate torus arrays
        for (int i = 0; i < plane.x; ++i) {
            for (int j = 0; j < plane.y; ++j) {
                auto torus_point = evaluate_point({i, j});
                auto torus_normal = evaluate_normal({i, j});

                rectangle2torus.position[i][j] = torus_point;
                rectangle2torus.normal[i][j] = torus_normal;

                vertices.push_back(torus_point);
                vertices.push_back(torus_normal);
                vertices.push_back(evaluate_texture_coordinate({i, j}));

                // Triangle 1
                int ip1 = (i + 1) % plane.x;
                int jp1 = (j + 1) % plane.y;

                indices.push_back(i * plane.y + j);
                indices.push_back((i * plane.y + jp1));
                indices.push_back((ip1 * plane.y + jp1));

                // Triangle 2
                indices.push_back((ip1 * plane.y + jp1));
                indices.push_back((ip1 * plane.y + j));
                indices.push_back(i * plane.y + j);
            }
        }
    }

    void bind_buffers() {
        GLuint vbo, ebo;

        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (6 * sizeof(float)));

        glBindVertexArray(0);
    }
};
