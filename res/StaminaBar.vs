#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

layout (location = 1) out vec2 outUV;

uniform mat4 a_Model;

void main() {
	
	outUV = inUV;
	gl_Position = a_Model * vec4(inPosition, 1);
}





