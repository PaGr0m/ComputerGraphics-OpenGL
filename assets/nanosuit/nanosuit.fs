#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 position;
in vec2 texture_coords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 cameraPos;
uniform samplerCube skybox;

uniform float coefficient_texture;
uniform float coefficient_reflection;
uniform float coefficient_refraction;

void main()
{
    vec3 I = normalize(position - cameraPos);
    vec3 R = refract(I, normalize(normal), 1.0);
    vec3 Re = reflect(I, normalize(normal));


    vec4 texel0, texel1, resultColor;
    texel0 = texture2D(texture_diffuse1, texture_coords);
    texel1 = texture2D(texture_specular1, texture_coords);
    resultColor = mix(texel0, texel1, coefficient_texture);

    vec4 color_R = mix(
        vec4(texture(skybox, R).rgb, 1.0),
        resultColor,
        coefficient_refraction
    );

    vec4 color_Re = mix(
        vec4(texture(skybox, Re).rgb, 1.0),
        resultColor,
        coefficient_reflection
    );

    FragColor = mix(
       color_R,
        color_Re,
        0.5
    );
}
