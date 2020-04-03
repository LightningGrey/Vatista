#version 410
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;

    float ambientPower;
    float shininess;      
    float attenuation;
    vec3 colour;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
 
};


layout (location = 0) in vec3 inWorldPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 tangentLightPos;
layout (location = 4) in vec3 tangentViewPos;
layout (location = 5) in vec3 tangentFragPos;


layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 brightColor;

uniform vec3  a_CameraPos;

uniform vec3  a_AmbientColor;
uniform float a_AmbientPower;

uniform vec3  a_LightPos;
uniform vec3  a_LightColor;
uniform float a_LightShininess;
uniform float a_LightAttenuation;
uniform sampler2D texSample;
uniform sampler2D normalMap;

uniform int rimOn;


uniform DirLight dirlight;
uniform SpotLight spotlight;


vec3 directionLightCalc(DirLight dirlight, vec4 surfaceColour, vec3 norm, vec3 halfDir)
{
    vec3 lightDir = normalize(-dirlight.direction);

    float diffuse = max(dot(norm, lightDir), 0.0);
    vec3 diffuseOut = dirlight.diffuse * diffuse * a_LightColor;

    // Our specular power is the angle between the the normal and the half vector, raised
    // to the power of the light's shininess
    float specular = pow(max(dot(norm, halfDir), 0.0), a_LightShininess);
    vec3 specOut = dirlight.specular * specular * a_LightColor;

    // Our ambient is simply the color times the ambient power
    vec3 ambientOut = dirlight.ambient * a_AmbientColor * a_AmbientPower;

    // Our result is our lighting multiplied by our object's color
    vec3 result = (ambientOut * surfaceColour.rgb) + (diffuseOut * surfaceColour.rgb) 
        + (specOut * surfaceColour.rgb);
    return result;

}


vec3 pointLightCalc(vec4 surfaceColour, vec3 norm, vec3 toLight, vec3 viewDir, vec3 halfDir, float distToLight)
{

    // Our specular power is the angle between the the normal and the half vector, raised
    // to the power of the light's shininess
    float specPower = pow(max(dot(norm, halfDir), 0.0), a_LightShininess);

    // Finally, we can calculate the actual specular factor
    vec3 specOut = specPower * a_LightColor;

    ////rim lighting
	float rimLight = 1 - dot(norm, viewDir);
	rimLight = clamp(rimLight - 0.4, 0.0, 1.0);
    rimLight = rimLight * rimOn;

    // Calculate our diffuse factor, this is essentially the angle between
    // the surface and the light
    float diffuseFactor = max(dot(norm, toLight), 0);
    // Calculate our diffuse output
    vec3  diffuseOut = diffuseFactor * a_LightColor;
    diffuseOut += rimLight * vec3(0.0f,0.0f,1.0f);
	
    // Our ambient is simply the color times the ambient power
    vec3 ambientOut = a_AmbientColor * a_AmbientPower;

    // We will use a modified form of distance squared attenuation, which will avoid divide
    // by zero errors and allow us to control the light's attenuation via a uniform
    float attenuation = 1.0 / (1.0 + a_LightAttenuation * pow(distToLight, 2));
    
    // Our result is our lighting multiplied by our object's color
    vec3 result = (ambientOut + attenuation * (diffuseOut + specOut)) * surfaceColour.rgb;
    return result;
}

vec3 spotLightCalc(SpotLight spotlight, vec4 surfaceColour, vec3 norm, vec3 toLight, float distToLight)
{

    // Determine the direction between the camera and the pixel
    vec3 viewDir = normalize(spotlight.position - inWorldPos);

    vec3 halfDir = normalize(toLight + viewDir);

    // Our specular power is the angle between the the normal and the half vector, raised
    // to the power of the light's shininess
    float specPower = pow(max(dot(norm, halfDir), 0.0), spotlight.shininess);

    // Finally, we can calculate the actual specular factor
    vec3 specOut = specPower * spotlight.specular * spotlight.colour;

    ////rim lighting
	//float rimLight = 1 - dot(norm, viewDir);
	//rimLight = clamp(rimLight - 0.4, 0.0, 1.0);
    //rimLight = rimLight * rimOn;

    // Calculate our diffuse factor, this is essentially the angle between
    // the surface and the light
    float diffuseFactor = max(dot(norm, toLight), 0);
    // Calculate our diffuse output
    vec3  diffuseOut = diffuseFactor * spotlight.diffuse * spotlight.colour;
    //diffuseOut += rimLight * vec3(1.0,0.0,0.0);

    // Our ambient is simply the color times the ambient power
    vec3 ambientOut = spotlight.ambientPower * spotlight.ambient * spotlight.colour;

    // We will use a modified form of distance squared attenuation, which will avoid divide
    // by zero errors and allow us to control the light's attenuation via a uniform
    float attenuation = 1.0 / (1.0 + spotlight.attenuation * pow(distToLight, 2));

    //spotlight intensity
    float theta = dot(viewDir, normalize(-spotlight.direction));
    float epsilon = spotlight.cutoff - spotlight.outerCutoff;
    float intensity = clamp ((theta - spotlight.outerCutoff) / epsilon, 
        0.0, 1.0);

    ambientOut *= intensity;
    diffuseOut *= intensity;
    specOut *= intensity;
    
    // Our result is our lighting multiplied by our object's color
    vec3 result = (ambientOut + attenuation * (diffuseOut + specOut)) *  surfaceColour.rgb;
       //* vec3(1.0, 0.0, 0.0);
    return result;
}



void main() {
    // Re-normalize our input, so that it is always length 1
    //vec3 norm = normalize(inNormal);

    vec3 norm = texture(normalMap, inUV).rgb;
    // transform normal vector to range [-1,1]
    norm = normalize(norm * 2.0 - 1.0);   

	vec4 surfaceColour = texture(texSample, inUV); //* texture(texSample2, inUV);

       // Determine the direction from the position to the light
    vec3 toLight = tangentLightPos - tangentFragPos;
    // Determine the distance to the light (used for attenuation later)
    float distToLight = length(toLight);
    // Normalize our toLight vector
    toLight = normalize(toLight);

    // Determine the direction between the camera and the pixel
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);

    vec3 halfDir = normalize(toLight + viewDir);

    //direction light
    vec3 result = directionLightCalc(dirlight, surfaceColour, norm, halfDir);
    //point light
    result += pointLightCalc(surfaceColour, norm, toLight, viewDir, halfDir, distToLight);
    //spot light
    result += spotLightCalc(spotlight, surfaceColour, norm, toLight, distToLight);

    // TODO: gamma correction

    // Write the output
	outColor = vec4(result, surfaceColour.a);// * a_ColorMultiplier;

	float brightness = dot(outColor.rgb, vec3(0.7152, 0.0, 0.7152));
    if(brightness > 1.0)
        brightColor = vec4(outColor.rgb, 1.0);
    else
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
