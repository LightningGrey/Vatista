#version 440

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec2 inScreenCoords;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D s_CameraDepth; // Camera's depth buffer
layout(binding = 2) uniform sampler2D s_GNormal;     // The normal buffer

// The inverse of the camera's view-project matrix (clip->world)
uniform mat4 a_ViewProjectionInv;
// The position of the camera, in world space
uniform vec3 a_CameraPos;

// The light's position, in world space
uniform vec3  a_LightPos;
// The light's color
uniform vec3  a_LightColor;
// The attenuation factor for the light (1/dist)
uniform float a_LightAttenuation;
// This should really be a GBuffer parameter
uniform float a_MatShininess;

const vec3 HALF = vec3(0.5);
const vec3 DOUBLE = vec3(2.0);
// Unpacks a normal from the [0,1] range to the [-1, 1] range
vec3 UnpackNormal(vec3 rawNormal) {
	return (rawNormal - HALF) * DOUBLE;
}

// Calculates a world position from the main camera's depth buffer
vec4 GetWorldPos(vec2 uv) {
	// Get the depth buffer value at this pixel.    
	float zOverW = texture(s_CameraDepth, uv).r * 2 - 1; 
	// H is the viewport position at this pixel in the range -1 to 1.    
	vec4 currentPos = vec4(uv.xy * 2 - 1, zOverW, 1); 
	// Transform by the view-projection inverse.    
	vec4 D = a_ViewProjectionInv * currentPos; 
	// Divide by w to get the world position.    
	vec4 worldPos = D / D.w;
	return worldPos;
}

// Caluclate the blinn-phong factor
vec3 BlinnPhong(vec3 fragPos, vec3 fragNorm, vec3 lightPosition, vec3 lightColor, float lAttenuation) {
	// Determine the direction from the position to the light
	vec3 toLight = lightPosition - fragPos;

	// Determine the distance to the light (used for attenuation later)
	float distToLight = length(toLight);
	// Normalize our toLight vector
	toLight = normalize(toLight);

	// Determine the direction between the camera and the pixel
	vec3 viewDir = normalize(a_CameraPos - fragPos);

	// Calculate the halfway vector between the direction to the light and the direction to the eye
	vec3 halfDir = normalize(toLight + viewDir);

	// Our specular power is the angle between the the normal and the half vector, raised
	// to the power of the light's shininess
	float specPower = pow(max(dot(fragNorm, halfDir), 0.0), a_MatShininess);

	// Finally, we can calculate the actual specular factor
	vec3 specOut = specPower * lightColor;

	// Calculate our diffuse factor, this is essentially the angle between
	// the surface and the light
	float diffuseFactor = max(dot(fragNorm, toLight), 0);
	// Calculate our diffuse output
	vec3  diffuseOut = diffuseFactor * lightColor;

	// We will use a modified form of distance squared attenuation, which will avoid divide
	// by zero errors and allow us to control the light's attenuation via a uniform
	float attenuation = 1.0 / (1.0 + lAttenuation * pow(distToLight, 2));

	return attenuation * (diffuseOut + specOut);
}

void main() {
	// Extract the world position from the depth buffer
	vec4 worldPos = GetWorldPos(inUV);  
	// Extract our normal from the G Buffer
	vec3 worldNormal = UnpackNormal(texture(s_GNormal, inUV).rgb);

	// Calculate our lighting for this point light
	vec3 result = BlinnPhong(worldPos.xyz, worldNormal, a_LightPos, a_LightColor, a_LightAttenuation);

	// Output the result
	outColor = vec4(result, 1.0);
}