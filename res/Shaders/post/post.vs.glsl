#version 410
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inUV;

layout (location = 0) out vec2 outUV;
layout (location = 1) out vec2 outScreenCoords;

uniform ivec2 screenRes;

void main() {
	gl_Position = vec4(inPosition, 0, 1);

	outUV = inUV;
}