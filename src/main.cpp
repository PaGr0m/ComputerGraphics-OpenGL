#define STB_IMAGE_IMPLEMENTATION

#include "shader.h"
#include "camera.h"
#include "initials.h"

int main() {
    GLFWwindow *window = init_opengl();

    int frame_width, frame_height;
    glfwGetFramebufferSize(window, &frame_width, &frame_height);

    // Buffers
    GLuint VBO, cubeVAO, lightVAO;
    init_buffers(VBO, cubeVAO, lightVAO);

    // Textures
    GLuint texture1, texture2;
    init_textures(texture1, texture2);

    // Shader
    Shader lighting_shader("assets/cube.vs", "assets/cube.fs");
    Shader lamp_shader("assets/lamp.vs", "assets/lamp.fs");

    // load textures (we now use a utility function to keep the code more organized)
    unsigned int diffuseMap = load_texture("assets/container.png");
    unsigned int specularMap = load_texture("assets/container_specular.png");

    // shader configuration
    // --------------------
    lighting_shader.use();
    lighting_shader.set_uniform("material.diffuse", 0);
    lighting_shader.set_uniform("material.specular", 1);

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
        lighting_shader.set_uniform("light.position", lightPos);
        lighting_shader.set_uniform("viewPos", camera.position());

        // Light properties
        lighting_shader.set_uniform("light.ambient", 0.2f, 0.2f, 0.2f);
        lighting_shader.set_uniform("light.diffuse", 0.5f, 0.5f, 0.5f);
        lighting_shader.set_uniform("light.specular", 1.0f, 1.0f, 1.0f);

        // Material properties
        lighting_shader.set_uniform("material.shininess", 64.0f);

        // Init MVP
        glm::mat4 model = glm::identity<glm::mat4>();
        glm::mat4 view = camera.view();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom()), RATIO, 0.1f, 100.0f);
        lighting_shader.set_uniform("model", glm::value_ptr(model));
        lighting_shader.set_uniform("view", glm::value_ptr(view));
        lighting_shader.set_uniform("projection", glm::value_ptr(projection));

        // Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // Bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // Render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Init lamp_shader
        model = glm::identity<glm::mat4>();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        lamp_shader.use();
        lamp_shader.set_uniform("model", glm::value_ptr(model));
        lamp_shader.set_uniform("view", glm::value_ptr(view));
        lamp_shader.set_uniform("projection", glm::value_ptr(projection));

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
