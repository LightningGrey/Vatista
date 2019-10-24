#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Vatista {
	Camera::Camera() :
		myPosition(glm::vec3(0)),
		myView(glm::mat4(1.0f)),
		Projection(glm::mat4(1.0f))
	{ }

	Camera::~Camera()
	{
	}

	void Camera::SetPosition(const glm::vec3& pos) {
		myView[3] = glm::vec4(-(glm::mat3(myView) * pos), 1.0f);
		myPosition = pos;
	}

	void Camera::LookAt(const glm::vec3& target, const glm::vec3& up) {
		myView = glm::lookAt(myPosition, target, up);
	}	void Camera::Rotate(const glm::quat& rot) {
		// Only update if we have an actual value to rotate by
		if (rot != glm::quat(glm::vec3(0))) {
			myView = glm::mat4_cast(rot) * myView;
		}
	}	void Camera::Move(const glm::vec3& local) {
		// Only update if we have actually moved
		if (local != glm::vec3(0)) {
			// We only need to subtract since we are already in the camera's local space
			myView[3] -= glm::vec4(local, 0);
			// Recalculate our position in world space and cache it
			myPosition = -glm::inverse(glm::mat3(myView)) * myView[3];
		}
	}
}