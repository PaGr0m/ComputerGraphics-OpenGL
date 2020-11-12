#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_texture_pos;

struct VS {
    vec3 position;
    vec3 normal;
    vec3 texture_pos;
};
out VS vs;

uniform mat4 u_projection;
uniform mat4 u_view;

void main() {
    vs.position = in_position;
    vs.normal = in_normal;
    vs.texture_pos = in_texture_pos;

    gl_Position = u_projection * u_view * vec4(in_position, 1.0);
}
