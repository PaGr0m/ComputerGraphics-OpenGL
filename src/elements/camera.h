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
    glm::vec3 position_;
    glm::vec3 eye_{};
    glm::vec3 normal_{};

    float angle_ = 90.0f;

public:
    explicit Camera(glm::vec3 position) : position_(position) {}

    [[nodiscard]]
    glm::vec2 direction() const {
        return glm::vec2(cos(glm::radians(angle_)), sin(glm::radians(angle_)));
    }

    [[nodiscard]]
    glm::mat4 view() const {
        return glm::lookAt(eye_, position_, normal_);
    }

    [[nodiscard]]
    glm::vec3 position() const {
        return position_;
    }

    [[nodiscard]]
    glm::vec3 normal() const {
        return normal_;
    }

    [[nodiscard]]
    float angle() const {
        return angle_;
    }
};
