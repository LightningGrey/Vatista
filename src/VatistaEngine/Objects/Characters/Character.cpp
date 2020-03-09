#include "Character.h"
#include <GLM\include\GLM\gtc\matrix_transform.hpp>


namespace Vatista {
	Character::Character(bool ID, Vatista::Mesh::Sptr mesh, Vatista::Material::Sptr mat) : playerID(ID)
	{
		setMesh(mesh);
		setMat(mat);
		setCollider(0.74f, 1.78f);
		setStamina(100.0f);
		setAtk1Coll(glm::vec2(0.4f));
		setAtk2Coll(glm::vec2(0.4f));
		setLives(3);
		walls = 9.0f;
		for (int i = 0; i < 6; i++)
			animations.emplace_back();
		if (playerID) {
			setPos(13.0f, 6.0f, -10.0f);
			setRotY(90.0f);
			setAtk1Pos(glm::vec3(getPosX() + getCollX() + getAtk1CollX(), getPosY(), 9.0f));
			setAtk2Pos(glm::vec3(getPosX() + getCollX() + getAtk2CollX(), getPosY(), 9.0f));
		}
		else {
			setPos(30.0f, 6.0f, -10.0f);
			setRotY(-90.f);
			setAtk1Pos(glm::vec3(getPosX() - getCollX() - getAtk1CollX(), getPosY(), 9.0f));
			setAtk2Pos(glm::vec3(getPosX() - getCollX() - getAtk2CollX(), getPosY(), 9.0f));
		}
	}

	Character::~Character()
	{
	}

	void Character::Draw(const Vatista::Camera::Sptr& camera)
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