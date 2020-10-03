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

    // Load textures
    unsigned int diffuseMap = load_texture("assets/container.png");
    unsigned int specularMap = load_texture("assets/container_specular.png");

    // Shader configuration
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
        // be sure to activate shader when setting uniforms/drawing objects
        lighting_shader.use();
        lighting_shader.set_uniform("viewPos", camera.position());
        lighting_shader.set_uniform("material.shininess", 32.0f);

        // Directional light
        lighting_shader.set_uniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lighting_shader.set_uniform("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lighting_shader.set_uniform("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lighting_shader.set_uniform("dirLight.specular", 0.5f, 0.5f, 0.5f);

        lighting_shader.set_uniform("pointLights[0].position", pointLightPositions[0]);
        lighting_shader.set_uniform("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lighting_shader.set_uniform("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lighting_shader.set_uniform("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lighting_shader.set_uniform("pointLights[0].constant", 1.0f);
        lighting_shader.set_uniform("pointLights[0].linear", 0.09f);
        lighting_shader.set_uniform("pointLights[0].quadratic", 0.032f);

        lighting_shader.set_uniform("pointLights[1].position", pointLightPositions[1]);
        lighting_shader.set_uniform("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lighting_shader.set_uniform("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lighting_shader.set_uniform("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lighting_shader.set_uniform("pointLights[1].constant", 1.0f);
        lighting_shader.set_uniform("pointLights[1].linear", 0.09f);
        lighting_shader.set_uniform("pointLights[1].quadratic", 0.032f);

        lighting_shader.set_uniform("pointLights[2].position", pointLightPositions[2]);
        lighting_shader.set_uniform("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lighting_shader.set_uniform("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lighting_shader.set_uniform("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lighting_shader.set_uniform("pointLights[2].constant", 1.0f);
        lighting_shader.set_uniform("pointLights[2].linear", 0.09f);
        lighting_shader.set_uniform("pointLights[2].quadratic", 0.032f);

        lighting_shader.set_uniform("pointLights[3].position", pointLightPositions[3]);
        lighting_shader.set_uniform("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lighting_shader.set_uniform("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        lighting_shader.set_uniform("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lighting_shader.set_uniform("pointLights[3].constant", 1.0f);
        lighting_shader.set_uniform("pointLights[3].linear", 0.09f);
        lighting_shader.set_uniform("pointLights[3].quadratic", 0.032f);

        lighting_shader.set_uniform("spotLight.position", camera.position());
        lighting_shader.set_uniform("spotLight.direction", camera.front());
        lighting_shader.set_uniform("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lighting_shader.set_uniform("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lighting_shader.set_uniform("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lighting_shader.set_uniform("spotLight.constant", 1.0f);
        lighting_shader.set_uniform("spotLight.linear", 0.09f);
        lighting_shader.set_uniform("spotLight.quadratic", 0.032f);
        lighting_shader.set_uniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lighting_shader.set_uniform("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

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

        // Render containers
        glBindVertexArray(cubeVAO);
        for (auto i = 0; i < 10; i++) {
            float angle = 20.0f * i;
            model = glm::identity<glm::mat4>();
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            lighting_shader.set_uniform("model", glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Also draw the lamp object(s)
        lamp_shader.use();
        lamp_shader.set_uniform("model", glm::value_ptr(model));
        lamp_shader.set_uniform("view", glm::value_ptr(view));
        lamp_shader.set_uniform("projection", glm::value_ptr(projection));

        glBindVertexArray(lightVAO);
        for (auto &pointLightPosition : pointLightPositions) {
            model = glm::identity<glm::mat4>();
            model = glm::translate(model, pointLightPosition);
            model = glm::scale(model, glm::vec3(0.2f));
            lighting_shader.set_uniform("model", glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
