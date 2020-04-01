#version 440

layout (location = 0) in vec2 inUV;
layout (location = 1) in vec2 inScreenCoords;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 brightColor;

uniform sampler2D xImage;

void main() {
	vec3 color = pow(texture(xImage, inUV).rgb;

	outColor = vec4(color.rgb, color.a);
}