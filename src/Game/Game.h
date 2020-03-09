#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

//#include <imgui/imgui.h>

#include "Graphics/Camera.h"
#include "Graphics/Window.h"
#include "Graphics/Rendering/Mesh.h"
#include "Graphics/Rendering/Shader.h"
#include "Graphics/Rendering/Texture2D.h"

#include "Utilities/Utils.h"

#include "Objects/GameObject.h"
#include "Objects/Characters/Character.h"
#include "Objects/Stationary/StationaryObj.h"
#include "Graphics/Rendering/LightComponent.h"

#include "Post-Processing/FrameBuffer.h"

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

		void render(float dt);
		void draw(float dt);
		void bufferCreation();
		void postProcess();
		void lightPass();

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
		
		Texture2D::Sptr texture;
		Texture2D::Sptr texture2;
		Texture2D::Sptr texture3;
		Texture2D::Sptr texture4;
		std::vector<Texture::Sptr> textures;
		std::vector<Material::Sptr> mats;
		Camera::Sptr mainCamera;
		Camera::Sptr orthoCamera;

		std::vector<GameObject::Sptr> ObjectList;
		std::vector<GameObject::Sptr> UIList;
		std::vector<Light::Sptr> LightList;
		Character::Sptr C1;
		Character::Sptr C2;
		//GameObject::Sptr TestMap;
		StationaryObj::Sptr stage;
		float x = 100.0f;
		float y = 100.0f;
		float move = 0.1f;
		//GameObject::Sptr TestStaminaBackground;
		std::vector<bool> toggles{0,0,0,0,0,0,0,0,0,0};
	
		Light::Sptr point;
		StationaryObj::Sptr prop;
		
		//ObjLoader loader;
		//
		//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		float time = 0.0f;

		//file on what to load
		std::fstream loadingFile;
		//file with actual data
		std::fstream dataFile;
	};
}