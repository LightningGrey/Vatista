#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp";

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "ObjLoader.h"
#include "Window.h"
#include <src\GameObject.h>
#include "Texture.h"
#include "Utils.h"

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
		void draw(float dt);
		bool collisionCheck(glm::vec3 obj1, glm::vec3 obj2, float len);
	private:
		Vatista::Window* gameWindow;
		glm::vec4 clearColour;
		std::string windowName;

		//A shared pointer to our mesh
		Vatista::Mesh::Sptr myMesh;
		Vatista::Mesh::Sptr myMesh2;
		std::vector<Vatista::Mesh::Sptr> meshList;

		// A shared pointer to our shader
		Vatista::Shader::Sptr myShader;
		Vatista::Shader::Sptr myNormalShader;
		std::vector<Vatista::Shader::Sptr> shaderList;

		Vatista::Texture::Sptr texture;
		Vatista::Camera::Sptr myCamera;

		std::vector<GameObject> myScene;

		glm::mat4 modelTransform = glm::mat4(0.f);
		glm::mat4 modelTransform2 = glm::mat4(0.f);
		glm::vec3 pos1;
		glm::vec3 pos2;
		bool dashing1;
		glm::vec3 lerpEnd;
		bool dashing2;

		float startTime;
		float journeyLength;

		//collisions
		glm::vec3 hitboxPos1;
		glm::vec3 collisionboxPos1;
		glm::vec3 hitboxPos2;
		glm::vec3 collisionboxPos2;
		
		Vatista::ObjLoader loader;
	};
}