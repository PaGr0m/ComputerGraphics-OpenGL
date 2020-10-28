#pragma once

#include <utility>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include "../utils/model.h"


class Camera {
public:
    // TODO: private
    glm::vec3 position_{};
    glm::vec3 eye_{};
    glm::vec3 normal_{};

    float angle_ = 90.0f;

    explicit Camera(glm::vec3 position) : position_(position) {}

    [[nodiscard]]
    glm::vec2 direction() const {
        return glm::vec2(cos(angle_), sin(angle_));
    }

    [[nodiscard]]
    glm::mat4 view() const {
        return glm::lookAt(eye_, position_, normal_);
    }

    [[nodiscard]]
    glm::vec3 position() const {
        return position_;
    }
};
