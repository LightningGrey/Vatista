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

		//dir light, move this to variable
		material->GetShader()->SetUniform("dirlight.direction", { -0.2f, -1.0f, -0.3f });
		material->GetShader()->SetUniform("dirlight.ambient", { 0.2f, 0.2f, 0.2f });
		material->GetShader()->SetUniform("dirlight.diffuse", { 0.5f, 0.5f, 0.5f });
		material->GetShader()->SetUniform("dirlight.specular", { 1.0f, 1.0f, 1.0f });

		material->GetShader()->SetUniform("spotlight.position", { 8.3f, 2.95f, -18.0f });
		material->GetShader()->SetUniform("spotlight.direction", { -0.4f, -0.05f, 1.0f });
		material->GetShader()->SetUniform("spotlight.ambientPower", 0.5f);
		material->GetShader()->SetUniform("spotlight.ambient", { 0.5f,  0.5f,  0.5f });
		material->GetShader()->SetUniform("spotlight.diffuse", { 1.0f, 1.0f, 1.0f });
		material->GetShader()->SetUniform("spotlight.specular", { 1.0f, 1.0f, 1.0f });
		material->GetShader()->SetUniform("spotlight.shininess", 256.0f);
		material->GetShader()->SetUniform("spotlight.cutoff", glm::cos(glm::radians(5.0f)));
		material->GetShader()->SetUniform("spotlight.outerCutoff", glm::cos(glm::radians(6.0f)));
		material->GetShader()->SetUniform("spotlight.attenuation", 0.04f);
		material->GetShader()->SetUniform("spotlight.colour", { 1.0f, 0.0f, 0.0f });

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