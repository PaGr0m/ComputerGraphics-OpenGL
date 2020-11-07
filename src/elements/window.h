#pragma once

#include <GLFW/glfw3.h>
#include <utility>

#include <glm/gtx/rotate_vector.hpp>
#include "../utils/shader.h"
#include "controller.h"
#include "cubemap.h"
#include "torus.h"
#include "object.h"
#include "light.h"


class Window {
    Cubemap cubemap_;
    Torus torus_;
    Object model_;
    Controller controller_;
    Light light_;

    GLFWwindow *window_;

public:
    Window(const Cubemap &cubemap, Torus torus, Object car, Controller controller, Light light, GLFWwindow *window)
            : cubemap_(cubemap),
              torus_(std::move(torus)),
              model_(std::move(car)),
              controller_(std::move(controller)),
              light_(std::move(light)) {
        window_ = window;
    }

    void render() {
        while (!glfwWindowShouldClose(window_)) {
            check_events();
            init_display();
            check_error();

            // Init Model Matrix
            Camera camera = controller_.camera();
            auto model = create_aircraft_model_matrix(camera);

            // Create and activate Depth texture
            light_.render(model);
            auto light_view = light_.view();
            auto light_projection = light_.projection();
            render_elements(model, light_view, light_projection, light_view, light_projection);
            light_.activate();

            init_display();

            // Create View, Projection Matrix
            glm::mat4 view = camera.view();
            glm::mat4 projection = glm::perspective(
                    glm::radians(45.0f),
                    Settings::RATIO,
                    Settings::Z_NEAR,
                    Settings::Z_FAR
            );

            // Render elements
            render_elements(model, view, projection, light_view, light_projection);

            // Swap
            glfwSwapBuffers(window_);
        }
    }

private:
    void render_elements(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection,
                         glm::mat4 &light_view, glm::mat4 &light_projection) {
        torus_.render(view, projection, light_view, light_projection);
        cubemap_.render(view, projection);
        model_.render(model, view, projection);
    }

    glm::mat4 create_aircraft_model_matrix(Camera camera) {
        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, controller_.get_position(controller_.plane()));
        model *= glm::orientation(
                glm::normalize(controller_.get_normal(controller_.plane())),
                glm::normalize(controller_.get_normal({0, 0}))
        );

        auto current_point = controller_.plane();
        auto p1 = controller_.get_position(current_point);
        auto p2 = controller_.get_position(current_point + glm::vec2(0.0f, 1.0f));
        auto p3 = controller_.get_position(current_point + glm::vec2(1.0f, 0.0f));

        auto vec1 = p2 - p1;
        auto vec2 = p3 - p1;

        auto current_normal = controller_.get_normal(controller_.plane());
        auto cross_vec1 = glm::normalize(glm::cross(vec1, current_normal));
        auto cross_vec2 = glm::normalize(glm::cross(vec2, current_normal));

        auto zero_point = glm::vec2(0.0f, 0.0f);
        auto p4 = controller_.get_position(zero_point);
        auto p5 = controller_.get_position(zero_point + glm::vec2(0.0f, 1.0f));
        auto p6 = controller_.get_position(zero_point + glm::vec2(1.0f, 0.0f));

        auto vec3 = p5 - p4;
        auto vec4 = p6 - p4;

        auto zero_normal = controller_.get_normal(zero_point);
        auto cross_vec3 = glm::normalize(glm::cross(vec3, zero_normal));
        auto cross_vec4 = glm::normalize(glm::cross(vec4, zero_normal));

        auto rotation1 = glm::identity<glm::mat4>();
        rotation1 *= glm::orientation(zero_normal, cross_vec3);

        auto rotation2 = glm::identity<glm::mat4>();
        rotation2 *= glm::orientation(current_normal, cross_vec1);

        model *= glm::rotate(
                glm::radians(camera.angle() - 90.0f),
                controller_.get_normal({0, 0})
        );
        model *= glm::orientation(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));

        auto scale = 0.05f;
        model = glm::scale(model, glm::vec3(scale, scale, scale));

        return model;
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
