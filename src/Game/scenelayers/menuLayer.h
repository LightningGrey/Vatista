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
#include "Miscellaneous Files/Math.h"
#include "Audio/AudioEngine.h"

#include "Objects/GameObject.h"
#include "Objects/Characters/Character.h"
#include "Objects/UI/Stamina.h"
#include "Objects/Stationary/StationaryObj.h"
#include "Objects/UI/Menu.h"


// the menu to select stage
namespace Vatista {
	class MenuLayer {
	public:
		MenuLayer();
		MenuLayer(std::string newName);
		~MenuLayer();
		void run();
		void init();
		void close();
		void update(float dt);
		void render(float dt);
		void draw(float dt);
		bool load(std::string filename);
	private:
		Window* gameWindow2;
		glm::vec4 clearColour2;
		std::string windowName2;
		AudioEngine::Sptr audioEngine2;

		//A shared pointer to our mesh
		Mesh::Sptr meshTitle2;

		std::vector<Mesh::Sptr> meshListMenu;

		// A shared pointer to our shader
		Shader::Sptr myShader2;
		Shader::Sptr myNormalShader;
		//std::vector<Vatista::Shader::Sptr> shaderList;
		Texture2D::Sptr texture;
		Texture2D::Sptr texture2;
		Texture2D::Sptr texture3;
		Texture2D::Sptr texture4;
		std::vector<Texture::Sptr> textures;
		std::vector<Material::Sptr> mats;


		Camera::Sptr orthoCamera;
		Menu::Sptr TitleMenu;
		//for the title name
		Menu::Sptr TitleMenu2;



		std::vector<GameObject::Sptr> ObjectList;
		std::vector<GameObject::Sptr> UIList;
		Character::Sptr C1;
		Character::Sptr C2;
		//GameObject::Sptr TestMap;
		float x = 100.0f;
		float y = 100.0f;

		//GameObject:
		float time = 0.0f;

		//file on what to load
		std::fstream loadingFile;
		//file with actual data
		std::fstream dataFile;
	};
}