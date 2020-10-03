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
    Shader lighting_shader("assets/cube.vs", "assets/cube.fs");
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
        lighting_shader.set_uniform("lightPos", lightPos);
        lighting_shader.set_uniform("viewPos", camera.position());

        // Light properties
        glm::vec3 lightColor = glm::vec3(0.7f,0.7f,0.7f );
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.25f);
        lighting_shader.set_uniform("light.diffuse", diffuseColor);
        lighting_shader.set_uniform("light.ambient", ambientColor);
        lighting_shader.set_uniform("light.specular", 1.0f, 1.0f, 1.0f);

        // Material properties
        lighting_shader.set_uniform("material.ambient", 1.0f, 0.5f, 0.31f);
        lighting_shader.set_uniform("material.diffuse", 1.0f, 0.5f, 0.31f);
        lighting_shader.set_uniform("material.specular", 0.5f, 0.5f, 0.5f);
        lighting_shader.set_uniform("material.shininess", 32.0f);

        // Init MVP
        glm::mat4 model = glm::identity<glm::mat4>();
        glm::mat4 view = camera.view();
        glm::mat4 projection = glm::perspective(camera.zoom(), RATIO, 0.1f, 100.0f);
        lighting_shader.set_uniform("model", glm::value_ptr(model));
        lighting_shader.set_uniform("view", glm::value_ptr(view));
        lighting_shader.set_uniform("projection", glm::value_ptr(projection));

        glBindVertexArray(containerVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &containerVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
