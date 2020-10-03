#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace {
    std::string read_shader_code(const std::string &filename) {
        std::stringstream file_stream;
        try {
            std::ifstream file(filename.c_str());
            file_stream << file.rdbuf();
            file.close();
        }
        catch (std::exception const &e) {
            std::cerr << "Error reading shader file: " << e.what() << std::endl;
        }
        return file_stream.str();
    }
}

Shader::Shader(const std::string &vertex_filepath, const std::string &fragment_filepath) {
    const auto vertex_code = read_shader_code(vertex_filepath);
    const auto fragment_code = read_shader_code(fragment_filepath);
    compile(vertex_code, fragment_code);
    link();
}

Shader::~Shader() = default;

void Shader::compile(const std::string &vertex_code, const std::string &fragment_code) {
    const char *vcode = vertex_code.c_str();
    vertex_id_ = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_id_, 1, &vcode, nullptr);
    glCompileShader(vertex_id_);

    const char *fcode = fragment_code.c_str();
    fragment_id_ = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id_, 1, &fcode, nullptr);
    glCompileShader(fragment_id_);
    check_compile_error();
}

void Shader::link() {
    program_id_ = glCreateProgram();
    glAttachShader(program_id_, vertex_id_);
    glAttachShader(program_id_, fragment_id_);
    glLinkProgram(program_id_);
    check_linking_error();
    glDeleteShader(vertex_id_);
    glDeleteShader(fragment_id_);
}

void Shader::use() const {
    glUseProgram(program_id_);
}

template<>
void Shader::set_uniform<int>(const std::string &name, int val) {
    glUniform1i(glGetUniformLocation(program_id_, name.c_str()), val);
}

template<>
void Shader::set_uniform<bool>(const std::string &name, bool val) {
    glUniform1i(glGetUniformLocation(program_id_, name.c_str()), val);
}

template<>
void Shader::set_uniform<float>(const std::string &name, float val) {
    glUniform1f(glGetUniformLocation(program_id_, name.c_str()), val);
}

template<>
void Shader::set_uniform<float>(const std::string &name, float val1, float val2) {
    glUniform2f(glGetUniformLocation(program_id_, name.c_str()), val1, val2);
}

template<>
void Shader::set_uniform<float>(const std::string &name, float val1, float val2, float val3) {
    glUniform3f(glGetUniformLocation(program_id_, name.c_str()), val1, val2, val3);
}

template<>
void Shader::set_uniform<float *>(const std::string &name, float *val) {
    glUniformMatrix4fv(glGetUniformLocation(program_id_, name.c_str()), 1, GL_FALSE, val);
}

void Shader::check_compile_error() const {
    int success;
    char infoLog[1024];
    glGetShaderiv(vertex_id_, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_id_, 1024, nullptr, infoLog);
        std::cerr << "Error compiling Vertex Shader:\n" << infoLog << std::endl;
    }
    glGetShaderiv(fragment_id_, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_id_, 1024, nullptr, infoLog);
        std::cerr << "Error compiling Fragment Shader:\n" << infoLog << std::endl;
    }
}

void Shader::check_linking_error() const {
    int success;
    char infoLog[1024];
    glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id_, 1024, nullptr, infoLog);
        std::cerr << "Error Linking Shader Program:\n" << infoLog << std::endl;
    }
}
