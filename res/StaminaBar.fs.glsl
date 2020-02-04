#version 410


layout(location = 0) in vec4 inColor;//frag color stuff
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inWorldPos;
layout(location = 0) out vec4 myOutputColor;



float remap( float minvalue, float maxvalue, float currentvalue )//remaping the values
{
    return ( currentvalue - minvalue ) / ( maxvalue - minvalue );
} 

const vec4 GREEN = vec4( 0.0, 1.0, 0.0, 1.0 );
const vec4 WHITE = vec4( 1.0, 1.0, 1.0, 1.0 );
const vec4 RED   = vec4( 1.0, 0.0, 0.0, 1.0 );

uniform float staminaValue;

void main() {

	//outColor = inColor;


	float u = staminaValue;

u = clamp( u, 0.0, 1.0 );
if( u < 0.5 )
    myOutputColor = mix( GREEN, WHITE, remap( 0.0, 0.5, u ) );
else
    myOutputColor = mix( WHITE, RED, remap( 0.5, 1.0, u ) );

}


