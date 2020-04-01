#pragma once
#include <GLM/glm.hpp>

struct Light {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct DirLight : Light {
	glm::vec3 direction;
};

struct PointLight : Light {
	glm::vec3 position;
};

struct SpotLight : Light {
	glm::vec3 position;
	glm::vec3 direction;
	float cutoff;
	float outerCutoff;

	float ambientPower;
	float shininess;
	float attenuation;
	glm::vec3 colour;
};