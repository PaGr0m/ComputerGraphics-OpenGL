#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <optional>

class Shader {
public:
    Shader(const std::string &vertex_filepath, const std::string &fragment_filepath);

    Shader(const std::string &vertex_filepath,
           const std::string &geometry_filepath,
           const std::string &fragment_filepath);

    ~Shader();

    void use() const;

    template<typename T>
    void set_uniform(const std::string &name, T val);

    template<typename T>
    void set_uniform(const std::string &name, T val1, T val2);

    template<typename T>
    void set_uniform(const std::string &name, T val1, T val2, T val3);

private:
    void check_compile_error() const;
    void check_linking_error() const;
    void compile(const std::string &vertex_code, const std::string &fragment_code);
    void compile(const std::string &vertex_code, const std::string &geometry_code, const std::string &fragment_code);
    void link();

private:
    GLuint vertex_id_{};
    GLuint fragment_id_{};
    GLuint program_id_{};

    std::optional<GLuint> geometry_id_{};
};
