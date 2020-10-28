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

    glm::ivec2 plane_max = {1024, 256};
    glm::ivec2 plane = {0, 0};

    float angle_shift = 0.01f;

public:
    explicit Controller(const Torus &torus)
            : table_(torus.table()),
              camera_(Camera(table_.position[0][0])) {}

    Camera camera() {
        return camera_;
    }

    void key_events() {
        glm::ivec2 direction = glm::ivec2(camera_.direction() * 1.5f);

        if (keys[GLFW_KEY_W]) plane += direction;
        if (keys[GLFW_KEY_S]) plane -= direction;

        if (keys[GLFW_KEY_A]) camera_.angle_ += angle_shift;
        if (keys[GLFW_KEY_D]) camera_.angle_ -= angle_shift;

        normalize(plane);

        glm::vec3 position = table_.position[plane.x][plane.y];
        glm::vec3 normal = table_.normal[plane.x][plane.y];
        auto new_direction = plane - direction;
        normalize(new_direction);

        camera_.position_ = position + 0.45f * normal;
        camera_.eye_ = table_.position[new_direction.x][new_direction.y] + 0.5f * normal;
        camera_.normal_ = normal;
    }

private:
    void normalize(glm::ivec2 &vec) const {
        vec.x = vec.x % plane_max.x;
        vec.y = vec.y % plane_max.y;

        if (vec.x < 0) vec.x = plane_max.x - 1;
        if (vec.y < 0) vec.y = plane_max.y - 1;
    }
};
