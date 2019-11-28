#include "GameObject.h"
#include <GLM\include\GLM\gtc\matrix_transform.hpp>

void GameObject::Draw(const Vatista::Camera::Sptr& camera, float time)
{
	glm::mat4 world = 
		glm::translate(glm::mat4(1.0f), Position) *
		glm::mat4_cast(glm::quat(glm::radians(EulerRotDeg))) *
		glm::scale(glm::mat4(1.0f), Scale);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(world)));

	Material->GetShader()->Bind();
	Material->Apply();
	Material->GetShader()->SetUniform("a_CameraPos", camera->GetPosition());

	// Update the MVP using the item's transform
	Material->GetShader()->SetUniform(
		"a_ModelViewProjection",
		camera->GetViewProjection() *
		world);

	// Update the model matrix to the item's world transform
	Material->GetShader()->SetUniform("a_Model", world);

	// Update the model matrix to the item's world transform
	Material->GetShader()->SetUniform("a_NormalMatrix", normalMatrix);


	float morph = time * speed;
	if (morph > 1.0f) {
		morph = 0.0f;
	}
	Material->GetShader()->SetUniform("morphT", morph);
	//Material->GetShader()->SetUniform("morphT", 0.5);

	// Draw the item
	
	Mesh->Draw();
}
