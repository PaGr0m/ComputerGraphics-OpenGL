#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 position;
in vec2 texture_coords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 camera_pos;
uniform samplerCube skybox;

uniform float coefficient_texture;
uniform float coefficient_reflection;
uniform float coefficient_refraction;
uniform float fresnel_alpha;
uniform float frag_color_mix;

void main()
{
    // Texture
    vec4 texel_diffuse = texture2D(texture_diffuse1, texture_coords);
    vec4 texel_specular = texture2D(texture_specular1, texture_coords);
    vec4 result_texture = mix(texel_diffuse, texel_specular, coefficient_texture);

    // Reflect, Refract, Fresnel
    float angle = -dot(normalize(position - camera_pos), normalize(normal));
    vec3 reflection = reflect(normalize(position - camera_pos), normalize(normal));
    vec3 refraction = refract(normalize(position - camera_pos), normalize(normal), coefficient_refraction);
    float coefficient_fresnel = pow(1 - angle, fresnel_alpha) * (1 - coefficient_reflection) + coefficient_reflection;

    vec4 R = mix(
        vec4(texture(skybox, refraction).rgb, 1.0),
        vec4(texture(skybox, reflection).rgb, 1.0),
        coefficient_fresnel
    );

    FragColor = mix(R, result_texture, frag_color_mix);
}
