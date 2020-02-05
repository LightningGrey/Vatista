#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;


layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec3 outNormal;






uniform mat4 a_ModelViewProjection;
uniform mat4 a_Model;
uniform mat4 a_ModelView;
uniform mat3 a_NormalMatrix;
uniform vec3 staminaValue;


	
void main() {
	outPosition = inPosition;
	outUV = inUV;
	outNormal = a_NormalMatrix * inNormal;

//	gl_Position = a_ModelViewProjection * vec4(inPosition, 1);
	
//	outPosition =  (a_Model * vec4(inPosition, 1)).xyz;


//staminaValue = mix( WHITE, RED, remap( 0.5, 1.0, u ) );
vec3 test = vec3(7.0f);
outPosition.x += test.x;

	//lerpPosition = mix( inPosition, vec3(), 0);
	//outPosition = vec3(a_Model * vec4(outPosition, 1));

	//outPosition.x += 0.3f;
	// = setScale(glm::vec3(1.0f))
gl_Position = vec4(outPosition, 1.0f);

}

//1 step lerp the scale of the bar
//
//this should actually be done in the fragment shader - what you can do is simply not draw
//pixels if their position along the bar (this can be easily achieved through an interpolated
//value) is greater than their health/maxHealth


//
//vec3 hsv2rgb(vec3 c)
//{
//    vec4 K = vec4(1.0, 0.6666666, 0.3333333, 3.0);
//    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
//}
//
//
//void mainImage( out vec4 fragColor, in vec2 fragCoord )
//{
//	vec2 border = vec2(0.05, 0.1);
//    vec4 backCol = vec4(1,1,1,1);
//    vec2 uv = fragCoord/iResolution.xy;
//    
//    // generate border mask
//	vec2 mask2 = step(border, uv) * step(uv, 1.0-border);
//    float mask = mask2.x*mask2.y;
//    
//    float progress = mod(iTime, 1.0);  
//    float blend = ((uv.x - progress) <= 0.0 ? 1.0 : 0.0) * mask;
//    vec4 foreCol = vec4(hsv2rgb(vec3(progress*0.33333 - 0.1, 1.0, 1.0)), 1.0);
//    fragColor = foreCol*blend + backCol*(1.0-blend);
//}
//

