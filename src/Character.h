#include "GameObject.h" 
#include <GLFW/glfw3.h> 
 
#include <iostream> 
 
class Character : public GameObject { 
public: 
	bool playerID; 
	int stanima; 
	int enemyLives; 
	Character() : enemyLives(3) {}; 
	Character(bool ID, Vatista::Mesh::Sptr mesh, Vatista::Material::Sptr mat); 
	void update(float dt, GLFWwindow* gameWindow, Character p2); 
private: 
	
	glm::vec3 lerper; 
	glm::vec3 lerpEnd; 
	bool walking = false;
	bool dashing = false; 
	bool isAttacking = false; 
	bool isBlocking = false; 
	bool atk = true; 

	glm::vec3 Atk1Pos; 
	glm::vec3 Atk2Pos; 
	glm::vec2 Atk1Collider; 
	glm::vec2 Atk2Collider; 
 
	float startTime; 
	float journeyLength; 
 
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); 
};