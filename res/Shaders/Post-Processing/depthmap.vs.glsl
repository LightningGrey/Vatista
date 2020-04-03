#version 440

layout (location = 0) in vec3 outpos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(outpos, 1.0);
}