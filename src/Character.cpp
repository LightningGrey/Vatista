#include "Character.h"

namespace Vatista {
	keyboard Vatista::Character::kb;
	Character::Character(bool ID, Vatista::Mesh::Sptr mesh, Vatista::Material::Sptr mat) : playerID(ID)
	{
		setMesh(mesh);
		setMat(mat);
		setCollider(0.74f, 1.78f);
		Atk1Collider = glm::vec2(0.4f);
		Atk2Collider = glm::vec2(0.4f);
		lives = 3;
		walls = 18.0f;
		for (int i = 0; i < 6; i++)
			animations.emplace_back();
		if (playerID) {
			setPos(-1.f, 1.f, 9);
			setRotY(90.f);
			Atk1Pos = glm::vec3(getPosX() + Collider.x + Atk1Collider.x, 1.f, 9);
			Atk2Pos = glm::vec3(getPosX() + Collider.x + Atk2Collider.x, 1.f, 9);
		}
		else {
			setPos(1.f, 1.f, 9);
			setRotY(-90.f);
			Atk1Pos = glm::vec3(getPosX() - Collider.x - Atk1Collider.x, 1.f, 9);
			Atk2Pos = glm::vec3(getPosX() - Collider.x - Atk2Collider.x, 1.f, 9);
		}
	}

