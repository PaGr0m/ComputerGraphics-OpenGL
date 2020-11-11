#version 330 core

out vec4 FragColor;

in vec3 vs_position;
in vec3 vs_normal;
in vec2 vs_texture_pos;

uniform sampler2D u_texture_1;

void main() {
    FragColor = texture(u_texture_1, vs_texture_pos);
}
