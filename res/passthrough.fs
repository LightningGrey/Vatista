#version 410

//layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 outUV;

layout (location = 0) out vec4 outColor;
uniform sampler2D texSample;

void main() {
    //outColor = vec4(1,1,1,1);
   outColor = texture(texSample, outUV);
}