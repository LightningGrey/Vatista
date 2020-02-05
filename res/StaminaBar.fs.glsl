#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

layout (location = 0) out vec3 colourvalue;
uniform vec4 myOutputColor;


float remap( float minvalue, float maxvalue, float currentvalue )//remaping the values
{
    return ( currentvalue - minvalue ) / ( maxvalue - minvalue );
} 

const vec4 GREEN = vec4( 0.0, 1.0, 0.0, 1.0 );
const vec4 WHITE = vec4( 1.0, 1.0, 1.0, 1.0 );
const vec4 RED   = vec4( 1.0, 0.0, 0.0, 1.0 );


void main() {

	//outColor = inColor;


	 vec4 u = myOutputColor;
	
//
//u = clamp( u, 0.0, 1.0 );
//if( u < 0.5 )
//    myOutputColor = mix( GREEN, WHITE, remap( 0.0, 0.5, u ) );
//else
//    myOutputColor = mix( WHITE, RED, remap( 0.5, 1.0, u ) );
//

//if(true)
//{
//    discard;
//}

}

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

