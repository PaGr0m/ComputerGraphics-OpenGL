#version 330 core

out vec4 FragColor;

in vec3 vs_position;
in vec3 vs_normal;
in vec3 vs_texture_pos;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
uniform sampler2D u_texture4;
uniform sampler2D u_texture_depth;

uniform float u_height_min;
uniform float u_height_max;
uniform float u_height_shift;

uniform mat4 u_light_projection;
uniform mat4 u_light_view;

void main() {
    vec3 light_dir = normalize(vec3(1.0f, 1.0f, 1.0f));
    float light_diffuse = max(dot(vs_normal, light_dir), 0.0);

    vec3 ambient = vec3(0.03f, 0.03f, 0.03f);

    vec4 pos = u_light_projection * u_light_view * vec4(vs_position, 1.0);
    pos = pos / pos.w;
    pos = pos / 2 + 0.5f;

    float shadow_coefficient = texture(u_texture_depth, pos.xy).x < pos.z - 0.005f
                             ? 0.0f
                             : 1.0f;

    vec3 color;
    color = texture(u_texture4, vs_texture_pos.xy).rgb;
//     if (abs(vs_texture_pos.z) > 1.1f) color = texture(u_texture1, vs_texture_pos.xy).rgb;
//     else if (abs(vs_texture_pos.z) > 0.5f) color = texture(u_texture2, vs_texture_pos.xy).rgb;
//     else if (abs(vs_texture_pos.z) > 0.2f) color = texture(u_texture3, vs_texture_pos.xy).rgb;
//     else color = texture(u_texture4, vs_texture_pos.xy).rgb;

    FragColor = vec4(color * (min(shadow_coefficient, light_diffuse) + 0.3f), 1.0f);

//     FragColor = vec4(1.0, 1.0f, 1.0f, 1.0f);
}
