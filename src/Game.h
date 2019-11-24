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
#include "Texture.h"

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
		Vatista::Mesh::Sptr   myMesh;
		std::vector<Vatista::Mesh::Sptr> meshList;

		Vatista::Texture::Sptr texture;

		// A shared pointer to our shader
		Vatista::Shader::Sptr myShader;
		std::vector<Vatista::Shader::Sptr> shaderList;
		
		Vatista::Camera::Sptr myCamera;
		
		glm::mat4 modelTransform;
		glm::mat4 modelTransform2;
		glm::mat4 modelTransform3;
		glm::vec3 pos1;
		glm::vec3 pos2;
		
		Vatista::ObjLoader loader;
	};
}