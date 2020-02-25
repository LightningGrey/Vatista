#include "StationaryObj.h"
#include <GLM\include\GLM\gtc\matrix_transform.hpp>

namespace Vatista {
	StationaryObj::StationaryObj() : GameObject()
	{
	}

	StationaryObj::~StationaryObj()
	{
	}

	void Vatista::StationaryObj::Draw(const Vatista::Camera::Sptr& camera)
	{
		glm::mat4 world =
			glm::translate(glm::mat4(1.0f), Position) *
			glm::mat4_cast(glm::quat(glm::radians(EulerRotDeg))) *
			glm::scale(glm::mat4(1.0f), Scale);
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(world)));

		material->GetShader()->Bind();
		material->Apply();
		material->GetShader()->SetUniform("a_CameraPos", camera->GetPosition());

		// Update the MVP using the item's transform
		material->GetShader()->SetUniform(
			"a_ModelViewProjection",
			camera->GetViewProjection() *
			world);

		// Update the model matrix to the item's world transform
		material->GetShader()->SetUniform("a_Model", world);

		// Update the model matrix to the item's world transform
		material->GetShader()->SetUniform("a_NormalMatrix", normalMatrix);
		model->Draw();
	}
}