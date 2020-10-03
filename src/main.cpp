#include "utils/shader.h"
#include "utils/camera.h"
#include "initials.h"
#include "utils/model.h"


int main() {
    GLFWwindow *window = init_opengl();

    int frame_width, frame_height;
    glfwGetFramebufferSize(window, &frame_width, &frame_height);

    // Shader
    Shader nanosuit_shader("assets/nanosuit/nanosuit.vs", "assets/nanosuit/nanosuit.fs");

    // Model
    Model nanosuit_model("assets/nanosuit/scene.gltf");

    while (!glfwWindowShouldClose(window)) {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        key_events();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Init lighting_shader
        nanosuit_shader.use();

        // Init MVP
        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

        glm::mat4 view = camera.view();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom()), RATIO, 0.1f, 100.0f);

        nanosuit_shader.set_uniform("model", glm::value_ptr(model));
        nanosuit_shader.set_uniform("view", glm::value_ptr(view));
        nanosuit_shader.set_uniform("projection", glm::value_ptr(projection));

        nanosuit_model.draw(nanosuit_shader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
