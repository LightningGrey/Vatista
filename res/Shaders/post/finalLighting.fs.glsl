#version 440

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec2 inScreenCoords;

layout(location = 0) out vec4 outColor;

uniform sampler2D xImage;

layout(binding = 1) uniform sampler2D a_GColor;
layout(binding = 2) uniform sampler2D a_HdrLightAccum;

uniform float a_Exposure;

vec3 ToneMap(vec3 color, float exposure) {
	const float gamma = 2.2;
	vec3 result = vec3(1.0) - exp(-color * exposure);
	result = pow(result, vec3(1.0 / gamma));
	return result;
}

void main() {
	vec4 color = texture(a_GColor, inUV) * texture(a_HdrLightAccum, inUV);
	outColor = vec4(ToneMap(color.rgb, a_Exposure), 1.0);
}