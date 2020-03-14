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
		Mesh::Sptr myMesh;
		std::vector<Mesh::Sptr> meshList;
		Texture::Sptr texture;
		Camera::Sptr mainCamera;


		std::vector<GameObject::Sptr> ObjectList;
		
		StationaryObj::Sptr carSphere;
		StationaryObj::Sptr wallCube;
		float timer = 0.0f;
		float velocity = 0.0f;


		//file on what to load
		std::fstream loadingFile;
		//file with actual data
		std::fstream dataFile;
	};
}