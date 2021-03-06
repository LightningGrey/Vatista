#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
//layout (location = 3) in vec3 incolor;


layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec3 outNormal;
//layout (location = 3) out vec3 outcolor;






uniform mat4 a_ModelViewProjection;
uniform mat4 a_Model;
uniform mat4 a_ModelView;
uniform mat3 a_NormalMatrix;
uniform float staminaValue;
uniform float staminaValueMax;
//uniform vec3 meshDimensions;
	
void main() {
	outPosition = inPosition;
	outUV = inUV;
	outNormal = a_NormalMatrix * inNormal;

	
//	outPosition =  (a_Model * vec4(inPosition, 1)).xyz;


//staminaValue = mix( WHITE, RED, remap( 0.5, 1.0, u ) );
//vec3 test = vec3(7777777777777.0f);
//outPosition.x += test.x;

	//lerpPosition = mix( inPosition, vec3(), 0);
	//outPosition = vec3(a_Model * vec4(outPosition, 1));

	//outPosition.x += 0.3f;
	// = setScale(glm::vec3(1.0f))
//gl_Position = vec4(outPosition, 1.0f);

//
//vec3 temp = meshDimensions;
//temp.x = meshDimensions.x * (staminaValue/staminaValueMax);
//outPosition = temp;

//gl_Position = a_ModelViewProjection * vec4(temp, 1);
gl_Position = a_ModelViewProjection *  vec4(outPosition, 1);

}

