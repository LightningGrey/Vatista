#include "GameObject.h"
#include <GLM\include\GLM\gtc\matrix_transform.hpp>

void GameObject::Draw(const Vatista::Camera::Sptr& camera)
{
	glm::mat4 world = 
		glm::translate(glm::mat4(1.0f), Position) *
		glm::mat4_cast(glm::quat(glm::radians(EulerRotDeg))) *
		glm::scale(glm::mat4(1.0f), Scale);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(world)));

	material->GetShader()->Bind();
	material->Apply();
	//material->GetShader()->SetUniform("texSample", 0);
	//material->GetShader()->SetUniform("normalMap", 1);
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


	currentTime += dt;

	if (currentTime > endTime) {
		dt = -0.0167f;
	}
	else if (currentTime < 0.0f) {
		dt = 0.0167f;
	}

	morph = currentTime / endTime;
	
	material->GetShader()->SetUniform("morphT", morph);

	// Draw the item
	
	model->Draw();
}
