#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp";

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
		void draw(float dt);
		bool load(std::string filename);
	private:
		Vatista::Window* gameWindow;
		glm::vec4 clearColour;
		std::string windowName;

		//A shared pointer to our mesh
		Vatista::Mesh::Sptr myMesh;
		Vatista::Mesh::Sptr myMesh2;
		Vatista::Mesh::Sptr myMesh3;
		std::vector<Vatista::Mesh::Sptr> meshList;

		// A shared pointer to our shader
		Vatista::Shader::Sptr myShader;
		Vatista::Shader::Sptr myNormalShader;
		std::vector<Vatista::Shader::Sptr> shaderList;

		Vatista::Texture::Sptr texture;
		Vatista::Camera::Sptr myCamera;

		Character C1;
		Character C2;
		GameObject TestMap;

		glm::mat4 modelTransform2 = glm::mat4(0.f);
		glm::vec3 pos1;
		glm::vec3 pos2;
		glm::vec3 pos3;
		
		Vatista::ObjLoader loader;

		float time = 0.0f;

		//file on what to load
		std::fstream loadingFile;
		//file with actual data
		std::fstream dataFile;
	};
}