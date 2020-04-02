#version 440

layout (location = 0) in vec2 inUV;
layout (location = 1) in vec2 inScreenCoords;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

uniform bool isHorizontal;
uniform float weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216); 


void main() {
    vec2 off = 1.0 / textureSize(xImage, 0);
    if (isHorizontal) {
		off = off * vec2(1, 0);
	} else {
		off = off * vec2(0, 1);
	}

    vec4 result = texture(xImage, inUV) * weights[0];

	for(int i = 1; i < 5; ++i)
    {
       result += texture(xImage, inUV + off * i) * weights[i];
       result += texture(xImage, inUV - off * i) * weights[i];
     }

    outColor = vec4(result.rgb, 1.0);
}