#include "Character.h"
#include <GLM\include\GLM\gtc\matrix_transform.hpp>


namespace Vatista {
	keyboard Character::kb;
	std::queue<std::pair<int, int>> Character::inputQueue;
	bool Character::roundEnd = false;

	Character::~Character()
	{
	}


	void Character::setStateTracker(int i)
	{
		if (getStateTracker() != i) {
			stateTracker = i;
			animIndex = 0;
			setMesh(animations[i].second[0]);
			endTime = animations[i].first[0];
		}
	}

	void Character::updateAnim()
	{
		if (currentTime >= endTime && getStateTracker() != 0) {
			if (getAnimIndex() + 1 < animations[getStateTracker()].second.size())
				animIndex++; 
			else
				animIndex = 0;
			setMesh(animations[getStateTracker()].second[getAnimIndex()]);
			currentTime = 0;
			endTime = animations[getStateTracker()].first[getAnimIndex()];
		}
	}

	void Character::Draw(const Vatista::Camera::Sptr& camera, std::vector<bool> toggles, float brightness)
	{
		glm::mat4 world =
			glm::translate(glm::mat4(1.0f), Position) *
			glm::mat4_cast(glm::quat(glm::radians(EulerRotDeg))) *
			glm::scale(glm::mat4(1.0f), Scale);
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(world)));

		material->GetShader()->Bind();
		material->Apply();

		material->GetShader()->SetUniform("ambientOn", toggles[0]);
		material->GetShader()->SetUniform("diffuseOn", toggles[1]);
		material->GetShader()->SetUniform("specularOn", toggles[2]);
		material->GetShader()->SetUniform("directionLightOn", toggles[3]);
		material->GetShader()->SetUniform("pointLightOn", toggles[4]);
		material->GetShader()->SetUniform("spotLightOn", toggles[5]);
		material->GetShader()->SetUniform("textureOn", toggles[6]);
		material->GetShader()->SetUniform("rimOn", toggles[7]);
		material->GetShader()->SetUniform("blank", 99);
		material->GetShader()->SetUniform("brightValue", brightness);

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
		material->GetShader()->SetUniform("spotlight.colour", { 1.0f, 0.83922f, 0.66667f });
		//material->GetShader()->SetUniform("spotlight.colour", { 1.0f, 1.0f, 1.0f });

		material->GetShader()->SetUniform("a_CameraPos", camera->GetPosition());

		normalMap->bind(2);
		material->GetShader()->SetUniform("normalMap", 2);


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

	void Character::depthdraw(Shader::Sptr shader)
	{
		shader->SetUniform("model", transform);
		model->Draw();
	}

	void Character::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		inputQueue.emplace();
		inputQueue.back().first = action;
		inputQueue.back().second = key;

	}

	void Character::inputBuffer()
	{
		if (inputQueue.empty())
			return;
		if (inputQueue.front().first == GLFW_PRESS) {
			switch (inputQueue.front().second) {
			case GLFW_KEY_A:
				if (kb.doubleTap1 && kb.tap1 == inputQueue.front().second && glfwGetTime() - kb.atkTimer1 > 0.8f) {
					//std::cout << "dash" << std::endl;
					kb.dash1 = true;
					kb.doubleTap1 = false;
				}
				else {
					kb.tapTimer1 = glfwGetTime();
					kb.doubleTap1 = true;
					kb.tap1 = inputQueue.front().second;
				}
				kb.a = true;
				break;
			case GLFW_KEY_D:
				if (kb.doubleTap1 && kb.tap1 == inputQueue.front().second && glfwGetTime() - kb.atkTimer1 > 0.8f) {
					//std::cout << "dash" << std::endl;
					kb.dash1 = true;
					kb.doubleTap1 = false;
				}
				else {
					kb.tapTimer1 = glfwGetTime();
					kb.doubleTap1 = true;
					kb.tap1 = inputQueue.front().second;
				}
				kb.d = true;
				break;
			case GLFW_KEY_LEFT:
				if (kb.doubleTap2 && kb.tap2 == inputQueue.front().second && glfwGetTime() - kb.atkTimer2 > 0.8f) {
					//std::cout << "dash" << std::endl;
					kb.dash2 = true;
					kb.doubleTap2 = false;
				}
				else {
					kb.tapTimer2 = glfwGetTime();
					kb.doubleTap2 = true;
					kb.tap2 = inputQueue.front().second;
				}
				kb.left = true;
				break;
			case GLFW_KEY_RIGHT:
				if (kb.doubleTap2 && kb.tap2 == inputQueue.front().second && glfwGetTime() - kb.atkTimer2 > 0.8f) {
					//std::cout << "dash" << std::endl;
					kb.dash2 = true;
					kb.doubleTap2 = false;
				}
				else {
					kb.tapTimer2 = glfwGetTime();
					kb.doubleTap2 = true;
					kb.tap2 = inputQueue.front().second;
				}
				kb.right = true;
				break;
			case GLFW_KEY_F:
				//std::cout << "attack1" << std::endl;
				kb.f = true;
				kb.g = false; break;
			case GLFW_KEY_G:
				kb.g = true;
				kb.f = false; break;
			case GLFW_KEY_RIGHT_CONTROL:
				//std::cout << "attack2" << std::endl;
				kb.rctrl = true;
				kb.ralt = false; break;
			case GLFW_KEY_RIGHT_ALT:
				kb.ralt = true;
				kb.rctrl = false; break;
			}
		}
		if (inputQueue.front().first == GLFW_RELEASE) {
			switch (inputQueue.front().second) {
			case GLFW_KEY_A: kb.a = false; if (kb.dash1) { kb.dash1 = false; } break;
			case GLFW_KEY_D: kb.d = false; if (kb.dash1) { kb.dash1 = false; } break;
			case GLFW_KEY_LEFT: kb.left = false; if (kb.dash2) { kb.dash2 = false; } break;
			case GLFW_KEY_RIGHT: kb.right = false; if (kb.dash2) { kb.dash2 = false; } break;
			case GLFW_KEY_F: kb.f = false; break;
			case GLFW_KEY_G: kb.g = false; break;
			case GLFW_KEY_RIGHT_CONTROL: kb.rctrl = false; break;
			case GLFW_KEY_RIGHT_ALT: kb.ralt = false; break;
			}
		}
		inputQueue.pop();
	}
	
}