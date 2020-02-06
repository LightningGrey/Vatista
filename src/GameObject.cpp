#include "GameObject.h"
#include <GLM\include\GLM\gtc\matrix_transform.hpp>

namespace Vatista {
	bool GameObject::collisionCheck(glm::vec3 x, glm::vec2 collider1, glm::vec3 y, glm::vec2 collider2)
	{
		//std::cout << "           obj1.z = " << obj1.z << std::endl;
		//std::cout << "                         obj2.z = " << obj2.z << std::endl;
		if (x.x < y.x + collider2.x && x.x + collider1.x > y.x&& x.y < y.y + collider2.y && x.y + collider1.y > y.y) {
			return true;
		}
		else
			return false;

	}

	void GameObject::Draw(const Vatista::Camera::Sptr& camera)
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

		//get stamina vector
		
		float vec = 100.0f;
		material->GetShader()->SetUniform("staminaValue", vec);
		float vec2 = 50.0f;

		material->GetShader()->SetUniform("staminaValueMax", vec2);


		glm::vec3 vecVert = glm::vec3(15, 0, 2);
		material->GetShader()->SetUniform("meshDimensions", vecVert);


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
}