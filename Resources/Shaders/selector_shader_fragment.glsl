#version 330 core
out vec4 Color;
uniform vec3 color;

void main(void) {
    Color = vec4(color, 1);
}