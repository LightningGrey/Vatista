#pragma once
#include "Objects/GameObject.h"
#include <GLFW/glfw3.h> 
#include "Utilities/Utils.h"
#include <iostream>
#include "Audio/AudioEngine.h"

typedef struct {
	bool a = false;
	bool d = false;
	bool left = false;
	bool right = false;
	bool f = false;
	bool g = false;
	bool rctrl = false;
	bool ralt = false;
	float atkTimer1 = 0.0f;
	float atkTimer2 = 0.0f;
	int tap1 = 0;
	int tap2 = 0;
	bool doubleTap1 = false;
	bool doubleTap2 = false;
	bool dash1 = false;
	bool dash2 = false;
	float tapTimer1 = 0.0f;
	float tapTimer2 = 0.0f;
}keyboard;

namespace Vatista {
	class Character : public GameObject {
	public:

		SharedPtr(Character);

		bool playerID;
		std::vector<std::pair<std::string, std::vector<Vatista::Mesh::Sptr>>> animations;
		Character(bool ID, Mesh::Sptr mesh, Material::Sptr mat);
		~Character();
		void update(float dt, GLFWwindow* gameWindow, Character::Sptr p2, AudioEngine::Sptr ae);

		void setStamina(float s) { stamina = s; }
		void setLives(int l) { lives = l; }
		void setHitStun(bool h) { hitStun = h; }
		void setHSTimer(float t) { hitstunTimer = t; }
		void setAtk1Pos(glm::vec3 a) { Atk1Pos = a; }
		void setAtk2Pos(glm::vec3 a) { Atk2Pos = a; }
		void setAtk1Coll(glm::vec2 a) { Atk1Collider = a; }
		void setAtk2Coll(glm::vec2 a) { Atk2Collider = a; }
		void setAtk1PosX(float x) { Atk1Pos.x = x; }
		void setAtk2PosX(float x) { Atk2Pos.x = x; }
		void setAtk1CollX(float x) { Atk1Collider.x = x; }
		void setAtk2CollX(float x) { Atk2Collider.x = x; }

		void Draw(const Vatista::Camera::Sptr& camera);

		float getStamina() { return stamina; }
		int getLives() { return lives; }
		bool getHitStun() { return hitStun; }
		float getHSTimer() { return hitstunTimer; }
		glm::vec3 getAtk1Pos() { return Atk1Pos; }
		glm::vec3 getAtk2Pos() { return Atk2Pos; }
		glm::vec2 getAtk1Coll() { return Atk1Collider; }
		glm::vec2 getAtk2Coll() { return Atk2Collider; }
		float getAtk1PosX() { return Atk1Pos.x; }
		float getAtk2PosX() { return Atk2Pos.x; }
		float getAtk1CollX() { return Atk1Collider.x; }
		float getAtk2CollX() { return Atk2Collider.x; }

	protected:
		float stamina;
		int lives;

		float lerper;
		glm::vec3 lerpEnd;
		bool isWalking = false;
		bool isDashing = false;
		bool isAttacking = false;
		bool isBlocking = false;
		bool hitStun = false;
		bool atk = true;
		float walls;
		float hitstunTimer;

		glm::vec3 Atk1Pos;
		glm::vec3 Atk2Pos;
		glm::vec2 Atk1Collider;
		glm::vec2 Atk2Collider;

		float startTime;
		float journeyLength;

		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static keyboard kb;


		float dt = 0.0167f;
		float morph;

		float currentTime = 0.0f;
		float endTime = 10.0f;
	};
}