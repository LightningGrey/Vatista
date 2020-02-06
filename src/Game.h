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
#include "GameObject.h"
#include "Character.h"
#include "Texture.h"
#include "Utils.h"
#include "Math.h"
#include "AudioEngine.h"
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
		Window* gameWindow;
		glm::vec4 clearColour;
		std::string windowName;
		AudioEngine::Sptr audioEngine;

		//A shared pointer to our mesh
		Mesh::Sptr myMesh;
		std::vector<Mesh::Sptr> meshList;

		// A shared pointer to our shader
		Shader::Sptr myShader;
		Shader::Sptr myNormalShader;
		//std::vector<Vatista::Shader::Sptr> shaderList;
		Texture::Sptr textureStamina;//place holder texture will add more complex later
		Texture::Sptr texture;
		Texture::Sptr texture2;
		Texture::Sptr texture3;
		Texture::Sptr texture4;
		std::vector<Texture::Sptr> textures;
		std::vector<Material::Sptr> mats;
		Camera::Sptr mainCamera;
		Camera::Sptr orthoCamera;

		std::vector<GameObject::Sptr> ObjectList;
		std::vector<GameObject::Sptr> UIList;
		Character::Sptr C1;
		Character::Sptr C2;
		GameObject::Sptr TestMap;
		GameObject::Sptr stage;
		GameObject::Sptr staminaBar;
		GameObject::Sptr staminaBar2; 
		float x = 100.0f;
		float y = 100.0f;
		GameObject::Sptr TestStamina;
		GameObject::Sptr TestStaminaBackground;

		glm::mat4 modelTransform2 = glm::mat4(0.f);
		glm::vec3 pos1;
		glm::vec3 pos2;
		glm::vec3 pos3;
		
		ObjLoader loader;
		//
		//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		float time = 0.0f;

		//file on what to load
		std::fstream loadingFile;
		//file with actual data
		std::fstream dataFile;
	};
}