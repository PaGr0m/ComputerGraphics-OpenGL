#pragma once

#include <iostream>
#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "buffers.h"

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

bool keys[1024];
double lastX = 400, lastY = 300;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void key_events() {
    if (keys[GLFW_KEY_W]) camera.keyboard_events(Camera::Movement::FORWARD, deltaTime);
    if (keys[GLFW_KEY_S]) camera.keyboard_events(Camera::Movement::BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A]) camera.keyboard_events(Camera::Movement::LEFT, deltaTime);
    if (keys[GLFW_KEY_D]) camera.keyboard_events(Camera::Movement::RIGHT, deltaTime);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

void mouse_callback(GLFWwindow *window, double position_x, double position_y) {
    GLfloat offset_x = position_x - lastX;
    GLfloat offset_y = lastY - position_y;

    glfwGetCursorPos(window, &lastX, &lastY);

    camera.process_mouse_movement(offset_x, offset_y);
}

void scroll_callback(GLFWwindow *window, double offset_x, double offset_y) {
    camera.process_mouse_scroll(offset_y);
}

unsigned int load_texture(const std::string &filepath) {
//    int texture_x, texture_y, channels;
//
//    unsigned char *data = stbi_load(filepath.c_str(), &texture_x, &texture_y, &channels, 0);
//    if (data) {
//        std::string extension = filepath.substr(filepath.length() - 3);
//        if (extension == "png") {
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_x, texture_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//        } else if (extension == "jpg") {
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_x, texture_y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//        }
//        glGenerateMipmap(GL_TEXTURE_2D);
//    } else {
//        std::cerr << fmt::format("Failed to load texture: {}", filepath);
//    }
//
//    stbi_image_free(data);
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filepath << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void init_buffers(GLuint &VBO, GLuint &cubeVAO, GLuint &lightVAO) {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(0);
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

GLFWwindow *init_opengl() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            "Lesson-8-texture-map",
            nullptr,
            nullptr
    );

    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    int frame_width, frame_height;
    glfwGetFramebufferSize(window, &frame_width, &frame_height);
    glViewport(0, 0, frame_width, frame_height);
    glEnable(GL_DEPTH_TEST);

    return window;
}
