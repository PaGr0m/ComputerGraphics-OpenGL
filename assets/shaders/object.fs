#version 330 core

out vec4 FragColor;

in vec3 vs_position;
in vec3 vs_normal;
in vec2 vs_texture_pos;

uniform sampler2D u_texture_1;
uniform vec3 camera_pos;

void main() {
    // Texture

    // Reflect, Refract, Fresnel

//     FragColor = texel_diffuse;
    FragColor = texture(u_texture_1, vs_texture_pos);
}
