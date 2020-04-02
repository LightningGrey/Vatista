#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

#include "Graphics/Camera.h"
#include "Graphics/Window.h"
#include "Graphics/Rendering/Mesh.h"
#include "Graphics/Rendering/Shader.h"
#include "Graphics/Rendering/Texture2D.h"

#include "Utilities/Utils.h"
#include "Utilities/FileReader.h"
#include "Miscellaneous Files/Math.h"
#include "Audio/AudioEngine.h"

#include "Objects/GameObject.h"
#include "Objects/Characters/Character.h"
#include "Objects/UI/Stamina.h"
#include "Objects/UI/UIObject.h"
#include "Objects/Stationary/StationaryObj.h"
#include "Objects/UI/Menu.h"

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
		bool load(std::string filename);
		void bufferCreation();
		void postProcess();
	private:
		Window* gameWindow;
		glm::vec4 clearColour;
		std::string windowName;
		AudioEngine::Sptr audioEngine;

		//A shared pointer to our mesh
		Mesh::Sptr mesh;
		std::vector<Mesh::Sptr> meshList;
		std::vector<Mesh::Sptr> meshListMenu;


		// A shared pointer to our shader
		Shader::Sptr myShader;
		Shader::Sptr myNormalShader;
		//std::vector<Vatista::Shader::Sptr> shaderList;
		Texture2D::Sptr textureStamina;//place holder texture will add more complex later
		Texture2D::Sptr texture;
		Texture2D::Sptr texture2;
		Texture2D::Sptr bladeText;
		Texture2D::Sptr stamUIText;
		Texture2D::Sptr TitleTexture;

		std::vector<Material::Sptr> stamUIMats;
		Texture::Sptr textures;
		Material::Sptr mats;
		Camera::Sptr mainCamera;
		Camera::Sptr orthoCamera;

		std::vector<GameObject::Sptr> ObjectList;
		std::vector<GameObject::Sptr> UIList;
		std::vector<GameObject::Sptr> TitleMenuList;

		Character::Sptr C1;
		Character::Sptr C2;
		StationaryObj::Sptr stage;
		StationaryObj::Sptr sword1;
		StationaryObj::Sptr sword2;
		StationaryObj::Sptr sheath1;
		StationaryObj::Sptr sheath2;
		Stamina::Sptr S1;
		UIObject::Sptr UI1;
		Menu::Sptr TitleMenu1; //my changes
		UIObject::Sptr charProfile1;
		Stamina::Sptr S2;
		UIObject::Sptr UI2;
		UIObject::Sptr charProfile2;
		float x = 100.0f;
		float y = 100.0f;

		//GameObject::Sptr StaminaBackground;
		

		FrameBuffer::Sptr buffer;


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