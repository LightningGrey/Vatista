#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inPositionS;
layout (location = 4) in vec3 inNormalS;

layout (location = 0) out vec3 outWorldPos;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec4 lightSpacePos;

uniform mat4 a_ModelViewProjection;
uniform mat4 a_Model;
uniform mat4 a_ModelView;
uniform mat3 a_NormalMatrix;
uniform mat4 lightSpaceMatrix;
uniform float morphT;

void main() {
	vec3 lerpPos = mix(inPositionS, inPosition, morphT);
	vec3 lerpNorm = mix(inNormalS, inNormal, morphT);

	outNormal = a_NormalMatrix * lerpNorm;
	outUV = inUV;
	outWorldPos =  (a_Model * vec4(lerpPos, 1)).xyz;
	lightSpacePos = lightSpaceMatrix * vec4(inPosition, 1.0);
	gl_Position = a_ModelViewProjection * vec4(lerpPos,1);
}