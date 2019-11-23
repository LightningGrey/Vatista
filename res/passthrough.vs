#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

//layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outUV;

uniform mat4 a_ModelViewProjection;

void main() {
	//change later for texture
	//outColor = vec4(1,1,1,1);
	outUV = inUV;

	gl_Position = a_ModelViewProjection * vec4(inPosition, 1);
}