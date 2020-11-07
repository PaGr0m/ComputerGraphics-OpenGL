#pragma once

#include <GLFW/glfw3.h>
#include <utility>

#include <glm/gtx/rotate_vector.hpp>
#include "../utils/shader.h"
#include "controller.h"
#include "skybox.h"
#include "torus.h"
#include "car.h"


class Window {
    Skybox skybox_;
    Torus torus_;
    Car model_;
    Controller controller_;

    GLFWwindow *window_;

    // lighting info
    glm::vec3 lightPos = glm::vec3(8.0f, 8.0f, 8.0f);

    Shader simpleDepthShader = Shader(
            "assets/shadow/depth_map.vs",
            "assets/shadow/depth_map.fs"
    );

    unsigned int depthMapFBO{};
    const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
    GLuint depthMap{};

public:
    Window(const Skybox &skybox, Torus torus, Car car, Controller controller, GLFWwindow *window)
            : skybox_(skybox),
              torus_(std::move(torus)),
              model_(std::move(car)),
              controller_(std::move(controller)) {
        window_ = window;
        init();
    }

    void init() {
        glGenFramebuffers(1, &depthMapFBO);

        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH, SHADOW_HEIGHT,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glm::mat4 create_aircraft_model_matrix(Camera camera) {
        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, controller_.get_position(controller_.plane()));
        model *= glm::orientation(
                glm::normalize(controller_.get_normal(controller_.plane())),
                glm::normalize(controller_.get_normal({0, 0}))
        );
        model *= glm::rotate(
                glm::radians(camera.angle() - 90.0f),
                controller_.get_normal({0, 0})
        );
        model *= glm::orientation(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));

        auto scale = 0.05f;
        model = glm::scale(model, glm::vec3(scale, scale, scale));

        return model;
    }

    void render() {
        while (!glfwWindowShouldClose(window_)) {
            check_events();
            init_display();
            check_error();


            // Init MVP
            Camera camera = controller_.camera();
            auto model = create_aircraft_model_matrix(camera);

            // 1. render depth of scene to texture (from light's perspective)
            // --------------------------------------------------------------
            float near_plane = 8.0f;
            float far_plane = 19.0f;
            glm::mat4 light_projection = glm::ortho(
                    -6.0f, 6.0f,
                    -6.0f, 6.0f,
                    near_plane, far_plane
            );
            glm::mat4 light_view = glm::lookAt(
                    lightPos,
                    glm::vec3(0.0f),
                    glm::vec3(0.0, 1.0, 0.0)
            );
            glm::mat4 light_space_matrix = light_projection * light_view;

            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glClear(GLuint(GL_DEPTH_BUFFER_BIT));

            // render scene from light's point of view ll sadasds
            simpleDepthShader.use();
            simpleDepthShader.set_uniform("lightSpaceMatrix", glm::value_ptr(light_space_matrix));
            render_elements(model, light_view, light_projection, light_view, light_projection);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glActiveTexture(Settings::TEXTURE_DEPTH);
            glBindTexture(GL_TEXTURE_2D, depthMap);

            glClear(GLuint(GL_COLOR_BUFFER_BIT) | GLuint(GL_DEPTH_BUFFER_BIT));

            int display_width, display_height;
            glfwGetFramebufferSize(window_, &display_width, &display_height);
            glViewport(0, 0, display_width, display_height);

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
        skybox_.render(view, projection);
        model_.render(model, view, projection);
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