	void Character::update(float dt, GLFWwindow* gameWindow, Character::Sptr p2, AudioEngine::Sptr ae)
	{
		if (hitStun && glfwGetTime() - hitstunTimer > 1.f)
			hitStun = false;
		glm::vec3 movement = glm::vec3(0.0f);
		float speed = 10.0f;
		glfwSetKeyCallback(gameWindow, key_callback);
		glfwSetInputMode(gameWindow, GLFW_STICKY_KEYS, GLFW_TRUE);
		if (playerID) {
			if (glfwGetTime() - kb.tapTimer1 > 0.2f)
				kb.doubleTap1 = false;

			if (kb.a && !kb.dash1 && glfwGetTime() - kb.atkTimer1 > 0.8f) {
				isWalking = true;
				if (getRot().y == 90.0f) {
					movement.x -= speed * 0.001f;
					if (!isDashing)
						isBlocking = true;
				}
				else if (getRot().y == -90.0f) {
					movement.x -= speed * 0.0025f;
					isBlocking = false;
				}
			}
			if (kb.d && !kb.dash1 && glfwGetTime() - kb.atkTimer1 > 0.8f) {
				isWalking = true;
				if (getRot().y == 90.0f) {
					movement.x += speed * 0.0025f;
					isBlocking = false;
				}
				else if (getRot().y == -90.0f) {
					movement.x += speed * 0.001f;
					if (!isDashing)
						isBlocking = true;
				}
			}

			if (glfwGetTime() - kb.atkTimer1 > 0.8f)
				isAttacking = false;
			if (kb.f && !isDashing && glfwGetTime() - kb.atkTimer1 > 0.8f && !isAttacking) {
				isAttacking = true;
				isBlocking = false;
				atk = true;
				kb.atkTimer1 = glfwGetTime();
				movement.x = 0;
			}
			else if (kb.g && !isDashing && glfwGetTime() - kb.atkTimer1 > 0.8f && !isAttacking) {
				isAttacking = true;
				isBlocking = false;
				atk = false;
				kb.atkTimer1 = glfwGetTime();
				movement.x = 0;
			}
			if (isAttacking && glfwGetTime() - kb.atkTimer1 > 0.2f && glfwGetTime() - kb.atkTimer1 < 0.6f) {
				if (!p2->hitStun && !p2->isDashing) {
					if (atk) {
						if (collisionCheck(Atk1Pos, Atk1Collider, p2->getPos(), p2->Collider)) {
							if (!p2->isBlocking) {
								p2->setLives(getLives() - 1);
								p2->setHitStun(true);
								p2->setHSTimer(glfwGetTime());
								std::cout << "p2 dies" << std::endl;
								ae->SetEventParameter("LightAttack", "Missed", 0.f);
								ae->SetEventParameter("LightAttack", "Blocked", 0.f);
								ae->PlayEvent("LightAttack");
							}
							else {
								std::cout << "p2 blocked" << std::endl;
								p2->setHitStun(true);
								p2->setHSTimer(glfwGetTime());
								ae->SetEventParameter("LightAttack", "Missed", 0.f);
								ae->SetEventParameter("LightAttack", "Blocked", 1.f);
								ae->PlayEvent("LightAttack");
							}
						}
						else {
							ae->SetEventParameter("LightAttack", "Missed", 1.f);
							ae->SetEventParameter("LightAttack", "Blocked", 0.f);
							ae->PlayEvent("LightAttack");
						}
					}
					else {
						if (collisionCheck(Atk2Pos, Atk2Collider, p2->getPos(), p2->Collider)) {
							if (!p2->isBlocking) {
								p2->setLives(getLives() - 1);
								p2->setHitStun(true);
								p2->setHSTimer(glfwGetTime());
								std::cout << "p2 dies" << std::endl;
								ae->SetEventParameter("HeavyAttack", "Missed", 0.f);
								ae->SetEventParameter("HeavyAttack", "Blocked", 0.f);
								ae->PlayEvent("HeavyAttack");
							}
							else {
								std::cout << "p2 blocked" << std::endl;
								p2->setHitStun(true);
								p2->setHSTimer(glfwGetTime());
								ae->SetEventParameter("HeavyAttack", "Missed", 0.f);
								ae->SetEventParameter("HeavyAttack", "Blocked", 1.f);
								ae->PlayEvent("HeavyAttack");
							}
						}
						else {
							ae->SetEventParameter("HeavyAttack", "Missed", 1.f);
							ae->SetEventParameter("HeavyAttack", "Blocked", 0.f);
							ae->PlayEvent("HeavyAttack");
						}
					}
				}
			}

			if (!isDashing) {
				if (kb.dash1 && kb.tap1 == GLFW_KEY_A) {
					lerpEnd = getPos() - glm::vec3(5.f, 0, 0);
					isDashing = true;
					ae->PlayEvent("Dash");

				}
				if (kb.dash1 && kb.tap1 == GLFW_KEY_D) {
					lerpEnd = getPos() + glm::vec3(5.f, 0, 0);
					isDashing = true;
					ae->PlayEvent("Dash");
				}
				lerper = getPosX();
				startTime = glfwGetTime();
				journeyLength = glm::distance(getPos(), lerpEnd);
				kb.dash1 = false;
				if (movement.x == 0) {
					isBlocking = false;
					isWalking = false;
				}
				if (collisionCheck(getPos() + movement, Collider, p2->getPos(), p2->Collider))
					movement.x = 0;
				setPos(getPos() + movement);
			}
			else {
				isBlocking = false;
				isWalking = false;
				if (std::floor(lerper * 1000) / 1000 == std::floor(lerpEnd.x * 1000) / 1000) {
					isDashing = false;
					//std::cout << "done" << std::endl;

				}
				else {
					float distCovered = (glfwGetTime() - startTime) * 0.45f;
					float fractionOfJourney = distCovered / journeyLength;
					lerper = (1.0 - fractionOfJourney) * lerper + (fractionOfJourney * lerpEnd.x);
					std::cout << std::floor(lerper * 1000) / 1000 << " " << std::floor(lerpEnd.x * 1000) / 1000 << std::endl;
				}
				setPosX(lerper);
				if (collisionCheck(lerpEnd, Collider, p2->getPos(), p2->Collider)) {
					if (lerpEnd.x > getPosX() && lerpEnd.x > p2->getPosX()) {
						//std::cout << "rightA" << std::endl;
						lerpEnd.x = p2->getPosX() + (p2->Collider.x * 1.02f);
						journeyLength = glm::distance(getPos(), lerpEnd);
					}
					else if (lerpEnd.x < getPosX() && lerpEnd.x < p2->getPosX()) {
						//std::cout << "leftA" << std::endl;
						lerpEnd.x = p2->getPosX() - (p2->Collider.x * 1.02f);
						journeyLength = glm::distance(getPos(), lerpEnd);
					}
					if (collisionCheck(getPos(), Collider, p2->getPos(), p2->Collider)) {
						if (lerpEnd.x > getPosX()) {
							if (getPosX() < p2->getPosX()) {
								//std::cout << "leftB" << std::endl;
								setPosX(p2->getPosX() - (p2->Collider.x));
							}
						}
						else if (lerpEnd.x < getPosX()) {
							if (getPosX() > p2->getPosX()) {
								//std::cout << "rightB" << std::endl;
								setPosX(p2->getPosX() + (p2->Collider.x));
							}
						}
					}
				}
			}
		}
		else {
			if (glfwGetTime() - kb.tapTimer2 > 0.2f)
				kb.doubleTap2 = false;

			if (kb.left && !kb.dash2 && glfwGetTime() - kb.atkTimer2 > 0.8f) {
				isWalking = true;
				if (getRot().y == 90.0f) {
					movement.x -= speed * 0.001f;
					if (!isDashing)
						isBlocking = true;
				}
				else if (getRot().y == -90.0f) {
					movement.x -= speed * 0.0025f;
					isBlocking = false;
				}
			}
			if (kb.right && glfwGetTime() - kb.atkTimer2 > 0.8f) {
				isWalking = true;
				if (getRot().y == 90.0f) {
					movement.x += speed * 0.0025f;
					isBlocking = false;
				}
				else if (getRot().y == -90.0f) {
					movement.x += speed * 0.001f;
					if (!isDashing)
						isBlocking = true;
				}
			}

			if (glfwGetTime() - kb.atkTimer2 > 0.8f)
				isAttacking = false;
			if (kb.rctrl && !isDashing && glfwGetTime() - kb.atkTimer2 > 0.8f && !isAttacking) {
				isAttacking = true;
				isBlocking = false;
				atk = true;
				kb.atkTimer2 = glfwGetTime();
				movement.x = 0;
			}
			else if (kb.ralt && !isDashing && glfwGetTime() - kb.atkTimer2 > 0.8f && !isAttacking) {
				isAttacking = true;
				isBlocking = false;
				atk = false;
				kb.atkTimer2 = glfwGetTime();
				movement.x = 0;
			}
			if (atk) {
				if (isAttacking && glfwGetTime() - kb.atkTimer2 > 0.2f && glfwGetTime() - kb.atkTimer2 < 0.6f)
					if (!p2->hitStun && !p2->isDashing) {
						if (collisionCheck(Atk1Pos, Atk1Collider, p2->getPos(), p2->Collider)) {
							if (!p2->isBlocking) {
								p2->lives -= 1;
								p2->hitStun = true;
								p2->hitstunTimer = glfwGetTime();
								std::cout << "p1 dies" << std::endl;
								ae->SetEventParameter("LightAttack", "Missed", 0.f);
								ae->SetEventParameter("LightAttack", "Blocked", 0.f);
								ae->PlayEvent("LightAttack");
							}
							else {
								std::cout << "p1 blocked" << std::endl;
								p2->hitStun = true;
								p2->hitstunTimer = glfwGetTime();
								ae->SetEventParameter("LightAttack", "Missed", 0.f);
								ae->SetEventParameter("LightAttack", "Blocked", 1.f);
								ae->PlayEvent("LightAttack");
							}
						}
						else {
							ae->SetEventParameter("LightAttack", "Missed", 1.f);
							ae->SetEventParameter("LightAttack", "Blocked", 0.f);
							ae->PlayEvent("LightAttack");
						}
					}

			}
			else {
				if (isAttacking && glfwGetTime() - kb.atkTimer2 > 0.2f && glfwGetTime() - kb.atkTimer2 < 0.6f)
					if (!p2->hitStun && !p2->isDashing) {
						if (collisionCheck(Atk2Pos, Atk2Collider, p2->getPos(), p2->Collider)) {
							if (!p2->isBlocking) {
								p2->lives -= 1;
								p2->hitStun = true;
								p2->hitstunTimer = glfwGetTime();
								std::cout << "p1 dies" << std::endl;
								ae->SetEventParameter("HeavyAttack", "Missed", 0.f);
								ae->SetEventParameter("HeavyAttack", "Blocked", 0.f);
								ae->PlayEvent("HeavyAttack");
							}
							else {
								std::cout << "p1 blocked" << std::endl;
								p2->hitStun = true;
								p2->hitstunTimer = glfwGetTime();
								ae->SetEventParameter("HeavyAttack", "Missed", 0.f);
								ae->SetEventParameter("HeavyAttack", "Blocked", 1.f);
								ae->PlayEvent("HeavyAttack");
							}
						}
						else {
							ae->SetEventParameter("HeavyAttack", "Missed", 1.f);
							ae->SetEventParameter("HeavyAttack", "Blocked", 0.f);
							ae->PlayEvent("HeavyAttack");
						}
					}
			}

			if (!isDashing) {
				if (kb.dash2 && kb.tap2 == GLFW_KEY_LEFT) {
					lerpEnd = getPos() - glm::vec3(5.f, 0, 0);
					isDashing = true;
					ae->PlayEvent("Dash");
				}
				if (kb.dash2 && kb.tap2 == GLFW_KEY_RIGHT) {
					lerpEnd = getPos() + glm::vec3(5.f, 0, 0);
					isDashing = true;
					ae->PlayEvent("Dash");
				}
				lerper = getPosX();
				startTime = glfwGetTime();
				journeyLength = glm::distance(getPos(), lerpEnd);
				kb.dash2 = false;
				if (movement.x == 0) {
					isBlocking = false;
					isWalking = false;
				}
				if (collisionCheck(getPos() + movement, Collider, p2->getPos(), p2->Collider))
					movement.x = 0;
				setPos(getPos() + movement);
			}
			else {
				isBlocking = false;
				isWalking = false;
				if (std::floor(lerper * 1000) / 1000 == std::floor(lerpEnd.x * 1000) / 1000) {
					isDashing = false;
					//std::cout << "done" << std::endl;
				}
				else {
					float distCovered = (glfwGetTime() - startTime) * 0.45f;
					float fractionOfJourney = distCovered / journeyLength;
					lerper = (1.0 - fractionOfJourney) * lerper + (fractionOfJourney * lerpEnd.x);
					std::cout << lerper << " " << lerpEnd.x << std::endl;
				}
				setPosX(lerper);
				if (collisionCheck(lerpEnd, Collider, p2->getPos(), p2->Collider)) {
					if (lerpEnd.x > getPosX() && lerpEnd.x > p2->getPosX()) {
						//std::cout << "rightA" << std::endl;
						lerpEnd.x = p2->getPosX() + (p2->Collider.x * 1.02f);
						journeyLength = glm::distance(getPos(), lerpEnd);
					}
					else if (lerpEnd.x < getPosX() && lerpEnd.x < p2->getPosX()) {
						//std::cout << "leftA" << std::endl;
						lerpEnd.x = p2->getPosX() - (p2->Collider.x * 1.02f);
						journeyLength = glm::distance(getPos(), lerpEnd);
					}
					if (collisionCheck(getPos(), Collider, p2->getPos(), p2->Collider)) {
						if (lerpEnd.x > getPosX()) {
							if (getPosX() < p2->getPosX()) {
								//std::cout << "leftB" << std::endl;
								setPosX(p2->getPosX() - (p2->Collider.x));
							}
						}
						else if (lerpEnd.x < getPosX()) {
							if (getPosX() > p2->getPosX()) {
								//std::cout << "rightB" << std::endl;
								setPosX(p2->getPosX() + (p2->Collider.x));
							}
						}
					}
				}
			}
		}
		if (getPosX() > walls)
			setPosX(walls);
		if (getPosX() < walls * -1)
			setPosX(walls * -1);
		if (getPosX() > p2->getPosX()) {
			setRotY(-90.0f);
			Atk1Pos.x = getPosX() - Collider.x - Atk1Collider.x;
			Atk2Pos.x = getPosX() - Collider.x - Atk1Collider.x;
		}
		else {
			setRotY(90.0f);
			Atk1Pos.x = getPosX() + Collider.x + Atk1Collider.x;
			Atk2Pos.x = getPosX() + Collider.x + Atk1Collider.x;
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