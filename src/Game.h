#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "UI.h"

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp";
#include "Mesh2.h"

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "ObjLoader.h"
#include "Window.h"
#include <src\GameObject.h>
#include "Character.h"
#include "Texture.h"
#include "Utils.h"
//#include "Controls.h"


namespace Vatista {
	class Game {
	public:
		Game();
		Game(std::string newName);
		~Game();
		void run();
		void init();
		void close();
		void update(float dt);
		void draw(float dt);//its this one finally found it 
		void LoadContent();

	private:
		Vatista::Window* gameWindow;
		glm::vec4 clearColour;
		std::string windowName;

		//A shared pointer to our mesh
		Vatista::Mesh::Sptr myMesh;
		Vatista::Mesh::Sptr myMesh2;
		Vatista::Mesh::Sptr myMesh3;
		std::vector<Vatista::Mesh::Sptr> meshList;

		Mesh2::Sptr my2dmesh;//this was painfull to do just to make 2d shapes

		// A shared pointer to our shader
		Vatista::Shader::Sptr myShader;
		Vatista::Shader::Sptr myNormalShader;
		std::vector<Vatista::Shader::Sptr> shaderList;

		Vatista::Texture::Sptr texture;
		Vatista::Camera::Sptr myCamera;
		Vatista::Camera::Sptr myCameraUI;//camera for the general UI elements -gary


		Character C1;
		Character C2;
		//Stamina staminaBar;// stamina bar for ui -gary

		glm::mat4 modelTransform = glm::mat4(0.f);
		glm::mat4 modelTransform2 = glm::mat4(0.f);
		//glm::vec3 pos1;
		//glm::vec3 pos2;
		//glm::vec3 pos3;
		//
		//glm::vec3 lerper1;
		//glm::vec3 lerper2;
		//glm::vec3 lerpEnd1;
		//glm::vec3 lerpEnd2;
		//bool dashing1;
		//bool dashing2;
		//bool isBlocking1;
		//bool isBlocking2;
		//bool isAttacking1;
		//bool isAttacking2;
		//
		////static bool doubleTap;
		//float startTime1;
		//float startTime2;
		//float journeyLength1;
		//float journeyLength2;
		//
		////collisions
		//glm::vec3 p1AtkPos;
		//glm::vec3 p2AtkPos;
		//glm::vec2 p1AtkCollider;
		//glm::vec2 p2AtkCollider;
		//

		Vatista::ObjLoader loader;
		//
		//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		float time = 0.0f;

	};
}