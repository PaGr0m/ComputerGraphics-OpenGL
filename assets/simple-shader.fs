#version 330 core

out vec4 o_frag_color;

struct vx_output_t
{
    vec3 color;
    vec3 pos;
};
in vx_output_t v_out;


// Variables
uniform int u_max_iterations;
uniform float u_radius;
uniform float u_zoom;
uniform vec2 u_offset;
uniform vec2 u_resolution;
uniform vec2 u_shift_zoom;
uniform vec2 u_complex;
uniform sampler1D u_texture1D;
// ^^^ Variables


int mandelbrot(vec2 point);
vec2 complex_number_product(vec2 point);

int mandelbrot(vec2 point) {
    int iterations = 0;
    while (length(point) < u_radius && iterations < u_max_iterations) {
        point = complex_number_product(point) + u_complex;
        ++iterations;
    }

    return iterations;
}

vec2 complex_number_product(vec2 point) {
    return vec2(
        point.x * point.x - point.y * point.y,
        point.x * point.y + point.y * point.x
    );
}

void main() {
    vec2 point = u_zoom * v_out.pos.xy + u_offset;
    int iterations = mandelbrot(point);

    float coord = float(iterations) / float(u_max_iterations);
    o_frag_color = texture(u_texture1D, coord);
}
