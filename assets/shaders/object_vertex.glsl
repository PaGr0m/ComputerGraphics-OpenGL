#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_pos;

out vec3 vs_position;
out vec3 vs_normal;
out vec2 vs_texture_pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    vs_position = vec3(u_model * vec4(in_position, 1.0));
    vs_normal = mat3(transpose(inverse(u_model))) * in_normal;
    vs_texture_pos = in_texture_pos;

    gl_Position = u_projection * u_view * u_model * vec4(in_position, 1.0);
}
