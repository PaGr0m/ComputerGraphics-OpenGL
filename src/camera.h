#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera {
public:
    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

public:
    Camera(
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)
    ) {
        position_ = position;
        worldUp_ = up;
        update_camera_vectors();
    }

    glm::mat4 view() {
        return glm::lookAt(position_, position_ + front_, up_);
    }

    void keyboard_events(Movement direction, GLfloat deltaTime) {
        GLfloat velocity = MOVEMENT_SPEED * deltaTime;
        switch (direction) {
            case FORWARD :
                position_ += front_ * velocity;
                break;

            case BACKWARD:
                position_ -= front_ * velocity;
                break;

            case LEFT:
                position_ -= right_ * velocity;
                break;

            case RIGHT:
                position_ += right_ * velocity;
                break;

            default:
                break;
        }
    }

    void process_mouse_movement(GLfloat offset_x, GLfloat offset_y, GLboolean constrainPitch = true) {
        offset_x *= MOUSE_SENSITIVITY;
        offset_y *= MOUSE_SENSITIVITY;

        yaw_ += offset_x;
        pitch_ += offset_y;

        if (constrainPitch) {
            if (pitch_ > 89.0f) pitch_ = 89.0f;
            if (pitch_ < -89.0f) pitch_ = -89.0f;
        }

        update_camera_vectors();
    }

    void process_mouse_scroll(GLfloat offset_y) {
        if (zoom_ >= 1.0f && zoom_ <= 45.0f) zoom_ -= offset_y;
        if (zoom_ <= 1.0f) zoom_ = 1.0f;
        if (zoom_ >= 45.0f) zoom_ = 45.0f;
    }

    GLfloat zoom() const {
        return zoom_;
    }

    glm::vec3 position() const {
        return position_;
    }

    glm::vec3 front() const {
        return front_;
    }

private:
    void update_camera_vectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front.y = sin(glm::radians(pitch_));
        front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

        front_ = glm::normalize(front);
        right_ = glm::normalize(glm::cross(front_, worldUp_));
        up_ = glm::normalize(glm::cross(right_, front_));
    }

private:
    const GLfloat MOVEMENT_SPEED = 3.0f;
    const GLfloat MOUSE_SENSITIVITY = 0.25f;

private:
    glm::vec3 front_ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up_{};
    glm::vec3 right_{};
    glm::vec3 worldUp_{};
    glm::vec3 position_{};

    GLfloat yaw_ = -90.0f;
    GLfloat pitch_ = 0.0f;
    GLfloat zoom_ = 45.0f;
};
