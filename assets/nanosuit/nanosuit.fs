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

void main()
{
    vec4 texel0, texel1, result_texture;
    texel0 = texture2D(texture_diffuse1, texture_coords);
    texel1 = texture2D(texture_specular1, texture_coords);
    result_texture = mix(texel0, texel1, coefficient_texture);

    vec3 refraction = refract(normalize(position - camera_pos),
                              normalize(normal), 1.0);
    vec3 reflection = reflect(normalize(position - camera_pos),
                              normalize(normal));

    FragColor = mix(
       mix(vec4(texture(skybox, refraction).rgb, 1.0),
           result_texture,
           coefficient_refraction),
       mix(vec4(texture(skybox, reflection).rgb, 1.0),
           result_texture,
           coefficient_reflection),
       0.5);
}
