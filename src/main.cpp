#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "utils/shader.h"
#include "utils/camera.h"
#include "initials.h"

GLuint load_cubemap(const std::vector<std::string> &maps) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (GLuint i = 0; i < maps.size(); i++) {
        GLubyte *data = stbi_load(maps[i].c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << maps[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

static std::vector<float> metaballs_vertices;
static float edge_size = 0.03f;

void init_metaballs(GLuint &skyboxVBO, GLuint &skyboxVAO) {
    int TMP = 40;
    for (int i = -TMP; i < TMP; i++) {
        for (int j = -TMP; j < TMP; j++) {
            for (int k = -TMP; k < TMP; k++) {
                metaballs_vertices.push_back(float(i) * edge_size);
                metaballs_vertices.push_back(float(j) * edge_size);
                metaballs_vertices.push_back(float(k) * edge_size);
            }
        }
    }

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(metaballs_vertices[0]) * metaballs_vertices.size(), metaballs_vertices.data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
}

void init_edge_table_buffer() {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(edge_table), edge_table, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);
}

void init_triangle_table_buffer() {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(triangle_table), triangle_table, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, buffer);
}


int main() {
    GLFWwindow *window = init_opengl();
    ImGuiIO io = init_ImGui(window);

    // Buffers
    GLuint skybox_vbo, skybox_vao;
    init_buffers(skybox_vbo, skybox_vao);

    GLuint metaballs_vbo, metaballs_vao;
    init_metaballs(metaballs_vbo, metaballs_vao);

    // Cubemap
    GLuint cubemap_texture = load_cubemap(cube_textures);

    // Shader
    Shader skybox_shader(
            "assets/skybox/shaders/vertex.glsl",
            "assets/skybox/shaders/fragment.glsl"
    );
    skybox_shader.set_uniform("u_skybox", 0);

    Shader metaballs_shader(
            "assets/metaballs/shaders/vertex.glsl",
            "assets/metaballs/shaders/geometry.glsl",
            "assets/metaballs/shaders/fragment.glsl"
    );

    init_edge_table_buffer();
    init_triangle_table_buffer();

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
        ImGui::End();

        // Init MVP
        glm::mat4 model = glm::mat4(1);
        glm::mat4 view = camera.view();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), RATIO, 0.1f, 100.0f);

        // Metaballs
        metaballs_shader.use();
        metaballs_shader.set_uniform("u_model", glm::value_ptr(model));
        metaballs_shader.set_uniform("u_view", glm::value_ptr(view));
        metaballs_shader.set_uniform("u_projection", glm::value_ptr(projection));
        metaballs_shader.set_uniform("u_camera_pos", camera.position());
        glBindVertexArray(metaballs_vao);
        glDrawArrays(GL_POINTS, 0, metaballs_vertices.size() / 3);
        glBindVertexArray(0);

        // Init MVP

        // Skybox
        glDepthFunc(GL_LEQUAL);
        skybox_shader.use();
        view = glm::mat4(glm::mat3(camera.view()));
        skybox_shader.set_uniform("u_view", glm::value_ptr(view));
        skybox_shader.set_uniform("u_projection", glm::value_ptr(projection));

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
