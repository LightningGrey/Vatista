#version 410

//layout (location = 0) in vec4 inColor;
layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outColor;
uniform sampler2D texSample;

void main() {
    //outColor = vec4(1,1,1,1);
    outColor = texture(texSample, inUV);
}