#include "Tashia.h"
 
namespace Vatista {
	Tashia::Tashia(bool ID, std::vector<Mesh::Sptr>& meshes, Material::Sptr mat)
	{
		playerID = ID;
		setMat(mat);
		setCollider(0.74f, 1.78f);
		setAtk1Coll(glm::vec2(0.4f));
		setAtk2Coll(glm::vec2(0.4f));
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
		animations[0].second.push_back(meshes[0]);
		animations[0].first.push_back(0.3333f);
		for (int i = 1; i < 11; i++) {
			animations[1].second.push_back(meshes[i]); // Run   =  1.0s
			animations[1].first.push_back(0.1f);
		}
		for (int i = 11; i < 15; i++) {
			animations[2].second.push_back(meshes[i]); // Walk  =  .66s
			animations[2].first.push_back(0.1667f);
		}
		for (int i = 15; i < 17; i++)
			animations[3].second.push_back(meshes[i]); // FDash =  .33s
		animations[3].first.push_back(1.2733f);
		animations[3].first.push_back(0.1667f);
		for (int i = 17; i < 19; i++)
			animations[4].second.push_back(meshes[i]); // BDash =  .33s
		animations[4].first.push_back(0.1667f);
		animations[4].first.push_back(1.2733f);
		for (int i = 19; i < 23; i++)
			animations[5].second.push_back(meshes[i]); // LAtk  =  .42s
		animations[5].first.push_back(0.1333f);
		animations[5].first.push_back(0.0833f);
		animations[5].first.push_back(0.0833f);
		animations[5].first.push_back(0.1167f);
		for (int i = 23; i < 27; i++)
			animations[6].second.push_back(meshes[i]); // HAtk  =  .50s
		animations[6].first.push_back(0.1667f);
		animations[6].first.push_back(0.0833f);
		animations[6].first.push_back(0.0833f);
		animations[6].first.push_back(0.1667f);

		animations[7].second.push_back(meshes[27]); // Block =  0.16s
		animations[7].first.push_back(0.1667f);
		setMesh(animations[0].second[0]);
	}

	Tashia::~Tashia()
	{
	}

