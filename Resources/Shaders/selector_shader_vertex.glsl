#version 420 core

layout (location = 0) in vec3 aPosition;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

void main() {
    gl_Position = projMat * viewMat * modelMat * vec4(aPosition, 1.0);
}