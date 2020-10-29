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
    TorusCoordinateTable table_;
    Camera camera_;

    const glm::ivec2 plane_max_ = {1024, 256};
    glm::ivec2 plane_ = {0, 0};

    float angle_shift_ = 0.1f;

public:
    explicit Controller(const Torus &torus)
            : table_(torus.table()),
              camera_(Camera(table_.position[0][0])) {}

    Camera camera() {
        return camera_;
    }

    void key_events() {
        glm::ivec2 direction = glm::ivec2(camera_.direction() * 1.5f);

        if (keys[GLFW_KEY_W]) plane_ += direction;
        if (keys[GLFW_KEY_S]) plane_ -= direction;

        if (keys[GLFW_KEY_A]) camera_.angle_ += angle_shift_;
        if (keys[GLFW_KEY_D]) camera_.angle_ -= angle_shift_;

        normalize(plane_);

        glm::vec3 position = table_.position[plane_.x][plane_.y];
        glm::vec3 normal = table_.normal[plane_.x][plane_.y];

        direction = plane_ - direction;
        normalize(direction);

        camera_.position_ = position + 1.2f * normal;
        camera_.eye_ = table_.position[direction.x][direction.y] + 2.9f * normal;
        camera_.normal_ = normal;
    }

private:
    void normalize(glm::ivec2 &vec) const {
        vec = vec % plane_max_;

        if (vec.x < 0) vec.x = plane_max_.x - 1;
        if (vec.y < 0) vec.y = plane_max_.y - 1;
    }
};
