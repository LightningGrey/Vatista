#version 410

//S for start, E for end
layout (location = 0) in vec3 inPositionS;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormalS;
layout (location = 3) in vec3 inPositionE;
layout (location = 4) in vec3 inNormalE;

//layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec3 outNormal;

uniform mat4 a_ModelViewProjection;
uniform float morphT;

void main() {
	outUV = inUV;

	vec3 lerpPos = mix(inPositionS, inPositionE, morphT);
	vec3 lerpNorm = mix(inNormalS, inNormalE, morphT);

	outNormal = lerpNorm;

	gl_Position = a_ModelViewProjection * lerpPos;
}