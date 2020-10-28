#pragma once

#include <string>
#include <utility>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <GL/glew.h>
#include "../elements/controller.h"


class OpenGl {
    std::string window_name_;
    GLFWwindow *window_;

public:
    explicit OpenGl(std::string window_name) : window_name_(std::move(window_name)) {
        window_ = init_opengl();
    }

    ~OpenGl() {
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    GLFWwindow *window() {
        return window_;
    }

private:
    GLFWwindow *init_opengl() {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        GLFWwindow *window = glfwCreateWindow(
                Settings::WINDOW_WIDTH,
                Settings::WINDOW_HEIGHT,
                window_name_.c_str(),
                nullptr,
                nullptr
        );

        if (window == nullptr) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(window);

//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetKeyCallback(window, key_callback);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("Failed to initialize GLEW");
        }

        int frame_width, frame_height;
        glfwGetFramebufferSize(window, &frame_width, &frame_height);
        glViewport(0, 0, frame_width, frame_height);
        glEnable(GL_DEPTH_TEST);

        return window;
    }
};
