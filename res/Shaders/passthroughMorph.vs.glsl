#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inPositionS;
layout (location = 4) in vec3 inNormalS;
layout (location = 5) in vec3 inTangent;
layout (location = 6) in vec3 inTangentS;

layout (location = 0) out vec3 outWorldPos;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec3 tangentLightPos;
layout (location = 4) out vec3 tangentViewPos;
layout (location = 5) out vec3 tangentFragPos;
layout (location = 6) out vec4 lightSpacePos;

uniform mat4 a_ModelViewProjection;
uniform mat4 a_Model;
uniform mat4 a_ModelView;
uniform mat3 a_NormalMatrix;
uniform mat4 lightSpaceMatrix;
uniform float morphT;

uniform vec3  a_LightPos;
uniform vec3  a_CameraPos;

void main() {
	//morphT = 0.5f;
	vec3 lerpPos = mix(inPositionS, inPosition, morphT);
	vec3 lerpNorm = mix(inNormalS, inNormal, morphT);
	vec3 lerpTangent = mix(inTangentS, inTangent, morphT);
	
	vec3 T = normalize(a_NormalMatrix * lerpTangent);
	vec3 N = normalize(a_NormalMatrix * lerpNorm);
	T = normalize(T - dot(T,N) * N);
	vec3 B = cross(N, T);
	
		

	outNormal = a_NormalMatrix * lerpNorm;
	outUV = inUV;
	outWorldPos =  (a_Model * vec4(lerpPos, 1)).xyz;
	lightSpacePos = lightSpaceMatrix * vec4(lerpPos, 1.0);
	gl_Position = a_ModelViewProjection * vec4(lerpPos,1);

	mat3 TBN = transpose(mat3(T, B, N));
	tangentLightPos = TBN * a_LightPos;
	tangentViewPos = TBN * a_CameraPos;
	tangentFragPos = TBN * outWorldPos;

}
