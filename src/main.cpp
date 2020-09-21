#include <iostream>
#include <fmt/format.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "settings.h"
#include "imgui.h"
#include "opengl_shader.h"

#include "../bindings/imgui_impl_opengl3.h"
#include "../bindings/imgui_impl_glfw.h"
#include "../lib/stb_image.h"

static ImVec2 offset = ImVec2();
static float zoom = 1;

static void glfw_error_callback(int error, const char *description) {
    std::cerr << fmt::format("Glfw Error {}: {}\n", error, description);
}

void create_triangle(GLuint &vbo, GLuint &vao, GLuint &ebo) {
    float triangle_vertices[] = {
            -1.0f, -1.0f, 0.0f,     // position vertex 1
            1.0f, 0.0f, 0.0f,       // color vertex 1

            -1.0f, 1.0f, 0.0f,      // position vertex 2
            0.0f, 1.0f, 0.0f,       // color vertex 2

            1.0f, -1.0f, 0.0f,      // position vertex 3
            0.0f, 0.0f, 1.0f,       // color vertex 3

            1.0f, 1.0f, 0.0f,       // position vertex 4
            0.0f, 0.0f, 1.0f,       // color vertex 4
    };

    unsigned int triangle_indices[] = {
            0, 1, 2,    // triangle 1
            1, 2, 3,    // triangle 2
    };

    glGenVertexArrays(1, &vao); // vertex array object
    glGenBuffers(1, &vbo);      // vertex buffer object
    glGenBuffers(1, &ebo);      // element buffer object
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_indices), triangle_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void scroll_callback(GLFWwindow *window, double x_offset, double y_offset) {
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 display = io.DisplaySize;

    ImVec2 mouse_pos = ImGui::GetMousePos();
    mouse_pos.x = (mouse_pos.x / display.x * 2 - 1) * zoom;
    mouse_pos.y = (mouse_pos.y / display.y * 2 - 1) * zoom;

    zoom = y_offset > 0
           ? zoom / ZOOM_SCALE
           : zoom * ZOOM_SCALE;

    ImVec2 new_mouse_pos = ImGui::GetMousePos();
    new_mouse_pos.x = (new_mouse_pos.x / display.x * 2 - 1) * zoom;
    new_mouse_pos.y = (new_mouse_pos.y / display.y * 2 - 1) * zoom;

    offset.x -= new_mouse_pos.x - mouse_pos.x;
    offset.y += new_mouse_pos.y - mouse_pos.y;
}

void mouse_offset(ImGuiIO &io) {
    ImVec2 display = io.DisplaySize;

    ImVec2 delta = ImGui::GetMouseDragDelta();
    ImGui::ResetMouseDragDelta();

    offset.x -= delta.x / display.x;
    offset.y += delta.y / display.y;
}

void load_texture(const std::string &filename) {
    int texture_x, texture_y, channels;
    std::string texture_path = resources_dir + filename;

    unsigned char *data = stbi_load(texture_path.c_str(), &texture_x, &texture_y, &channels, 0);
    if (data) {
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, texture_x, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_1D);
    } else {
        std::cerr << fmt::format("Failed to load texture: {}", resources_dir);
    }

    stbi_image_free(data);
}

int main(int, char **) {
    const std::vector<const char *> textures = {
            "texture.png",
            "texture.png"
    };
    std::string current_texture_filename = textures[0];

    // Use GLFW to create a simple window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.3 + GLSL 330
    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(
            1280,
            720,
            "Main window",
            nullptr,
            nullptr
    );

    if (window == nullptr)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW, i.e. fill all possible function pointers for current OpenGL context
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize OpenGL loader!\n";
        return 1;
    }

    // create our geometries
    GLuint vbo, vao, ebo;
    create_triangle(vbo, vao, ebo);

    // init shader
    shader_t triangle_shader("simple-shader.vs", "simple-shader.fs");

    // Setup GUI context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();

    // Mouse events
    glfwSetScrollCallback(window, scroll_callback);

    // Texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_1D, texture);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    load_texture(current_texture_filename);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Get windows size
        ImVec2 display = io.DisplaySize;

        // Set viewport to fill the whole window area
        glViewport(0, 0, display.x, display.y);

        // Fill background with solid color
        glClearColor(0.30f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Mouse event
        mouse_offset(io);

        // GUI Variables
        static float radius = MAX_RADIUS / 2;
        static int iterations = MAX_ITERATIONS / 2;
        static int listbox_item_current = 0;
        static float complex_real = -0.025;
        static float complex_image = 0.65;

        // GUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Settings");

        ImGui::Text("Fractal");
        ImGui::SliderFloat("Radius", &radius, MIN_RADIUS, MAX_RADIUS);
        ImGui::SliderInt("Iterations", &iterations, MIN_ITERATIONS, MAX_ITERATIONS);
        ImGui::Spacing();

        ImGui::Text("Complex number");
        ImGui::SliderFloat("Real", &complex_real, MIN_COMPLEX_REAL, MAX_COMPLEX_REAL);
        ImGui::SliderFloat("Image", &complex_image, MIN_COMPLEX_IMAGE, MAX_COMPLEX_IMAGE);
        ImGui::Spacing();

        ImGui::Text("Textures");
        ImGui::Combo("Current texture", &listbox_item_current, textures.data(), textures.size());
        ImGui::End();

        // Load textures
        if (current_texture_filename != textures[listbox_item_current]) {
            current_texture_filename = textures[listbox_item_current];
            load_texture(current_texture_filename);
        }

        // Pass the parameters to the shader as uniforms
        triangle_shader.set_uniform("u_zoom", zoom);
        triangle_shader.set_uniform("u_radius", radius);
        triangle_shader.set_uniform("u_offset", offset.x, offset.y);
        triangle_shader.set_uniform("u_resolution", display.x, display.y);
        triangle_shader.set_uniform("u_max_iterations", iterations);
        triangle_shader.set_uniform("u_complex", complex_real, complex_image);
        triangle_shader.use(); // Bind triangle shader

        glBindTexture(GL_TEXTURE_1D, texture);

        // Bind vertex array = buffers + indices
        glBindVertexArray(vao);

        // Execute draw call
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // Generate gui render commands
        ImGui::Render();

        // Execute gui render commands using OpenGL backend
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap the backbuffer with the frontbuffer that is used for screen display
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