	void Tashia::update(float dt, GLFWwindow* gameWindow, Character::Sptr p2, AudioEngine::Sptr ae)
	{
		if (roundEnd) {
			setIsWalking(false);
			setIsDashing(false);
			setIsAttacking(false);
			setIsBlocking(false);
			setHitStun(false);
			float lerperFloored = std::floor(getLerper() * 1000.f);
			float lerpendFloored = std::floor(getLerpEnd() * 1000.f);
			if (lerperFloored < lerpendFloored + 10.f && lerperFloored > lerpendFloored - 10.f) {
				roundEnd = false;
			}
			else {
				float distCovered = (glfwGetTime() - getStartTime()) * 0.45f;
				float fractionOfJourney = distCovered / getJourneyLength();
				setLerper((1.0f - fractionOfJourney) * getLerper() + (fractionOfJourney * getLerpEnd()));
				setPosX(getLerper());
				return;
			}
		}

		if (getHitStun()) {
			if (glfwGetTime() - getHSTimer() > 1.0f)
				setHitStun(false);
			else
				return;
		}
		glm::vec3 movement = glm::vec3(0.0f);
		float speed = 20.0f;
		glfwSetInputMode(gameWindow, GLFW_STICKY_KEYS, GLFW_TRUE);
		glfwSetKeyCallback(gameWindow, key_callback);
		inputBuffer();
		if (playerID) {
			if (glfwGetTime() - kb.tapTimer1 > 0.2f)
				kb.doubleTap1 = false;

			if (kb.a && !kb.dash1 && !getIsAttacking() && !getIsDashing()) {
				setIsWalking(true);
				if (getRot().y == 90.0f) {
					movement.x -= speed * 0.001f;
					setIsBlocking(true);
					setStateTracker(2);
				}
				else if (getRot().y == -90.0f) {
					movement.x -= speed * 0.0025f;
					setIsBlocking(false);
					setStateTracker(1);
				}
			}
			if (kb.d && !kb.dash1 && !getIsAttacking() && !getIsDashing()) {
				setIsWalking(true);
				if (getRot().y == 90.0f) {
					movement.x += speed * 0.0025f;
					setIsBlocking(false);
					setStateTracker(1);
				}
				else if (getRot().y == -90.0f) {
					movement.x += speed * 0.001f;
					setIsBlocking(true);
					setStateTracker(2);
				}
			}

			if ((glfwGetTime() - kb.atkTimer1 > 0.41f && atk) || (glfwGetTime() - kb.atkTimer1 > 0.5f && !atk))
				setIsAttacking(false);

			// Light Attack
			if (kb.f && !getHitStun() && !getIsDashing() && !getIsAttacking() && getStamina() >= 5.f) {
				setIsAttacking(true);
				setIsBlocking(false);
				setStateTracker(5);
				atk = true;
				setStamina(getStamina() - 5.f);
				kb.atkTimer1 = glfwGetTime();
				movement.x = 0;
				ae->PlayEvent("Tashia Light Attack");
			}
			// Heavy Attack
			else if (kb.g && !getHitStun() && !getIsDashing() && !getIsAttacking() && getStamina() >= 15.f) {
				setIsAttacking(true);
				setIsBlocking(false);
				setStateTracker(6);
				atk = false;
				setStamina(getStamina() - 15.f);
				kb.atkTimer1 = glfwGetTime();
				movement.x = 0;
				ae->PlayEvent("Tashia Heavy Attack");
			}
			if (getIsAttacking() && !p2->getHitStun() && !p2->getIsDashing()) {
				if (atk && glfwGetTime() - kb.atkTimer1 > 0.2f && glfwGetTime() - kb.atkTimer1 < 0.4f) {
					if (collisionCheck(getAtk1Pos(), getAtk1Coll(), p2->getPos(), p2->getCollider())) {
						if (!p2->getIsBlocking() || (p2->getIsBlocking() && p2->getStamina() < 10.f)) {
							setWins(getWins() + 1);
							setIsAttacking(false);
							kb.atkTimer1 = 0.0f;
							std::cout << "p2 dies" << std::endl;
							setLerpEnd(-4.0f);
							p2->setLerpEnd(4.0f);
							setLerper(getPosX());
							setStartTime(glfwGetTime());
							setJourneyLength(glm::distance(getPosX(), getLerpEnd()));
							p2->setLerper(p2->getPosX());
							p2->setStartTime(glfwGetTime());
							p2->setJourneyLength(glm::distance(p2->getPosX(), p2->getLerpEnd()));
							setStamina(100.0f);
							p2->setStamina(100.0f);
							setStateTracker(4);
							p2->setStateTracker(4);
							p2->hurtSound(p2, ae);
							roundEnd = true;
							return;
						}
						else {
							std::cout << "p2 blocked" << std::endl;
							p2->setHitStun(true);
							p2->setStateTracker(7);
							p2->setHSTimer(glfwGetTime());
							p2->setStamina(p2->getStamina() - 10.f);
							ae->PlayEvent("Block");
						}
					}
				}
				else if (!atk && glfwGetTime() - kb.atkTimer1 > 0.3f && glfwGetTime() - kb.atkTimer1 < 0.49f) {
					if (collisionCheck(getAtk2Pos(), getAtk2Coll(), p2->getPos(), p2->getCollider())) {
						if (!p2->getIsBlocking() || (p2->getIsBlocking() && p2->getStamina() < 10.f)) {
							setWins(getWins() + 1);
							setIsAttacking(false);
							kb.atkTimer1 = 0.0f;
							std::cout << "p2 dies" << std::endl;
							setLerpEnd(-4.0f);
							p2->setLerpEnd(4.0f);
							setLerper(getPosX());
							setStartTime(glfwGetTime());
							setJourneyLength(glm::distance(getPosX(), getLerpEnd()));
							p2->setLerper(p2->getPosX());
							p2->setStartTime(glfwGetTime());
							p2->setJourneyLength(glm::distance(p2->getPosX(), p2->getLerpEnd()));
							setStamina(100.0f);
							p2->setStamina(100.0f);
							setStateTracker(4);
							p2->setStateTracker(4);
							p2->hurtSound(p2, ae);
							roundEnd = true;
							return;
						}
						else {
							std::cout << "p2 blocked" << std::endl;
							p2->setHitStun(true);
							p2->setStateTracker(7);
							p2->setHSTimer(glfwGetTime());
							p2->setStamina(p2->getStamina() - 10.f);
							ae->PlayEvent("Block");
						}
					}
				}
			}

			if (!getIsDashing()) {
				if (kb.dash1 && kb.tap1 == GLFW_KEY_A && getStamina() >= 20.f) {
					setLerpEnd(getPosX() - 3.0f);
					setIsDashing(true);
					if (getRot().y == 90.0f)
						setStateTracker(4); // back dash
					else if (getRot().y == -90.0f)
						setStateTracker(3); // forward dash
					setStamina(getStamina() - 20.f);
					ae->PlayEvent("Dash");
				}
				if (kb.dash1 && kb.tap1 == GLFW_KEY_D && getStamina() >= 20.f) {
					setLerpEnd(getPosX() + 3.0f);
					setIsDashing(true);
					if (getRot().y == 90.0f)
						setStateTracker(3); // forward dash
					else if (getRot().y == -90.0f)
						setStateTracker(4); // back dash
					setStamina(getStamina() - 20.f);
					ae->PlayEvent("Dash");
				}
				setLerper(getPosX());
				setStartTime(glfwGetTime());
				setJourneyLength(glm::distance(getPosX(), getLerpEnd()));
				kb.dash1 = false;
				if (collisionCheck(getPos() + movement, getCollider(), p2->getPos(), p2->getCollider()))
					movement.x = 0.f;
				if (movement.x == 0.f) {
					setIsBlocking(false);
					setIsWalking(false);
					if (!getIsDashing() && !getIsAttacking())
						setStateTracker(0);
				}
				setPos(getPos() + movement);
			}
			else {
				setIsBlocking(false);
				setIsWalking(false);
				float lerperFloored = std::floor(getLerper() * 1000.f);
				float lerpendFloored = std::floor(getLerpEnd() * 1000.f);
				if (lerperFloored < lerpendFloored + 10.f && lerperFloored > lerpendFloored - 10.f) {
					setIsDashing(false);
					std::cout << glfwGetTime() - getStartTime() << std::endl;
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
					float distCovered = (glfwGetTime() - getStartTime()) * 0.45f;
					float fractionOfJourney = distCovered / getJourneyLength();
					setLerper((1.0f - fractionOfJourney) * getLerper() + (fractionOfJourney * getLerpEnd()));
					setPosX(getLerper());
					//std::cout << lerperFloored << " " << lerpendFloored << std::endl;
				}
			}
		}
		else {
			if (glfwGetTime() - kb.tapTimer2 > 0.2f)
				kb.doubleTap2 = false;

			if (kb.left && !kb.dash2 && !getIsAttacking() && !getIsDashing()) {
				setIsWalking(true);
				if (getRot().y == 90.0f) {
					movement.x -= speed * 0.001f;
					setIsBlocking(true);
					setStateTracker(2);
				}
				else if (getRot().y == -90.0f) {
					movement.x -= speed * 0.0025f;
					setIsBlocking(false);
					setStateTracker(1);
				}
			}
			if (kb.right && !kb.dash2 && !getIsAttacking() && !getIsDashing()) {
				setIsWalking(true);
				if (getRot().y == 90.0f) {
					movement.x += speed * 0.0025f;
					setIsBlocking(false);
					setStateTracker(1);
				}
				else if (getRot().y == -90.0f) {
					movement.x += speed * 0.001f;
					setIsBlocking(true);
					setStateTracker(2);
				}
			}

			if ((glfwGetTime() - kb.atkTimer2 > 0.41f && atk) || (glfwGetTime() - kb.atkTimer2 > 0.5f && !atk))
				setIsAttacking(false);

			// Light Attack
			if (kb.rctrl && !getHitStun() && !getIsDashing() && !getIsAttacking() && getStamina() >= 5.f) {
				setIsAttacking(true);
				setIsBlocking(false);
				setStateTracker(5);
				atk = true;
				setStamina(getStamina() - 5.f);
				kb.atkTimer2 = glfwGetTime();
				movement.x = 0;
				ae->PlayEvent("Tashia Light Attack");
			}
			// Heavy Attack
			else if (kb.ralt && !getHitStun() && !getIsDashing() && !getIsAttacking() && getStamina() >= 15.f) {
				setIsAttacking(true);
				setIsBlocking(false);
				setStateTracker(6);
				atk = false;
				setStamina(getStamina() - 15.f);
				kb.atkTimer2 = glfwGetTime();
				movement.x = 0;
				ae->PlayEvent("Tashia Heavy Attack");
			}
			if (getIsAttacking() && !p2->getHitStun() && !p2->getIsDashing()) {
				if (atk && glfwGetTime() - kb.atkTimer2 > 0.2f && glfwGetTime() - kb.atkTimer2 < 0.4f) {
					if (collisionCheck(getAtk1Pos(), getAtk1Coll(), p2->getPos(), p2->getCollider())) {
						if (!p2->getIsBlocking() || (p2->getIsBlocking() && p2->getStamina() < 10.f)) {
							setWins(getWins() + 1);
							setIsAttacking(false);
							kb.atkTimer2 = 0.0f;
							std::cout << "p1 dies" << std::endl;
							setLerpEnd(4.0f);
							p2->setLerpEnd(-4.0f);
							setLerper(getPosX());
							setStartTime(glfwGetTime());
							setJourneyLength(glm::distance(getPosX(), getLerpEnd()));
							p2->setLerper(p2->getPosX());
							p2->setStartTime(glfwGetTime());
							p2->setJourneyLength(glm::distance(p2->getPosX(), p2->getLerpEnd()));
							setStamina(100.0f);
							p2->setStamina(100.0f);
							setStateTracker(4);
							p2->setStateTracker(4);
							p2->hurtSound(p2, ae);
							roundEnd = true;
							return;
						}
						else {
							std::cout << "p1 blocked" << std::endl;
							p2->setHitStun(true);
							p2->setStateTracker(7);
							p2->setHSTimer(glfwGetTime());
							p2->setStamina(p2->getStamina() - 10.f);
							ae->PlayEvent("Block");
						}
					}
				}
				else if (!atk && glfwGetTime() - kb.atkTimer2 > 0.3f && glfwGetTime() - kb.atkTimer2 < 0.49f) {
					if (collisionCheck(getAtk2Pos(), getAtk2Coll(), p2->getPos(), p2->getCollider())) {
						if (!p2->getIsBlocking() || (p2->getIsBlocking() && p2->getStamina() < 10.f)) {
							setWins(getWins() + 1);
							setIsAttacking(false);
							kb.atkTimer2 = 0.0f;
							std::cout << "p1 dies" << std::endl;
							setLerpEnd(4.0f);
							p2->setLerpEnd(-4.0f);
							setLerper(getPosX());
							setStartTime(glfwGetTime());
							setJourneyLength(glm::distance(getPosX(), getLerpEnd()));
							p2->setLerper(p2->getPosX());
							p2->setStartTime(glfwGetTime());
							p2->setJourneyLength(glm::distance(p2->getPosX(), p2->getLerpEnd()));
							setStamina(100.0f);
							p2->setStamina(100.0f);
							setStateTracker(4);
							p2->setStateTracker(4);
							p2->hurtSound(p2, ae);
							roundEnd = true;
							return;
						}
						else {
							std::cout << "p1 blocked" << std::endl;
							p2->setHitStun(true);
							p2->setStateTracker(7);
							p2->setHSTimer(glfwGetTime());
							p2->setStamina(p2->getStamina() - 10.f);
							ae->PlayEvent("Block");
						}
					}
				}
			}

			if (!getIsDashing()) {
				if (kb.dash2 && kb.tap2 == GLFW_KEY_LEFT && getStamina() >= 20.f) {
					setLerpEnd(getPosX() - 3.0f);
					setIsDashing(true);
					if (getRot().y == 90.0f)
						setStateTracker(4); // back dash
					else if (getRot().y == -90.0f)
						setStateTracker(3); // forward dash
					setStamina(getStamina() - 20.f);
					ae->PlayEvent("Dash");
				}
				if (kb.dash2 && kb.tap2 == GLFW_KEY_RIGHT && getStamina() >= 20.f) {
					setLerpEnd(getPosX() + 3.0f);
					setIsDashing(true);
					if (getRot().y == 90.0f)
						setStateTracker(3); // forward dash
					else if (getRot().y == -90.0f)
						setStateTracker(4); // back dash
					setStamina(getStamina() - 20.f);
					ae->PlayEvent("Dash");
				}
				setLerper(getPosX());
				setStartTime(glfwGetTime());
				setJourneyLength(glm::distance(getPosX(), getLerpEnd()));
				kb.dash2 = false;
				if (collisionCheck(getPos() + movement, getCollider(), p2->getPos(), p2->getCollider()))
					movement.x = 0.f;
				if (movement.x == 0.f) {
					setIsBlocking(false);
					setIsWalking(false);
					if (!getIsDashing() && !getIsAttacking())
						setStateTracker(0);
				}
				setPos(getPos() + movement);
			}
			else {
				setIsBlocking(false);
				setIsWalking(false);
				float lerperFloored = std::floor(getLerper() * 1000.f);
				float lerpendFloored = std::floor(getLerpEnd() * 1000.f);
				if (lerperFloored < lerpendFloored + 10.f && lerperFloored > lerpendFloored - 10.f) {
					setIsDashing(false);
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
					float distCovered = (glfwGetTime() - getStartTime()) * 0.45f;
					float fractionOfJourney = distCovered / getJourneyLength();
					setLerper((1.0f - fractionOfJourney) * getLerper() + (fractionOfJourney * getLerpEnd()));
					//std::cout << lerperFloored << " " << lerpendFloored << std::endl;
					setPosX(getLerper());
				}
			}
		}
		if (getPosX() > walls) {
			setPosX(walls);
			if (collisionCheck(getPos(), getCollider(), p2->getPos(), p2->getCollider()))
				p2->setPosX(getPosX() - getCollX() - p2->getCollX());
		}
		if (getPosX() < walls * -1) {
			setPosX(walls * -1);
			if (collisionCheck(getPos(), getCollider(), p2->getPos(), p2->getCollider()))
				p2->setPosX(getPosX() + getCollX() + p2->getCollX());
		}
		if (!getIsDashing()) {
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
		}
		if (getStamina() < 100.0f) {
			if (getIsBlocking())
				setStamina(getStamina() + 0.025f);
			else
				setStamina(getStamina() + 0.05f);
			if (getStamina() < 20.0f)
				ae->SetEventParameter("Tashia Stamina Pants", "LowStamina", 1);
			else
				ae->SetEventParameter("Tashia Stamina Pants", "LowStamina", 0);

		}
		if (getStamina() > 100.0f)
			setStamina(100.0f);
		updateAnim();
	}
	void Tashia::hurtSound(Character::Sptr p2, AudioEngine::Sptr ae)
	{
		ae->PlayEvent("Tashia Hurt Grunts");
		if (p2->getWins() == 1)
			ae->SetEventParameter("Tashia Hurt Grunts", "Lives", 1);
		else if (p2->getWins() == 2)
			ae->SetEventParameter("Tashia Hurt Grunts", "Lives", 2);
	}
	void Tashia::victoryLine(AudioEngine::Sptr ae)
	{
		ae->PlayEvent("Tashia Victory");
	}
}