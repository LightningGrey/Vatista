#version 440

layout (location = 0) in vec2 inUV;
layout (location = 1) in vec2 inScreenCoords;
layout (location = 0) out vec4 outColor;

uniform sampler2D xImage;
uniform float exposure;

void main() {
    const float gamma = 2.2;
    vec3 result = texture(xImage, inUV).rgb;
    result = pow(result, vec3(1.0/gamma));
    outColor = vec4(result, 1.0);

    ////tone map
    //vec3 mapped = vec3(1.0) - exp(-result * exposure);
    //mapped = pow(mapped, vec3(1.0/gamma));
    //////
    ////////// reinhard tone mapping
    ////////vec3 mapped = result / (result + vec3(1.0));
    ////////// gamma correction 
    ////////mapped = pow(mapped, vec3(1.0 / gamma));
    //////
    //outColor = vec4(mapped, 1.0);
}
