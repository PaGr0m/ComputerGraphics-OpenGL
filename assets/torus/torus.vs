#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_texture_pos;

out vec3 vs_position;
out vec3 vs_normal;
out vec3 vs_texture_pos;

uniform mat4 u_projection;
uniform mat4 u_view;

void main() {
    vs_position = in_position;
    vs_normal = in_normal;
    vs_texture_pos = in_texture_pos;

    gl_Position = u_projection * u_view * vec4(in_position, 1.0);
}
