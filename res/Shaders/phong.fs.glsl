#version 410

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inWorldPos;

layout (location = 0) out vec4 outColor;

uniform vec3  a_CameraPos;

uniform vec3  a_AmbientColor;
uniform float a_AmbientPower;

uniform vec3  a_LightPos;
uniform vec3  a_LightColor;
uniform float a_LightShininess;

uniform vec4  a_Color;

void main() {
    // Re-normalize our input, so that it is always length 1
    vec3 norm = normalize(inNormal);
    // Determine the direction from the position to the light
    vec3 toLight = normalize(a_LightPos - inWorldPos);

    // Determine the direction between the camera and the pixel
    vec3 viewDir = normalize(a_CameraPos - inWorldPos);

    // Reflect the light to fragment around the face normal
    vec3 reflection = reflect(-toLight, norm);

    // Our specular power is the andle between the view direction and reflection, raised
    // to the power of the light's shininess
    float specPower = pow(max(dot(viewDir, reflection), 0.0), a_LightShininess);

    // Finally, we can calculate the actual specular factor
    vec3 specOut = specPower * a_LightColor;

    // Calculate our diffuse factor, this is essentially the angle between
    // the surface and the light
    float diffuseFactor = max(dot(norm, toLight), 0);
    // Calculate our diffuse output
    vec3  diffuseOut = diffuseFactor * a_LightColor;

    // Our ambient is simply the color times the ambient power
    vec3 ambientOut = a_AmbientColor * a_AmbientPower;
    
    // Our result is our lighting multiplied by our object's color
    vec3 result = (ambientOut + diffuseOut + specOut) * inColor.xyz;

    // Write the output
	outColor = vec4(result, inColor.a);// * a_ColorMultiplier;
}