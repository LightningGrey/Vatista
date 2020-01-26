#include "Character.h"

struct keyboard {
	bool a = false;
	bool d = false;
	bool left = false;
	bool right = false;
	bool f = false;
	bool g = false;
	bool rctrl = false;
	bool ralt = false;
	float atkTimer1 = 0;
	float atkTimer2 = 0;
	int tap1 = 0;
	int tap2 = 0;
	bool doubleTap1 = false;
	bool doubleTap2 = false;
	bool dash1 = false;
	bool dash2 = false;
	float tapTimer1 = 0;
	float tapTimer2 = 0;
}kb;

namespace Vatista {
	Character::Character(bool ID, Vatista::Mesh::Sptr mesh, Vatista::Material::Sptr mat) : playerID(ID)
	{
		model = mesh;
		material = mat;
		Collider = glm::vec2(0.74f, 1.78f);
		Atk1Collider = glm::vec2(0.4f);
		Atk2Collider = glm::vec2(0.4f);
		if (playerID) {
			Position = glm::vec3(-1.f, -2.f, 0);
			EulerRotDeg.y = 90.f;
			Atk1Pos = glm::vec3(Position.x + Collider.x + Atk1Collider.x, -2.f, 0);
			Atk2Pos = glm::vec3(Position.x + Collider.x + Atk2Collider.x, -2.f, 0);
		}
		else {
			Position = glm::vec3(1.f, -2.f, 0);
			EulerRotDeg.y = -90.f;
			Atk1Pos = glm::vec3(Position.x - Collider.x - Atk1Collider.x, -2.f, 0);
			Atk2Pos = glm::vec3(Position.x + Collider.x + Atk2Collider.x, -2.f, 0);
		}

	}

