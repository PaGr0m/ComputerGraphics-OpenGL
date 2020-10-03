#define STB_IMAGE_IMPLEMENTATION

#include "shader.h"
#include "camera.h"
#include "initials.h"

int main() {
    GLFWwindow *window = init_opengl();

    int frame_width, frame_height;
    glfwGetFramebufferSize(window, &frame_width, &frame_height);

    // Buffers
    GLuint VBO, containerVAO, lightVAO;
    init_buffers(VBO, containerVAO, lightVAO);

    // Textures
    GLuint texture1, texture2;
    init_textures(texture1, texture2);

    // Shader
    Shader lighting_shader("assets/lighting.vs", "assets/lighting.fs");
    Shader lamp_shader("assets/lamp.vs", "assets/lamp.fs");

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
        lighting_shader.use();
        lighting_shader.set_uniform("objectColor", 1.0f, 0.5f, 0.31f);
        lighting_shader.set_uniform("lightColor", 1.0f, 1.0f, 1.0f);
        lighting_shader.set_uniform("lightPos", lightPos.x, lightPos.y, lightPos.z);
        lighting_shader.set_uniform("viewPos", camera.position().x, camera.position().y, camera.position().z);

        // Init MVP
        glm::mat4 view = camera.view();
        glm::mat4 projection = glm::perspective(camera.zoom(), RATIO, 0.1f, 100.0f);
        lighting_shader.set_uniform("view", glm::value_ptr(view));
        lighting_shader.set_uniform("projection", glm::value_ptr(projection));

        glBindVertexArray(containerVAO);
        glm::mat4 model = glm::identity<glm::mat4>();
        lighting_shader.set_uniform("model", glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Init lamp_shader
        lamp_shader.use();
        lamp_shader.set_uniform("view", glm::value_ptr(view));
        lamp_shader.set_uniform("projection", glm::value_ptr(projection));

        model = glm::identity<glm::mat4>();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        lamp_shader.set_uniform("model", glm::value_ptr(model));

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &containerVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
