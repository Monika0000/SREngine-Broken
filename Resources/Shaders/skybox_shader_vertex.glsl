#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform vec3 CamPos;

void main()
{
    TexCoords = aPos;
    vec4 pos = projMat * viewMat * vec4(aPos + CamPos, 1.0);
    gl_Position = pos.xyww;
}  