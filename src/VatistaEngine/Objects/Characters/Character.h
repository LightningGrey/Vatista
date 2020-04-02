#pragma once
#include "Objects/GameObject.h"
#include <GLFW/glfw3.h> 
#include "Utilities/Utils.h"
#include "Audio/AudioEngine.h"
#include <iostream>
#include <queue>

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
		std::vector<std::pair<std::vector<float>, std::vector<Vatista::Mesh::Sptr>>> animations;
		Character() : wins(0), stamina(100), walls(9.0f) { setScale(0.01f); }
		~Character();
		virtual void update(float dt, GLFWwindow* gameWindow, Character::Sptr p2, AudioEngine::Sptr ae) = 0;

		void setStamina(float s) { stamina = s; }
		void setWins(int w) { wins = w; }
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
		void setIsWalking(bool state) { isWalking = state; }
		void setIsDashing(bool state) { isDashing = state; }
		void setIsAttacking(bool state) { isAttacking = state; }
		void setIsBlocking(bool state) { isBlocking = state; }
		void setLerper(float l) { lerper = l; }
		void setLerpEnd(float l) { lerpEnd = l; }
		void setStartTime(float t) { startTime = t; }
		void setJourneyLength(float l) { journeyLength = l; }

		void Draw(const Vatista::Camera::Sptr& camera);

		float getStamina() { return stamina; }
		int getWins() { return wins; }
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
		bool getIsWalking(){ return isWalking; }
		bool getIsDashing(){ return isDashing; }
		bool getIsAttacking() { return isAttacking; }
		bool getIsBlocking() { return isBlocking; }
		float getLerper() { return lerper; }
		float getLerpEnd() { return lerpEnd; }
		float getStartTime() { return startTime; }
		float getJourneyLength() { return journeyLength; }


		void setStateTracker(int i);
		int getStateTracker() { return stateTracker; }
		int getAnimIndex() { return animIndex; }
		void updateAnim();

		void setNormalMap(Texture::Sptr norm) { normalMap = norm; }

	protected:
		float stamina;
		int wins;

		float lerper;
		float lerpEnd;
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
		static bool roundEnd;
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static keyboard kb;
		static std::queue<std::pair<int, int>> inputQueue;
		static void inputBuffer();

		Texture::Sptr normalMap;

		int stateTracker = 0;
		int animIndex = 0;

		float dt = 0.0167f;
		float morph;

		float currentTime = 0.0f;
		float endTime = 1.0f;
	};
}