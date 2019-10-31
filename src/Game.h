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
	private:
		Vatista::Window* gameWindow;
		glm::vec4 clearColour;
		std::string windowName;

		//A shared pointer to our mesh
		Vatista::Mesh_sptr   myMesh;
		std::vector<Vatista::Mesh_sptr> meshList;

		// A shared pointer to our shader
		Vatista::Shader_sptr myShader;
		std::vector<Vatista::Shader_sptr> shaderList;
		
		Vatista::Camera::Sptr myCamera;
		
		glm::mat4 modelTransform;
		//glm::mat4 myModelTransform2;
		
		Vatista::ObjLoader loader;
	};
}