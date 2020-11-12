#version 330 core

out vec4 FragColor;

struct VS {
    vec3 position;
    vec3 normal;
    vec3 texture_pos;
};
in VS vs;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
uniform sampler2D u_texture4;
uniform sampler2D u_texture_depth;

uniform float u_height_min;
uniform float u_height_max;
uniform float u_height_shift;

uniform mat4 u_light_view;
uniform mat4 u_light_projection;

uniform mat4 u_spotlight_view;
uniform mat4 u_spotlight_projection;
uniform float u_spotlight_cone;

// Light
float evaluate_light_coefficient(float diffuse) {
    vec4 light_position = u_light_projection * u_light_view * vec4(vs.position, 1.0);
    light_position = light_position / light_position.w;
    light_position = light_position / 2 + 0.5f;

    float bias = 0.005f;
    float shadow_coefficient = texture(u_texture_depth, light_position.xy).x < light_position.z - bias
                             ? 0.0f
                             : 1.0f;
    float illumination_level = 0.15f;
    float light_coefficient = min(shadow_coefficient, diffuse) + illumination_level;

    return light_coefficient;
}

// Spotlight
float evaluate_spotlight_coefficient() {
    vec4 spotlight_position = u_spotlight_projection * u_spotlight_view * vec4(vs.position, 1.0);

    spotlight_position = spotlight_position / spotlight_position.w;
    spotlight_position = spotlight_position / 2 + 0.5f;

    float spotlight = 0.0f;
    if (
        spotlight_position.x > 0 && spotlight_position.x < 1 &&
        spotlight_position.y > 0 && spotlight_position.y < 1 &&
        spotlight_position.z > 0 && spotlight_position.z < 1
    ) {
        spotlight = 0.4f;
    }

    return spotlight;
}

void main() {
    vec3 light_dir = normalize(vec3(1.0f, 1.0f, 1.0f));
    float light_diffuse = max(dot(vs.normal, light_dir), 0.0);

    vec3 color;
    float height = (vs.texture_pos.z - u_height_min) / (u_height_max - u_height_min);
    if (height > 0.9f)
        color = mix(
            texture(u_texture2, vs.texture_pos.xy).rgb,
            texture(u_texture3, vs.texture_pos.xy).rgb,
            0.5f
        );
    else if (height > 0.6f)
        color = mix(
            texture(u_texture3, vs.texture_pos.xy).rgb,
            texture(u_texture4, vs.texture_pos.xy).rgb,
            0.25f
        );
    else color = texture(u_texture4, vs.texture_pos.xy).rgb;

    float light_coefficient = evaluate_light_coefficient(light_diffuse);
    float spotlight = evaluate_spotlight_coefficient();

    FragColor = vec4(color * light_coefficient + spotlight, 1.0f);
}
