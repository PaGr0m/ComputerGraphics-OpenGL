#pragma once

#include <array>
#include <utility>
#include "torus.h"
#include "camera.h"

std::array<bool, 1024> keys;

inline void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

class Controller {


private:
    using Table = std::vector<std::vector<glm::vec3>>;

    TorusCoordinateTable table_;
    Camera camera_ = Camera(glm::vec3(0, 0, 0));

    glm::vec2 plane_ = {0.0f, 0.0f};
    const glm::ivec2 plane_max_ = {1024, 256};

    float angle_shift_ = 1.0f;

public:
    explicit Controller(const Torus &torus) : table_(torus.table()) {
        camera_ = Camera(get_position(glm::vec2(0.0f, 0.0f)));
    }

    Camera camera() {
        return camera_;
    }

    void key_events() {
        glm::vec2 direction = camera_.direction();

        if (keys[GLFW_KEY_W]) plane_ += direction;
        if (keys[GLFW_KEY_S]) plane_ -= direction;

        if (keys[GLFW_KEY_A]) camera_.angle_ += angle_shift_;
        if (keys[GLFW_KEY_D]) camera_.angle_ -= angle_shift_;

        glm::vec2 point1 = plane_;
        glm::vec2 point2 = plane_ - direction * 5.0f;

        camera_.position_ = get_position(point1) + 1.5f * get_normal(point1);
        camera_.eye_ = get_position(point2) + 2.9f * get_normal(point2);
        camera_.normal_ = get_normal(point1);
    }

    [[nodiscard]]
    glm::vec3 get_normal(glm::vec2 point) {
        return barycentric_coordinates(point, table_.normal);
    }

    [[nodiscard]]
    glm::vec3 get_position(glm::vec2 point) {
        return barycentric_coordinates(point, table_.position);
    }

    [[nodiscard]]
    glm::vec2 plane() const {
        return plane_;
    }

private:
    glm::vec3 barycentric_coordinates(glm::vec2 point, const Table &table) {
        glm::ivec2 p1 = normalize(point);
        glm::ivec2 p2 = normalize(p1 + glm::ivec2(0, 1));
        glm::ivec2 p3 = normalize(p1 + glm::ivec2(1, 1));
        glm::ivec2 p4 = normalize(p1 + glm::ivec2(1, 0));

        glm::vec2 offset = point - glm::vec2(glm::ivec2(point));

        if (offset.y > offset.x) {
            return glm::vec3(
                    (1 - offset.y) * table[p1.x][p1.y] +
                    offset.x * table[p3.x][p3.y] +
                    (offset.y - offset.x) * table[p2.x][p2.y]
            );
        }

        return glm::vec3(
                (1 - offset.x) * table[p1.x][p1.y] +
                offset.y * table[p3.x][p3.y] +
                (offset.x - offset.y) * table[p4.x][p4.y]
        );
    }


    [[nodiscard]]
    glm::ivec2 normalize(const glm::ivec2 &vec) const {
        return ((vec % plane_max_) + plane_max_) % plane_max_;
    }
};
