#version 430 core

uniform vec3 u_camera_pos;
uniform samplerCube u_skybox;

float u_refraction_value = 1000;

in vec3 shader_position;
in vec3 shader_normal;

out vec4 gl_FragColor;

float coefficient_refraction = 0.25f;
float coefficient_reflection = 0.75f;
float fresnel_alpha = 0.5f;

void main() {
    vec3 reflection = reflect(u_camera_pos, shader_normal);
    vec3 refraction = refract(u_camera_pos, shader_normal, 1 / coefficient_refraction);

    gl_FragColor = mix(
        vec4(texture(u_skybox, refraction).rgb, 1.0),
        vec4(texture(u_skybox, reflection).rgb, 1.0),
        0.2f
    );
}
