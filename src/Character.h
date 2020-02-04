#pragma once
#include "GameObject.h"
#include <GLFW/glfw3.h> 
#include "Utils.h"
#include <iostream>
#include "AudioEngine.h"
 
namespace Vatista {
	class Character : public GameObject {
	public:
		SharedPtr(Character);

		bool playerID;
		int stanima;
		int lives;
		std::vector<std::pair<std::string, std::vector<Vatista::Mesh::Sptr>>> animations;
		Character(bool ID, Mesh::Sptr mesh, Material::Sptr mat);
		void update(float dt, GLFWwindow* gameWindow, Character::Sptr p2, AudioEngine::Sptr ae);
	private:

		glm::vec3 lerper;
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
	};
}