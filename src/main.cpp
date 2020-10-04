#include "utils/shader.h"
#include "utils/camera.h"
#include "utils/model.h"
#include "initials.h"


int main() {
    GLFWwindow *window = init_opengl();
    ImGuiIO io = init_ImGui(window);

    // Buffers
    GLuint skybox_vbo, skybox_vao;
    init_buffers(skybox_vbo, skybox_vao);

    // Cubemap
    GLuint cubemap_texture = Model::load_cubemap(cube_textures);

    // Shader
    Shader nanosuit_shader("assets/nanosuit/nanosuit.vs", "assets/nanosuit/nanosuit.fs");
    Shader skybox_shader("assets/skybox/skybox.vs", "assets/skybox/skybox.fs");

    skybox_shader.use();
    skybox_shader.set_uniform("skybox", 0);

    // Model
    Model nanosuit_model("assets/nanosuit/scene.gltf");
//    Model nanosuit_model("assets/aircraft/piper_pa18.obj");

    // Uniforms
    static float coefficient_texture;
    static float coefficient_reflection;
    static float coefficient_refraction;

    while (!glfwWindowShouldClose(window)) {
        // Check and call events
        glfwPollEvents();

        // Settings
        int frame_width, frame_height;
        glfwGetFramebufferSize(window, &frame_width, &frame_height);
        glViewport(0, 0, frame_width, frame_height);
        glClear(GLuint(GL_COLOR_BUFFER_BIT) | GLuint(GL_DEPTH_BUFFER_BIT));

        // Mouse event
        if (!ImGui::IsAnyWindowFocused()) {
            auto[delta_x, delta_y] = ImGui::GetMouseDragDelta();
            ImGui::ResetMouseDragDelta();

            glfwGetFramebufferSize(window, &frame_width, &frame_height);
            camera.process_mouse_movement(delta_x / float(frame_width),
                                          delta_y / float(frame_height));
        }

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Texture", &coefficient_texture, 0.0f, 1.0f);
        ImGui::SliderFloat("Reflection", &coefficient_reflection, 0.0f, 1.0f);
        ImGui::SliderFloat("Refraction", &coefficient_refraction, 0.0f, 1.0f);
        ImGui::End();

        nanosuit_shader.use();
        nanosuit_shader.set_uniform("coefficient_texture", coefficient_texture);
        nanosuit_shader.set_uniform("coefficient_reflection", coefficient_reflection);
        nanosuit_shader.set_uniform("coefficient_refraction", coefficient_refraction);

        // Init MVP
        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5));

        glm::mat4 view = camera.view();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), RATIO, 0.1f, 100.0f);

        nanosuit_shader.set_uniform("model", glm::value_ptr(model));
        nanosuit_shader.set_uniform("view", glm::value_ptr(view));
        nanosuit_shader.set_uniform("projection", glm::value_ptr(projection));
        nanosuit_shader.set_uniform("camera_pos", camera.position());

        nanosuit_model.draw(nanosuit_shader);

        // Skybox
        glDepthFunc(GL_LEQUAL);
        skybox_shader.use();
        view = glm::mat4(glm::mat3(camera.view()));
        skybox_shader.set_uniform("view", glm::value_ptr(view));
        skybox_shader.set_uniform("projection", glm::value_ptr(projection));

        // Skybox cube
        glBindVertexArray(skybox_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &skybox_vao);
    glDeleteBuffers(1, &skybox_vbo);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
