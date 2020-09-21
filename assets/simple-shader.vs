#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;

struct vx_output_t {
    vec3 color;
    vec3 pos;
};
out vx_output_t v_out;

void main() {
    v_out.color = in_color;
    v_out.pos = in_position;

    gl_Position = vec4(in_position.x, in_position.y, in_position.z, 1.0);
}
