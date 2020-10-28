#pragma once

#include <GLFW/glfw3.h>
#include <utility>

#include "../utils/shader.h"
#include "controller.h"
#include "skybox.h"
#include "torus.h"
#include "car.h"


class Window {
    Skybox skybox_;
    Torus torus_;
    Car car_;
    Controller controller_;

    GLFWwindow *window_;

public:
    Window(const Skybox &skybox, Torus torus, Car car, Controller controller, GLFWwindow *window)
            : skybox_(skybox),
              torus_(std::move(torus)),
              car_(std::move(car)),
              controller_(std::move(controller)) {
        window_ = window;
    }

    void render() {
        while (!glfwWindowShouldClose(window_)) {
            check_events();
            init_display();
            check_error();


            // Init MVP
            auto model = glm::identity<glm::mat4>();
            model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));

            auto scale = 0.2f;
            model = glm::scale(model, glm::vec3(scale, scale, scale));

            glm::mat4 view = controller_.camera().view();
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), Settings::RATIO, 0.1f, 100.0f);


            // Render elements
            render_elements(model, view, projection);


//            view = controller_.camera().view();
        }
    }

private:
    void render_elements(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection) {
        torus_.render(view, projection);
        skybox_.render(view, projection);
        car_.render(model, view, projection);

        // Swap
        glfwSwapBuffers(window_);
    }

    // Check and call events
    void check_events() {
        glfwPollEvents();
        controller_.key_events();
    }

    // Error
    static void check_error() {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << gluErrorString(err) << std::endl;
        }
    }

    // Display
    void init_display() {
        int display_width, display_height;
        glfwGetFramebufferSize(window_, &display_width, &display_height);
        glViewport(0, 0, display_width, display_height);
        glClear(GLuint(GL_COLOR_BUFFER_BIT) | GLuint(GL_DEPTH_BUFFER_BIT));
    }
};
