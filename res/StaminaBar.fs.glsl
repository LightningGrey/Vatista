#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
//layout (location = 3) in vec3 incolor;



layout (location = 0) out vec4 colourvalue;
uniform vec4 myOutputColor;
uniform vec3 UVoffset;
uniform sampler2D texSample;

void main() {

	//outColor = inColor;


	vec2 offset = vec2(UVoffset.x, UVoffset.y);//
	

	vec4 surfaceColour = texture(texSample, inUV + offset);//pushing the UVs over of the texture
	
	colourvalue = surfaceColour;

	colourvalue.a = int(colourvalue.x>0);//so colourvalue.x>0 returns ether 1 or 0 1 being true and 0 being false
	//then the colour value will be ether green or cleared (if it was black it got turned transparent) 
	//depending on if its true or false

	//colourvalue.a = colourvalue.x;
}