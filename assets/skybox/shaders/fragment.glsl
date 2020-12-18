#version 430 core

out vec4 gl_FragColor;

in vec3 shader_texture_position;

uniform samplerCube u_skybox;

void main() {
    gl_FragColor = texture(u_skybox, shader_texture_position);
}
