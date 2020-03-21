#include "Character.h"
#include <GLM\include\GLM\gtc\matrix_transform.hpp>


namespace Vatista {
	keyboard Vatista::Character::kb;
	bool Character::roundEnd = false;
	Character::Character(bool ID, std::vector<Mesh::Sptr>& meshes, Material::Sptr mat) : playerID(ID)
	{
		setMat(mat);
		setCollider(0.74f, 1.78f);
		setStamina(100.0f);
		setAtk1Coll(glm::vec2(0.4f));
		setAtk2Coll(glm::vec2(0.4f));
		setLives(3);
		setScale(0.01f);
		walls = 9.0f;
		for (int i = 0; i < 8; i++)
			animations.emplace_back();
		if (playerID) {
			setPos(-4.0f, 0.0f, 9.0f);
			setRotY(90.0f);
			setAtk1Pos(glm::vec3(getPosX() + getCollX() + getAtk1CollX(), getPosY(), 9.0f));
			setAtk2Pos(glm::vec3(getPosX() + getCollX() + getAtk2CollX(), getPosY(), 9.0f));
		}
		else {
			setPos(4.0f, 0.0f, 9.0f);
			setRotY(-90.f);
			setAtk1Pos(glm::vec3(getPosX() - getCollX() - getAtk1CollX(), getPosY(), 9.0f));
			setAtk2Pos(glm::vec3(getPosX() - getCollX() - getAtk2CollX(), getPosY(), 9.0f));
		}
		animations[0].second.push_back(meshes[20]);
		animations[0].first.push_back(1.0f);
		for (int i = 21; i < 28; i++) {
			animations[1].second.push_back(meshes[i]); // Run   =  1.0s
			animations[1].first.push_back(0.14285f);
		}
		for (int i = 28; i < 33; i++) {
			animations[2].second.push_back(meshes[i]); // Walk  =  .82s
			animations[2].first.push_back(0.164f);
		}
		for (int i = 33; i < 35; i++) {
			animations[3].second.push_back(meshes[i]); // FDash =  0.5s
			animations[3].first.push_back(0.25f);
		}
		for (int i = 35; i < 37; i++) {
			animations[4].second.push_back(meshes[i]); // BDash =  0.5s
			animations[4].first.push_back(0.25f);
		}
		for (int i = 37; i < 42; i++)
			animations[5].second.push_back(meshes[i]); // LAtk  =  .41s
		animations[5].first.push_back(0.0833f);
		animations[5].first.push_back(0.1667f);
		animations[5].first.push_back(0.05f);
		animations[5].first.push_back(0.05f);
		animations[5].first.push_back(0.0667f);
		for (int i = 42; i < 48; i++)
			animations[6].second.push_back(meshes[i]); // HAtk  =  .58s
		animations[6].first.push_back(0.0833f);
		animations[6].first.push_back(0.0833f);
		animations[6].first.push_back(0.1667f);
		animations[6].first.push_back(0.0667f);
		animations[6].first.push_back(0.1f);
		animations[6].first.push_back(0.0833f);
		for (int i = 48; i < 50; i++) {
			animations[7].second.push_back(meshes[i]); // Block =  0.2s
			animations[7].first.push_back(0.1f);
		}
		setMesh(animations[0].second[0]);
	}

	Character::~Character()
	{
	}

