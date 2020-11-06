#version 330 core

//uniform ivec2 dim;

const vec2 triOffset[] = vec2[](
    vec2(0,0),
    vec2(0,1),
    vec2(1,1),
    vec2(0,0),
    vec2(1,1),
    vec2(1,0));

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat3 normalMat;

void main() {
    ivec2 dim = ivec2(1,1);

    int triVert = gl_VertexID % 6;
    int gridIndex = gl_VertexID / 6;
    vec2 coord = vec2(gridIndex / dim.x, gridIndex % dim.x);
    coord = (coord + triOffset[triVert]) / vec2(dim);
    vec4 esVert = viewMat * vec4(coord * 2.0 - 1.0, 0.0, 1.0);
    gl_Position = projMat * esVert;
}