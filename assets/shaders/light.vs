#version 330 core

layout (location = 0) in vec3 in_pos;

uniform mat4 u_projection;
uniform mat4 u_view;

void main() {
    gl_Position = u_projection * u_view * vec4(in_pos, 1.0);
}
