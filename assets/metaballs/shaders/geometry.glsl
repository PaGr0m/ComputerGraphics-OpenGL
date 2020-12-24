#version 430 core
layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

layout(std430, binding = 1) buffer edge_table_buffer { int edge_table[]; };
layout(std430, binding = 2) buffer triangle_table_buffer { int triangle_table[256][16]; };

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

const float CUBE_SIZE = 0.05f;

out vec3 shader_normal;

float vertex_function(vec3 point) {
    vec3 metaball_center1 = vec3(0.0f, 0.0f, 0.0f);
    vec3 metaball_center2 = vec3(0.1f, -0.2f, 0.5f);
    vec3 metaball_center3 = vec3(-0.5f, 0.5f, -0.5f);

    vec3 delta1 = point - metaball_center1;
    vec3 delta2 = point - metaball_center2;
    vec3 delta3 = point - metaball_center3;

    float radius = 0.25f;
    float sum = (1 / dot(delta1, delta1)) + (1 / dot(delta2, delta2)) + (1 / dot(delta3, delta3));

    return radius * radius * sum - 1;
}

vec3 evaluate_normal(vec3 point) {
    return normalize(vec3(
        vertex_function(point + vec3(CUBE_SIZE / 8, 0, 0)) - vertex_function(point - vec3(CUBE_SIZE / 8, 0, 0)),
        vertex_function(point + vec3(0, CUBE_SIZE / 8, 0)) - vertex_function(point - vec3(0, CUBE_SIZE / 8, 0)),
        vertex_function(point + vec3(0, 0, CUBE_SIZE / 8)) - vertex_function(point - vec3(0, 0, CUBE_SIZE / 8))
    ));
}

vec3 lerp(vec3 point1, vec3 point2) {
    float coefficient = abs(vertex_function(point1)) / (abs(vertex_function(point1)) + abs(vertex_function(point2)));
    return point1 + (point2 - point1) * coefficient;
}

void main() {
    vec3 cube[8] = {
        gl_in[0].gl_Position.xyz + vec3(-CUBE_SIZE / 2, -CUBE_SIZE/ 2, -CUBE_SIZE/ 2),
        gl_in[0].gl_Position.xyz + vec3(CUBE_SIZE/ 2, -CUBE_SIZE/ 2, -CUBE_SIZE/ 2),
        gl_in[0].gl_Position.xyz + vec3(CUBE_SIZE/ 2, -CUBE_SIZE/ 2, CUBE_SIZE/ 2),
        gl_in[0].gl_Position.xyz + vec3(-CUBE_SIZE/ 2, -CUBE_SIZE/ 2, CUBE_SIZE/ 2),
        gl_in[0].gl_Position.xyz + vec3(-CUBE_SIZE/ 2, CUBE_SIZE/ 2, -CUBE_SIZE/ 2),
        gl_in[0].gl_Position.xyz + vec3(CUBE_SIZE/ 2, CUBE_SIZE/ 2, -CUBE_SIZE/ 2),
        gl_in[0].gl_Position.xyz + vec3(CUBE_SIZE/ 2, CUBE_SIZE/ 2, CUBE_SIZE/ 2),
        gl_in[0].gl_Position.xyz + vec3(-CUBE_SIZE/ 2, CUBE_SIZE/ 2, CUBE_SIZE/ 2)
    };

    int cubeBitMaskIndex = 0;
    if (vertex_function(cube[0]) < 0) cubeBitMaskIndex |= 1;
    if (vertex_function(cube[1]) < 0) cubeBitMaskIndex |= 2;
    if (vertex_function(cube[2]) < 0) cubeBitMaskIndex |= 4;
    if (vertex_function(cube[3]) < 0) cubeBitMaskIndex |= 8;
    if (vertex_function(cube[4]) < 0) cubeBitMaskIndex |= 16;
    if (vertex_function(cube[5]) < 0) cubeBitMaskIndex |= 32;
    if (vertex_function(cube[6]) < 0) cubeBitMaskIndex |= 64;
    if (vertex_function(cube[7]) < 0) cubeBitMaskIndex |= 128;

    vec3 vertexOnEdge[12];
    int edges = edge_table[cubeBitMaskIndex];
    if (edges == 0)          return;
    if ((edges & 1) != 0)    vertexOnEdge[0]  = lerp(cube[0], cube[1]);
    if ((edges & 2) != 0)    vertexOnEdge[1]  = lerp(cube[1], cube[2]);
    if ((edges & 4) != 0)    vertexOnEdge[2]  = lerp(cube[2], cube[3]);
    if ((edges & 8) != 0)    vertexOnEdge[3]  = lerp(cube[3], cube[0]);
    if ((edges & 16) != 0)   vertexOnEdge[4]  = lerp(cube[4], cube[5]);
    if ((edges & 32) != 0)   vertexOnEdge[5]  = lerp(cube[5], cube[6]);
    if ((edges & 64) != 0)   vertexOnEdge[6]  = lerp(cube[6], cube[7]);
    if ((edges & 128) != 0)  vertexOnEdge[7]  = lerp(cube[7], cube[4]);
    if ((edges & 256) != 0)  vertexOnEdge[8]  = lerp(cube[0], cube[4]);
    if ((edges & 512) != 0)  vertexOnEdge[9]  = lerp(cube[1], cube[5]);
    if ((edges & 1024) != 0) vertexOnEdge[10] = lerp(cube[2], cube[6]);
    if ((edges & 2048) != 0) vertexOnEdge[11] = lerp(cube[3], cube[7]);

    mat4 mvp = u_projection * u_view * u_model;
    for (int i = 0; triangle_table[cubeBitMaskIndex][i] != -1; i += 3) {
        gl_Position = mvp * vec4(vertexOnEdge[triangle_table[cubeBitMaskIndex][i]], 1);
        shader_normal = evaluate_normal(vertexOnEdge[triangle_table[cubeBitMaskIndex][i]]);
        EmitVertex();

        gl_Position = mvp * vec4(vertexOnEdge[triangle_table[cubeBitMaskIndex][i + 1]], 1);
        shader_normal = evaluate_normal(vertexOnEdge[triangle_table[cubeBitMaskIndex][i + 1]]);
        EmitVertex();

        gl_Position = mvp * vec4(vertexOnEdge[triangle_table[cubeBitMaskIndex][i + 2]], 1);
        shader_normal = evaluate_normal(vertexOnEdge[triangle_table[cubeBitMaskIndex][i + 2]]);
        EmitVertex();

        EndPrimitive();
    }
}
