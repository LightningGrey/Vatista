#version 440

layout (location = 0) in vec2 inUV;
layout (location = 1) in vec2 inScreenCoords;

layout (location = 0) out vec4 outColor;

uniform sampler2D mainImage;
uniform sampler2D blurImage;

void main() {
	vec3 result = texture(mainImage, inUV).rgb + texture(blurImage, inUV).rgb;  
	outColor = vec4(result, 1.0);
}