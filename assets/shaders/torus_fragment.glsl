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
float evaluate_spotlight_coefficient(float diffuse) {
    vec4 spotlight_position = u_spotlight_projection * u_spotlight_view * vec4(vs.position, 1.0);
    spotlight_position = spotlight_position / spotlight_position.w;
    spotlight_position = spotlight_position / 2 + 0.5f;

    //    float theta = dot(light_dir, normalize(-vec3(0.1f, 0.1f, 0.1f)));
    //    float spotlight = texture(u_texture_depth, spotlight_position.xy).x < spotlight_position.z
    float spotlight = spotlight_position.x < spotlight_position.z
                    ? 0.0f
                    : 0.4f;
//    float illumination_level = 0.15f;
//    float spotlight = min(spotlight_coefficient, diffuse) + illumination_level;

    return spotlight;
}

void main() {
    vec3 light_dir = normalize(vec3(1.0f, 1.0f, 1.0f));
    float light_diffuse = max(dot(vs.normal, light_dir), 0.0);

    vec3 color;
     color = texture(u_texture3, vs.texture_pos.xy).rgb;
    if (abs(vs.texture_pos.z) > 1.1f) color = texture(u_texture1, vs.texture_pos.xy).rgb;
//    else if (abs(vs.texture_pos.z) > 0.5f) color = texture(u_texture2, vs.texture_pos.xsy).rgb;
//    else if (abs(vs.texture_pos.z) > 0.2f) color = texture(u_texture3, vs.texture_pos.xy).rgb;
//    else color = texture(u_texture4, vs.texture_pos.xy).rgb;

    float light_coefficient = evaluate_light_coefficient(light_diffuse);
    float spotlight_coefficient = evaluate_spotlight_coefficient(light_diffuse);

    FragColor = vec4(color * light_coefficient + spotlight_coefficient, 1.0f);
//    FragColor = vec4(color * light_coefficient, 1.0f);
}
