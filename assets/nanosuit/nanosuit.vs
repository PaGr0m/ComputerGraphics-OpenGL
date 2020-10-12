#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_coord;

out vec2 texture_coords;
out vec3 normal;
out vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    texture_coords = in_texture_coord;
    normal = mat3(transpose(inverse(model))) * in_normal;
    position = vec3(model * vec4(in_pos, 1.0));

    gl_Position = projection * view * model * vec4(in_pos, 1.0);
}