	void Character::update(float dt, GLFWwindow* gameWindow, Character::Sptr p2)
	{
		glm::vec3 movement = glm::vec3(0.0f);
		float speed = 5.0f;
		glfwSetKeyCallback(gameWindow, key_callback);
		glfwSetInputMode(gameWindow, GLFW_STICKY_KEYS, GLFW_TRUE);
		if (playerID) {
			if (glfwGetTime() - kb.tapTimer1 > 0.2f)
				kb.doubleTap1 = false;

			if (kb.a && !kb.dash1 && glfwGetTime() - kb.atkTimer1 > 0.8f) {
				walking = true;
				if (EulerRotDeg.y == 90.0f) {
					movement.x -= speed * 0.001f;
					if (!dashing)
						isBlocking = true;
				}
				else if (EulerRotDeg.y == -90.0f) {
					movement.x -= speed * 0.0025f;
					isBlocking = false;
				}
			}
			if (kb.d && !kb.dash1 && glfwGetTime() - kb.atkTimer1 > 0.8f) {
				walking = true;
				if (EulerRotDeg.y == 90.0f) {
					movement.x += speed * 0.003f;
					isBlocking = false;
				}
				else if (EulerRotDeg.y == -90.0f) {
					movement.x += speed * 0.0025f;
					if (!dashing)
						isBlocking = true;
				}
			}

			if (glfwGetTime() - kb.atkTimer1 > 0.8f)
				isAttacking = false;
			if (kb.f && !dashing && glfwGetTime() - kb.atkTimer1 > 0.8f && !isAttacking) {
				isAttacking = true;
				isBlocking = false;
				atk = true;
				kb.atkTimer1 = glfwGetTime();
				movement.x = 0;
			}
			else if (kb.g && !dashing && glfwGetTime() - kb.atkTimer1 > 0.8f && !isAttacking) {
				isAttacking = true;
				isBlocking = false;
				atk = false;
				kb.atkTimer1 = glfwGetTime();
				movement.x = 0;
			}
			if (atk) {
				if (isAttacking && glfwGetTime() - kb.atkTimer1 > 0.2f && glfwGetTime() - kb.atkTimer1 < 0.6f)
					if (collisionCheck(Atk1Pos, Atk1Collider, p2->Position, p2->Collider) && !p2->dashing && !p2->isBlocking) {
						enemyLives -= 1;
						std::cout << "p2 dies" << std::endl;
					}
			}
			else {
				if (isAttacking && glfwGetTime() - kb.atkTimer1 > 0.2f && glfwGetTime() - kb.atkTimer1 < 0.6f)
					if (collisionCheck(Atk2Pos, Atk2Collider, p2->Position, p2->Collider) && !p2->dashing && !p2->isBlocking) {
						enemyLives -= 1;
						std::cout << "p2 dies" << std::endl;
					}
			}

			if (!dashing) {
				if (kb.dash1 && kb.tap1 == GLFW_KEY_A) {
					lerpEnd = Position - glm::vec3(3.f, 0, 0);
					dashing = true;
				}
				if (kb.dash1 && kb.tap1 == GLFW_KEY_D) {
					lerpEnd = Position + glm::vec3(3.f, 0, 0);
					dashing = true;
				}
				lerper = Position;
				startTime = glfwGetTime();
				journeyLength = glm::distance(Position, lerpEnd);
				kb.dash1 = false;
				if (movement.x == 0) {
					isBlocking = false;
					walking = false;
				}
				if (collisionCheck(Position + movement, Collider, p2->Position, p2->Collider))
					movement.x = 0;
				Position += movement;
			}
			else {
				isBlocking = false;
				walking = false;
				if (std::floor(lerper.x * 1000) / 1000 == std::floor(lerpEnd.x * 1000) / 1000) {
					dashing = false;
					//std::cout << "done" << std::endl;

				}
				else {
					float distCovered = (glfwGetTime() - startTime) * 0.45f;
					float fractionOfJourney = distCovered / journeyLength;
					lerper.x = (1.0 - fractionOfJourney) * lerper.x + (fractionOfJourney * lerpEnd.x);
					std::cout << std::floor(lerper.x * 1000) / 1000 << " " << std::floor(lerpEnd.x * 1000) / 1000 << std::endl;
				}
				Position = lerper;
				if (collisionCheck(lerpEnd, Collider, p2->Position, p2->Collider)) {
					if (lerpEnd.x > Position.x&& lerpEnd.x > p2->Position.x) {
						//std::cout << "rightA" << std::endl;
						lerpEnd.x = p2->Position.x + (p2->Collider.x * 1.02f);
						journeyLength = glm::distance(Position, lerpEnd);
					}
					else if (lerpEnd.x < Position.x && lerpEnd.x < p2->Position.x) {
						//std::cout << "leftA" << std::endl;
						lerpEnd.x = p2->Position.x - (p2->Collider.x * 1.02f);
						journeyLength = glm::distance(Position, lerpEnd);
					}
					if (collisionCheck(Position, Collider, p2->Position, p2->Collider)) {
						if (lerpEnd.x > Position.x) {
							if (Position.x < p2->Position.x) {
								//std::cout << "leftB" << std::endl;
								Position.x = p2->Position.x - (p2->Collider.x);
							}
						}
						else if (lerpEnd.x < Position.x) {
							if (Position.x > p2->Position.x) {
								//std::cout << "rightB" << std::endl;
								Position.x = p2->Position.x + (p2->Collider.x);
							}
						}
					}
				}
			}

			if (Position.x > 7.5f)
				Position.x = 7.5f;
			if (Position.x < -7.5f)
				Position.x = -7.5f;
			if (Position.x > p2->Position.x) {
				EulerRotDeg.y = -90.0f;
				Atk1Pos.x = Position.x - Collider.x - Atk1Collider.x;
			}
			else {
				EulerRotDeg.y = 90.0f;
				Atk1Pos.x = Position.x + Collider.x + Atk1Collider.x;
			}
		}
		else {
			if (glfwGetTime() - kb.tapTimer2 > 0.2f)
				kb.doubleTap2 = false;

			if (kb.left && !kb.dash2 && glfwGetTime() - kb.atkTimer2 > 0.8f) {
				walking = true;
				if (EulerRotDeg.y == 90.0f) {
					movement.x -= speed * 0.001f;
					if (!dashing)
						isBlocking = true;
				}
				else if (EulerRotDeg.y == -90.0f) {
					movement.x -= speed * 0.0025f;
					isBlocking = false;
				}
			}
			if (kb.right && glfwGetTime() - kb.atkTimer2 > 0.8f) {
				walking = true;
				if (EulerRotDeg.y == 90.0f) {
					movement.x += speed * 0.0025f;
					isBlocking = false;
				}
				else if (EulerRotDeg.y == -90.0f) {
					movement.x += speed * 0.001f;
					if (!dashing)
						isBlocking = true;
				}
			}

			if (glfwGetTime() - kb.atkTimer2 > 0.8f)
				isAttacking = false;
			if (kb.rctrl && !dashing && glfwGetTime() - kb.atkTimer2 > 0.8f && !isAttacking) {
				isAttacking = true;
				isBlocking = false;
				atk = true;
				kb.atkTimer2 = glfwGetTime();
				movement.x = 0;
			}
			else if (kb.ralt && !dashing && glfwGetTime() - kb.atkTimer2 > 0.8f && !isAttacking) {
				isAttacking = true;
				isBlocking = false;
				atk = false;
				kb.atkTimer2 = glfwGetTime();
				movement.x = 0;
			}
			if (atk) {
				if (isAttacking && glfwGetTime() - kb.atkTimer2 > 0.2f && glfwGetTime() - kb.atkTimer2 < 0.6f)
					if (collisionCheck(Atk1Pos, Atk1Collider, p2->Position, p2->Collider) && !p2->dashing && !p2->isBlocking) {
						enemyLives -= 1;
						std::cout << "p1 dies" << std::endl;
					}
			}
			else {
				if (isAttacking && glfwGetTime() - kb.atkTimer2 > 0.2f && glfwGetTime() - kb.atkTimer2 < 0.6f)
					if (collisionCheck(Atk1Pos, Atk1Collider, p2->Position, p2->Collider) && !p2->dashing && !p2->isBlocking) {
						enemyLives -= 1;
						std::cout << "p1 dies" << std::endl;
					}
			}

			if (!dashing) {
				if (kb.dash2 && kb.tap2 == GLFW_KEY_LEFT) {
					lerpEnd = Position - glm::vec3(3.f, 0, 0);
					dashing = true;
				}
				if (kb.dash2 && kb.tap2 == GLFW_KEY_RIGHT) {
					lerpEnd = Position + glm::vec3(3.f, 0, 0);
					dashing = true;
				}
				lerper = Position;
				startTime = glfwGetTime();
				journeyLength = glm::distance(Position, lerpEnd);
				kb.dash2 = false;
				if (movement.x == 0) {
					isBlocking = false;
					walking = false;
				}
				if (collisionCheck(Position + movement, Collider, p2->Position, p2->Collider))
					movement.x = 0;
				Position += movement;
			}
			else {
				isBlocking = false;
				walking = false;
				if (std::floor(lerper.x * 1000) / 1000 == std::floor(lerpEnd.x * 1000) / 1000) {
					dashing = false;
					//std::cout << "done" << std::endl;
				}
				else {
					float distCovered = (glfwGetTime() - startTime) * 0.45f;
					float fractionOfJourney = distCovered / journeyLength;
					lerper.x = (1.0 - fractionOfJourney) * lerper.x + (fractionOfJourney * lerpEnd.x);
					std::cout << lerper.x << " " << lerpEnd.x << std::endl;
				}
				Position = lerper;
				if (collisionCheck(lerpEnd, Collider, p2->Position, p2->Collider)) {
					if (lerpEnd.x > Position.x&& lerpEnd.x > p2->Position.x) {
						//std::cout << "rightA" << std::endl;
						lerpEnd.x = p2->Position.x + (p2->Collider.x * 1.02f);
						journeyLength = glm::distance(Position, lerpEnd);
					}
					else if (lerpEnd.x < Position.x && lerpEnd.x < p2->Position.x) {
						//std::cout << "leftA" << std::endl;
						lerpEnd.x = p2->Position.x - (p2->Collider.x * 1.02f);
						journeyLength = glm::distance(Position, lerpEnd);
					}
					if (collisionCheck(Position, Collider, p2->Position, p2->Collider)) {
						if (lerpEnd.x > Position.x) {
							if (Position.x < p2->Position.x) {
								//std::cout << "leftB" << std::endl;
								Position.x = p2->Position.x - (p2->Collider.x);
							}
						}
						else if (lerpEnd.x < Position.x) {
							if (Position.x > p2->Position.x) {
								//std::cout << "rightB" << std::endl;
								Position.x = p2->Position.x + (p2->Collider.x);
							}
						}
					}
				}
			}

			if (Position.x > 7.5f)
				Position.x = 7.5f;
			if (Position.x < -7.5f)
				Position.x = -7.5f;
			if (Position.x > p2->Position.x) {
				EulerRotDeg.y = -90.0f;
				Atk1Pos.x = Position.x - Collider.x - Atk1Collider.x;
			}
			else {
				EulerRotDeg.y = 90.0f;
				Atk1Pos.x = Position.x + Collider.x + Atk1Collider.x;
			}
		}

	}

