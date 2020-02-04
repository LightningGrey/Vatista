#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

layout (location = 1) out vec2 outUV;

layout (location = 0) out vec3 outPosition;

uniform mat4 a_Model;

void main() {
	outPosition = inPosition;
	outUV = inUV;
	gl_Position = a_Model * vec4(inPosition, 1);
	vec3 lerpPosition;
	
	outPosition = (x = -1,y, z);

	//lerpPosition = mix( inPosition, vec3(), 0);
}
//1 step lerp the scale of the bar
//



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

