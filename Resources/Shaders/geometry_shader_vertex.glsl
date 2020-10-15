#version 420 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord0;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoord0;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Normal0;
} vs_out;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

void main() {
    vs_out.FragPos   = vec3(modelMat * vec4(aPosition, 1.0));
    vs_out.TexCoord0 = vec2(aTexCoord0.x, 1 - aTexCoord0.y); // flip the texture along the Y axis

    gl_Position = projMat * viewMat * vec4(vs_out.FragPos, 1.0);
}