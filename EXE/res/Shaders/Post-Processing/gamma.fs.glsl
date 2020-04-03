#version 440

layout (location = 0) in vec2 inUV;
layout (location = 1) in vec2 inScreenCoords;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

void main() {
	float gamma = 2.2;
	vec4 color = pow(texture(xImage, inUV), vec4(gamma));

	outColor = vec4(color.rgb, color.a);
}