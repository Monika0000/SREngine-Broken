#version 420 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

void main() {
    vec3 FragPos = vec3(modelMat * vec4(aPosition, 1.0));

    gl_Position = projMat * viewMat * vec4(FragPos, 1.0);
}