	void Character::update(float dt, GLFWwindow* gameWindow, Character::Sptr p2, AudioEngine::Sptr ae)
	{
		if (roundEnd) {
			isWalking = false;
			isDashing = false;
			isAttacking = false;
			isBlocking = false;
			setHitStun(false);
			float lerperFloored = std::floor(lerper * 1000.f);
			float lerpendFloored = std::floor(lerpEnd.x * 1000.f);
			if (lerperFloored < lerpendFloored + 10.f && lerperFloored > lerpendFloored - 10.f) {
				roundEnd = false;
			}
			else {
				float distCovered = (glfwGetTime() - startTime) * 0.45f;
				float fractionOfJourney = distCovered / journeyLength;
				lerper = (1.0f - fractionOfJourney) * lerper + (fractionOfJourney * lerpEnd.x);
				setPosX(lerper);
				return;
			}
		}

		if (getHitStun() && glfwGetTime() - getHSTimer() > 1.0f)
			setHitStun(false);
		glm::vec3 movement = glm::vec3(0.0f);
		float speed = 10.0f;
		glfwSetKeyCallback(gameWindow, key_callback);
		glfwSetInputMode(gameWindow, GLFW_STICKY_KEYS, GLFW_TRUE);
		if (playerID) {
			if (glfwGetTime() - kb.tapTimer1 > 0.2f)
				kb.doubleTap1 = false;

			if (kb.a && !kb.dash1 && !isAttacking && !isDashing) {
				isWalking = true;
				if (getRot().y == 90.0f) {
					movement.x -= speed * 0.001f;
					isBlocking = true;
					setStateTracker(2);
				}
				else if (getRot().y == -90.0f) {
					movement.x -= speed * 0.0025f;
					isBlocking = false;
					setStateTracker(1);
				}
			}
			if (kb.d && !kb.dash1 && !isAttacking && !isDashing) {
				isWalking = true;
				if (getRot().y == 90.0f) {
					movement.x += speed * 0.0025f;
					isBlocking = false;
					setStateTracker(1);
				}
				else if (getRot().y == -90.0f) {
					movement.x += speed * 0.001f;
					isBlocking = true;
					setStateTracker(2);
				}
			}

			if (glfwGetTime() - kb.atkTimer1 > 0.8f)
				isAttacking = false;

			// Light Attack
			if (kb.f && !getHitStun() && !isDashing  && !isAttacking && getStamina() >= 5.f) {
				isAttacking = true;
				isBlocking = false;
				setStateTracker(5);
				atk = true;
				setStamina(getStamina() - 5.f);
				kb.atkTimer1 = glfwGetTime();
				movement.x = 0;
				ae->PlayEvent("LightAttack");
			}
			// Heavy Attack
			else if (kb.g && !getHitStun() && !isDashing && !isAttacking && getStamina() >= 15.f) {
				isAttacking = true;
				isBlocking = false;
				setStateTracker(6);
				atk = false;
				setStamina(getStamina() - 15.f);
				kb.atkTimer1 = glfwGetTime();
				movement.x = 0;
				ae->PlayEvent("HeavyAttack");
			}
			if (isAttacking && glfwGetTime() - kb.atkTimer1 > 0.2f && glfwGetTime() - kb.atkTimer1 < 0.6f) {
				if (!p2->hitStun && !p2->isDashing) {
					if (atk) {
						if (collisionCheck(getAtk1Pos(), getAtk1Coll(), p2->getPos(), p2->getCollider())) {
							if (!p2->isBlocking || (p2->isBlocking && p2->getStamina() < 10.f)) {
								p2->setLives(p2->getLives() - 1);
								isAttacking = false;
								kb.atkTimer1 = 0.0f;
								std::cout << "p2 dies" << std::endl;
								lerpEnd = glm::vec3(-4.0f, 0.0f, 9.0f);
								p2->lerpEnd = glm::vec3(4.0f, 0.0f, 9.0f);
								lerper = getPosX();
								startTime = glfwGetTime();
								journeyLength = glm::distance(getPos(), lerpEnd);
								p2->lerper = p2->getPosX();
								p2->startTime = glfwGetTime();
								p2->journeyLength = glm::distance(p2->getPos(), p2->lerpEnd);
								setStamina(100.0f);
								p2->setStamina(100.0f);
								setStateTracker(4);
								p2->setStateTracker(4);
								roundEnd = true;
								return;
							}
							else {
								std::cout << "p2 blocked" << std::endl;
								p2->setHitStun(true);
								p2->setHSTimer(glfwGetTime());
								p2->setStamina(p2->getStamina() - 10.f);
								ae->PlayEvent("Block");
							}
						}
					}
					else {
						if (collisionCheck(getAtk2Pos(), getAtk2Coll(), p2->getPos(), p2->getCollider())) {
							if (!p2->isBlocking || (p2->isBlocking && p2->getStamina() < 10.f)) {
								p2->setLives(p2->getLives() - 1);
								isAttacking = false;
								kb.atkTimer1 = 0.0f;
								std::cout << "p2 dies" << std::endl;
								lerpEnd = glm::vec3(-4.0f, 0.0f, 9.0f);
								p2->lerpEnd = glm::vec3(4.0f, 0.0f, 9.0f);
								lerper = getPosX();
								startTime = glfwGetTime();
								journeyLength = glm::distance(getPos(), lerpEnd);
								p2->lerper = p2->getPosX();
								p2->startTime = glfwGetTime();
								p2->journeyLength = glm::distance(p2->getPos(), p2->lerpEnd);
								setStamina(100.0f);
								p2->setStamina(100.0f);
								setStateTracker(4);
								p2->setStateTracker(4);
								roundEnd = true;
								return;
							}
							else {
								std::cout << "p2 blocked" << std::endl;
								p2->setHitStun(true);
								p2->setHSTimer(glfwGetTime());
								p2->setStamina(p2->getStamina() - 10.f);
								ae->PlayEvent("Block");
							}
						}
					}
				}
			}

			if (!isDashing) {
				if (kb.dash1 && kb.tap1 == GLFW_KEY_A && getStamina() >= 20.f) {
					lerpEnd = getPos() - glm::vec3(3.0f, 0.0f, 0.0f);
					isDashing = true;
					if (getRot().y == 90.0f)
						setStateTracker(4); // back dash
					else if (getRot().y==-90.0f)
						setStateTracker(3); // forward dash
					setStamina(getStamina() - 20.f);
					ae->PlayEvent("Dash");
				}
				if (kb.dash1 && kb.tap1 == GLFW_KEY_D && getStamina() >= 20.f) {
					lerpEnd = getPos() + glm::vec3(3.0f, 0.0f, 0.0f);
					isDashing = true;
					if (getRot().y == 90.0f)
						setStateTracker(3); // forward dash
					else if (getRot().y == -90.0f)
						setStateTracker(4); // back dash
					setStamina(getStamina() - 20.f);
					ae->PlayEvent("Dash");
				}
				lerper = getPosX();
				startTime = glfwGetTime();
				journeyLength = glm::distance(getPos(), lerpEnd);
				kb.dash1 = false;
				if (collisionCheck(getPos() + movement, getCollider(), p2->getPos(), p2->getCollider()))
					movement.x = 0.f;
				if (movement.x == 0.f) {
					isBlocking = false;
					isWalking = false;
					if (!isDashing && !isAttacking)
						setStateTracker(0);
				}
				setPos(getPos() + movement);
			}
			else {
				isBlocking = false;
				isWalking = false;
				float lerperFloored = std::floor(lerper * 1000.f);
				float lerpendFloored = std::floor(lerpEnd.x * 1000.f);
				if (lerperFloored < lerpendFloored + 10.f && lerperFloored > lerpendFloored - 10.f) {
					isDashing = false;
					//std::cout << "done" << std::endl;
					if (collisionCheck(getPos(), getCollider(), p2->getPos(), p2->getCollider())) {
						float midPoint = (getPosX() + p2->getPosX()) / 2.0f;
						if (getPosX() > p2->getPosX()) {
							setPosX(midPoint + (getCollX()/2.0f));
							p2->setPosX(midPoint - (p2->getCollX()/2.0f));
						}
						else {
							setPosX(midPoint - (getCollX()/2.0f));
							p2->setPosX(midPoint + (p2->getCollX()/2.0f));
						}
					}
				}
				else {
					float distCovered = (glfwGetTime() - startTime) * 0.45f;
					float fractionOfJourney = distCovered / journeyLength;
					lerper = (1.0f - fractionOfJourney) * lerper + (fractionOfJourney * lerpEnd.x);
					setPosX(lerper);
					//std::cout << lerperFloored << " " << lerpendFloored << std::endl;
				}
			}
		}
		else {
			if (glfwGetTime() - kb.tapTimer2 > 0.2f)
				kb.doubleTap2 = false;

			if (kb.left && !kb.dash2 && !isAttacking && !isDashing) {
				isWalking = true;
				if (getRot().y == 90.0f) {
					movement.x -= speed * 0.001f;
					isBlocking = true;
					setStateTracker(2);
				}
				else if (getRot().y == -90.0f) {
					movement.x -= speed * 0.0025f;
					isBlocking = false;
					setStateTracker(1);
				}
			}
			if (kb.right && !kb.dash2 && !isAttacking && !isDashing) {
				isWalking = true;
				if (getRot().y == 90.0f) {
					movement.x += speed * 0.0025f;
					isBlocking = false;
					setStateTracker(1);
				}
				else if (getRot().y == -90.0f) {
					movement.x += speed * 0.001f;
					isBlocking = true;
					setStateTracker(2);
				}
			}

			if (glfwGetTime() - kb.atkTimer2 > 0.8f)
				isAttacking = false;

			// Light Attack
			if (kb.rctrl && !getHitStun() && !isDashing && !isAttacking && getStamina() >= 5.f) {
				isAttacking = true;
				isBlocking = false;
				setStateTracker(5);
				atk = true;
				setStamina(getStamina() - 5.f);
				kb.atkTimer2 = glfwGetTime();
				movement.x = 0;
				ae->PlayEvent("LightAttack");
			}
			// Heavy Attack
			else if (kb.ralt && !getHitStun() && !isDashing && !isAttacking && getStamina() >= 15.f) {
				isAttacking = true;
				isBlocking = false;
				setStateTracker(6);
				atk = false;
				setStamina(getStamina() - 15.f);
				kb.atkTimer2 = glfwGetTime();
				movement.x = 0;
				ae->PlayEvent("HeavyAttack");
			}
			if (isAttacking && glfwGetTime() - kb.atkTimer2 > 0.2f && glfwGetTime() - kb.atkTimer2 < 0.6f) {
				if (!p2->hitStun && !p2->isDashing) {
					if (atk) {
						if (collisionCheck(getAtk1Pos(), getAtk1Coll(), p2->getPos(), p2->getCollider())) {
							if (!p2->isBlocking || (p2->isBlocking && p2->getStamina() < 10.f)) {
								p2->setLives(p2->getLives() - 1);
								isAttacking = false;
								kb.atkTimer2 = 0.0f;
								std::cout << "p1 dies" << std::endl;
								lerpEnd = glm::vec3(4.0f, 0.0f, 9.0f);
								p2->lerpEnd = glm::vec3(-4.0f, 0.0f, 9.0f);
								lerper = getPosX();
								startTime = glfwGetTime();
								journeyLength = glm::distance(getPos(), lerpEnd);
								p2->lerper = p2->getPosX();
								p2->startTime = glfwGetTime();
								p2->journeyLength = glm::distance(p2->getPos(), p2->lerpEnd);
								setStamina(100.0f);
								p2->setStamina(100.0f);
								setStateTracker(4);
								p2->setStateTracker(4);
								roundEnd = true;
								return;
							}
							else {
								std::cout << "p1 blocked" << std::endl;
								p2->setHitStun(true);
								p2->setHSTimer(glfwGetTime());
								p2->setStamina(p2->getStamina() - 10.f);
								ae->PlayEvent("Block");
							}
						}
					}
					else {
						if (collisionCheck(getAtk2Pos(), getAtk2Coll(), p2->getPos(), p2->getCollider())) {
							if (!p2->isBlocking || (p2->isBlocking && p2->getStamina() < 10.f)) {
								p2->setLives(p2->getLives() - 1);
								isAttacking = false;
								kb.atkTimer2 = 0.0f;
								std::cout << "p1 dies" << std::endl;
								lerpEnd = glm::vec3(4.0f, 0.0f, 9.0f);
								p2->lerpEnd = glm::vec3(-4.0f, 0.0f, 9.0f);
								lerper = getPosX();
								startTime = glfwGetTime();
								journeyLength = glm::distance(getPos(), lerpEnd);
								p2->lerper = p2->getPosX();
								p2->startTime = glfwGetTime();
								p2->journeyLength = glm::distance(p2->getPos(), p2->lerpEnd);
								setStamina(100.0f);
								p2->setStamina(100.0f);
								setStateTracker(4);
								p2->setStateTracker(4);
								roundEnd = true;
								return;
							}
							else {
								std::cout << "p1 blocked" << std::endl;
								p2->setHitStun(true);
								p2->setHSTimer(glfwGetTime());
								p2->setStamina(p2->getStamina() - 10.f);
								ae->PlayEvent("Block");
							}
						}
					}
				}
			}

			if (!isDashing) {
				if (kb.dash2 && kb.tap2 == GLFW_KEY_LEFT && getStamina() >= 20.f) {
					lerpEnd = getPos() - glm::vec3(3.0f, 0.0f, 0.0f);
					isDashing = true;
					if (getRot().y == 90.0f)
						setStateTracker(4); // back dash
					else if (getRot().y == -90.0f)
						setStateTracker(3); // forward dash
					setStamina(getStamina() - 20.f);
					ae->PlayEvent("Dash");
				}
				if (kb.dash2 && kb.tap2 == GLFW_KEY_RIGHT && getStamina() >= 20.f) {
					lerpEnd = getPos() + glm::vec3(3.0f, 0.0f, 0.0f);
					isDashing = true;
					if (getRot().y == 90.0f)
						setStateTracker(3); // forward dash
					else if (getRot().y == -90.0f)
						setStateTracker(4); // back dash
					setStamina(getStamina() - 20.f);
					ae->PlayEvent("Dash");
				}
				lerper = getPosX();
				startTime = glfwGetTime();
				journeyLength = glm::distance(getPos(), lerpEnd);
				kb.dash2 = false;
				if (collisionCheck(getPos() + movement, getCollider(), p2->getPos(), p2->getCollider()))
					movement.x = 0.f;
				if (movement.x == 0.f) {
					isBlocking = false;
					isWalking = false;
					if (!isDashing && !isAttacking)
						setStateTracker(0);
				}
				setPos(getPos() + movement);
			}
			else {
				isBlocking = false;
				isWalking = false;
				float lerperFloored = std::floor(lerper * 1000.f);
				float lerpendFloored = std::floor(lerpEnd.x * 1000.f);
				if (lerperFloored < lerpendFloored + 10.f && lerperFloored > lerpendFloored - 10.f) {
					isDashing = false;
					//std::cout << "done" << std::endl;
					if (collisionCheck(getPos(), getCollider(), p2->getPos(), p2->getCollider())) {
						float midPoint = (getPosX() + p2->getPosX()) / 2.0f;
						if (getPosX() > p2->getPosX()) {
							setPosX(midPoint + (getCollX() / 2.0f));
							p2->setPosX(midPoint - (p2->getCollX() / 2.0f));
						}
						else {
							setPosX(midPoint - (getCollX() / 2.0f));
							p2->setPosX(midPoint + (p2->getCollX() / 2.0f));
						}
					}
				}
				else {
					float distCovered = (glfwGetTime() - startTime) * 0.45f;
					float fractionOfJourney = distCovered / journeyLength;
					lerper = (1.0f - fractionOfJourney) * lerper + (fractionOfJourney * lerpEnd.x);
					//std::cout << lerperFloored << " " << lerpendFloored << std::endl;
					setPosX(lerper);
				}
			}
		}
		if (getPosX() > walls) {
			setPosX(walls);
			if (collisionCheck(getPos(), getCollider(), p2->getPos(), p2->getCollider()))
				p2->setPosX(getPosX() - getCollX() - p2->getCollX());
		}
		if (getPosX() < walls * -1){
			setPosX(walls * -1);
			if (collisionCheck(getPos(), getCollider(), p2->getPos(), p2->getCollider()))
				p2->setPosX(getPosX() + getCollX() + p2->getCollX());
		}
		if (getPosX() > p2->getPosX()) {
			setRotY(-90.0f);
			setAtk1PosX(getPosX() - getCollX() - getAtk1CollX());
			setAtk2PosX(getPosX() - getCollX() - getAtk2CollX());
		}
		else {
			setRotY(90.0f);
			setAtk1PosX(getPosX() + getCollX() + getAtk1CollX());
			setAtk2PosX(getPosX() + getCollX() + getAtk2CollX());
		}
		if (getStamina() < 100.0f) {
			if (isBlocking)
				setStamina(getStamina() + 0.05f);
			else
				setStamina(getStamina() + 0.1f);
		}
		if (getStamina() > 100.0f)
			setStamina(100.0f);
		if (getHitStun())
			setStateTracker(7);
		updateAnim();
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
		if (currentTime >= endTime) {
			if (getStateTracker()!=0) {
				if (getAnimIndex() + 1 < animations[getStateTracker()].second.size())
					animIndex++;
				else
					animIndex = 0;
				setMesh(animations[getStateTracker()].second[getAnimIndex()]);
				currentTime = 0;
				endTime = animations[getStateTracker()].first[getAnimIndex()];
			}
		}
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
		//dir light, move this to variable
		material->GetShader()->SetUniform("dirlight.direction", { 0.0f, -1.0f, -0.6f });
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