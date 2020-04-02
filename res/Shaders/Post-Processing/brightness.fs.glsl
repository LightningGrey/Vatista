#version 440

layout (location = 0) in vec2 inUV;
layout (location = 1) in vec2 inScreenCoords;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 brightColor;

uniform sampler2D xImage;

void main() {
    vec3 sampled = texture2D(xImage, inUV).rgb;
    outColor = vec4(sampled, 1.0);
	// check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(sampled, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        brightColor = vec4(sampled, 1.0);
    else
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}