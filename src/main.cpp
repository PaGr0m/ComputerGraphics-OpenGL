#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "buffers.h"


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

const GLuint WINDOW_WIDTH = 800;
const GLuint WINDOW_HEIGHT = 600;

void load_texture(const std::string &filepath) {
    int texture_x, texture_y, channels;

    unsigned char *data = stbi_load(filepath.c_str(), &texture_x, &texture_y, &channels, 0);
    if (data) {
        std::string extension = filepath.substr(filepath.length() - 3);
        if (extension == "png") {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_x, texture_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else if (extension == "jpg") {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_x, texture_y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << fmt::format("Failed to load texture: {}", filepath);
    }

    stbi_image_free(data);
}

void init_buffers(GLuint &cubeVBO, GLuint &cubeVAO) {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void init_textures(GLuint &texture1, GLuint &texture2) {
    // Texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    load_texture("assets/container.jpg");
    glBindTexture(GL_TEXTURE_2D, 0);

    // Texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    load_texture("assets/awesomeface.png");
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            "Lesson-1-window",
            nullptr,
            nullptr
    );

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int frame_width, frame_height;
    glfwGetFramebufferSize(window, &frame_width, &frame_height);
    glViewport(0, 0, frame_width, frame_height);

    // Buffers
    GLuint cubeVBO, cubeVAO;
    init_buffers(cubeVBO, cubeVAO);

    // Textures
    GLuint texture1, texture2;
    init_textures(texture1, texture2);

    glEnable(GL_DEPTH_TEST);

    // Shader
    Shader shader("assets/simple.vs", "assets/simple.fs");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        shader.set_uniform("ourTexture1", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        shader.set_uniform("ourTexture2", 1);

        glm::mat4 view = glm::identity<glm::mat4>();
        glm::mat4 projection = glm::identity<glm::mat4>();
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(45.0f,
                                      GLfloat(WINDOW_WIDTH) / GLfloat(WINDOW_HEIGHT),
                                      0.1f,
                                      100.0f);

        shader.set_uniform("view", glm::value_ptr(view));
        shader.set_uniform("projection", glm::value_ptr(projection));

        glBindVertexArray(cubeVAO);
        for (GLuint i = 0; i < 10; ++i) {
            glm::mat4 model = glm::identity<glm::mat4>();
            model = glm::translate(model, cubePositions[i]);

            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

            shader.set_uniform("model", glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