	void Character::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS) {
			switch (key) {
			case GLFW_KEY_A:
				if (kb.doubleTap1 && kb.tap1 == key && glfwGetTime() - kb.atkTimer1 > 0.8f) {
					//std::cout << "dash" << std::endl;
					kb.dash1 = true;
					kb.doubleTap1 = false;
				}
				else {
					kb.tapTimer1 = glfwGetTime();
					kb.doubleTap1 = true;
					kb.tap1 = key;
				}
				kb.a = true;
				break;
			case GLFW_KEY_D:
				if (kb.doubleTap1 && kb.tap1 == key && glfwGetTime() - kb.atkTimer1 > 0.8f) {
					//std::cout << "dash" << std::endl;
					kb.dash1 = true;
					kb.doubleTap1 = false;
				}
				else {
					kb.tapTimer1 = glfwGetTime();
					kb.doubleTap1 = true;
					kb.tap1 = key;
				}
				kb.d = true;
				break;
			case GLFW_KEY_LEFT:
				if (kb.doubleTap2 && kb.tap2 == key && glfwGetTime() - kb.atkTimer2 > 0.8f) {
					//std::cout << "dash" << std::endl;
					kb.dash2 = true;
					kb.doubleTap2 = false;
				}
				else {
					kb.tapTimer2 = glfwGetTime();
					kb.doubleTap2 = true;
					kb.tap2 = key;
				}
				kb.left = true;
				break;
			case GLFW_KEY_RIGHT:
				if (kb.doubleTap2 && kb.tap2 == key && glfwGetTime() - kb.atkTimer2 > 0.8f) {
					//std::cout << "dash" << std::endl;
					kb.dash2 = true;
					kb.doubleTap2 = false;
				}
				else {
					kb.tapTimer2 = glfwGetTime();
					kb.doubleTap2 = true;
					kb.tap2 = key;
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
		if (action == GLFW_RELEASE) {
			switch (key) {
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

	}
}