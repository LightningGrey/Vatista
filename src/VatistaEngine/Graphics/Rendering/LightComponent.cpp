#include "LightComponent.h"

Vatista::Light::Light() : StationaryObj(), colour(glm::vec3(1.0f, 1.0f, 1.0f)),
	attenuation(1.0f)
{
}

Vatista::Light::~Light()
{
}

//horizontal back and forth transform
void Vatista::Light::transform()
{
	total += inc;
	this->setPosX(this->getPosX() + inc);

	if (total >= 2.0f || total <= -2.0f) {
		inc = -inc;
	}
}

void Vatista::Light::controller(Window* window)
{
	if (glfwGetKey(window->getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
		this->setPosX(this->getPosX() - 0.01);
	}
	else if (glfwGetKey(window->getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
		this->setPosX(this->getPosX() + 0.01);
	}
	else if (glfwGetKey(window->getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
		this->setPosZ(this->getPosZ() - 0.01);
	}
	else if (glfwGetKey(window->getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
		this->setPosZ(this->getPosZ() + 0.01);
	}
	else if (glfwGetKey(window->getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
		this->setPosZ(this->getPosY() + 0.01);
	}
	else if (glfwGetKey(window->getWindow(), GLFW_KEY_E) == GLFW_PRESS) {
		this->setPosZ(this->getPosY() - 0.01);
	}

	//
	else if (glfwGetKey(window->getWindow(), GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		//this->setPosZ(this->getPosY() - 0.01);
		//placeholder
	}        


}
