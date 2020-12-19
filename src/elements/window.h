#pragma once

#include <utility>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include "cubemap.h"
#include "metaballs.h"
#include "../../bindings/imgui_impl_opengl3.h"
#include "../../bindings/imgui_impl_glfw.h"

class Window {
    Cubemap cubemap_;
    Metaballs metaballs_;
    ImGuiIO io_;
    GLFWwindow *window_;

public:
    explicit Window(const Cubemap &cubemap, Metaballs metaballs, GLFWwindow *window)
            : cubemap_(cubemap),
              metaballs_(std::move(metaballs)),
              window_(window),
              io_(init_ImGui(window)) {}

    void render() {
        while (!glfwWindowShouldClose(window_)) {
            check_error();
            check_events();
            init_display();
            mouse_events();

            // Create Mode, View, Projection Matrix
            glm::mat4 model = glm::mat4(1);
            glm::mat4 view = camera.view();
            glm::mat4 projection = glm::perspective(
                    glm::radians(Settings::CAMERA_DEGREES),
                    Settings::RATIO,
                    Settings::Z_NEAR,
                    Settings::Z_FAR
            );

            // Render elements
            metaballs_.render(model, view, projection);
            view = glm::mat4(glm::mat3(camera.view()));
            cubemap_.render(view, projection);

            // ImGui
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();
            ImGui::Begin("Information");
            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Swap
            glfwSwapBuffers(window_);
        }
    }

private:
    // Check and call events
    static void check_events() {
        glfwPollEvents();
    }

    // Mouse events
    void mouse_events() {
        int display_width, display_height;
        if (!ImGui::IsAnyWindowFocused()) {
            auto[delta_x, delta_y] = ImGui::GetMouseDragDelta();
            ImGui::ResetMouseDragDelta();

            glfwGetFramebufferSize(window_, &display_width, &display_height);
            camera.process_mouse_movement(delta_x / float(display_width),
                                          delta_y / float(display_height));
        }
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

    static ImGuiIO init_ImGui(GLFWwindow *window) {
        const char *glsl_version = "#version 430";

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        return io;
    }
};
