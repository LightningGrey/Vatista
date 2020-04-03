#version 440
//intended to be used for colour correction but did not work

layout (location = 0) in vec2 inUV;
layout (location = 1) in vec2 inScreenCoords;

layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;

uniform sampler3D cool;
uniform sampler3D warm;
uniform sampler3D custom;

void main() {

	vec4 color = texture(xImage, inUV);

	vec4 lut = texture(cool, color.rgb);

	outColor = vec4(lut.rgb, 1);
}