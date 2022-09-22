#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    const GLfloat MOUSE_SENSITIVITY = 45.0f;

    glm::vec3 position_{};

    GLfloat pitch_ = 0.0f;
    GLfloat yaw_ = 0.0f;

    GLfloat zoom_ = 45.0f;
    
public:
    explicit Camera(glm::vec3 position) {
        position_ = position;
    }

    [[nodiscard]]
    glm::mat4 view() const {
        return glm::lookAt(
                position(),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
        );
    }

    void process_mouse_movement(GLfloat offset_x, GLfloat offset_y) {
        yaw_ += offset_x * MOUSE_SENSITIVITY;
        pitch_ += offset_y * MOUSE_SENSITIVITY;

        if (pitch_ > 89.0f) pitch_ = 89.0f;
        if (pitch_ < -89.0f) pitch_ = -89.0f;
    }

    void process_mouse_scroll(GLfloat offset_y) {
        if (offset_y < 0) zoom_ *= 1.2;
        else zoom_ /= 1.2;
    }

    [[nodiscard]]
    glm::vec3 position() const {
        glm::vec3 pos = position_ * zoom_;

        auto rotate = glm::rotate(
                glm::identity<glm::mat4>(),
                glm::radians(yaw_),
                glm::vec3(0.0f, 1.0f, 0.0f)
        );
        pos = glm::vec3(rotate * glm::vec4(pos, 1.0f));

        auto inverse = glm::vec3(rotate * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        rotate = glm::rotate(
                glm::identity<glm::mat4>(),
                glm::radians(pitch_),
                inverse
        );

        return pos = glm::vec3(rotate * glm::vec4(pos, 1.0f));
    }
};
