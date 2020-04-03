#version 440

layout (location = 0) in vec2 inUV;
layout (location = 1) in vec2 inScreenCoords;

layout (location = 0) out vec4 outColor;

uniform sampler2D mainImage;
uniform sampler2D blurImage;
uniform float exposure;

void main() {
	const float gamma = 2.2;
	vec3 result = texture(mainImage, inUV).rgb + texture(blurImage, inUV).rgb;
	
	 // tone mapping
    result = vec3(1.0) - exp(result * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
	outColor = vec4(result, 1.0);
}