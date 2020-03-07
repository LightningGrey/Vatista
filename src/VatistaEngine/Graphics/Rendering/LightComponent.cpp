#include "LightComponent.h"

Vatista::Light::Light() : StationaryObj(), colour(glm::vec3(1.0f, 1.0f, 1.0f)),
	attenuation(1.0f)
{
}

Vatista::Light::~Light()
{
}
