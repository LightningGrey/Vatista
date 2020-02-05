#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 outWorldPos;

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
}


