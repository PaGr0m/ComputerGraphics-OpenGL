#version 430 core

layout (location = 0) in vec3 in_position;

out vec3 shader_texture_position;

uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    shader_texture_position = in_position;
    vec4 pos = u_projection * u_view * vec4(in_position, 1.0);

    gl_Position = pos.xyww;
}
