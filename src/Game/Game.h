#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdint.h>
#include <cstdint> 

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
#include "Objects/Characters/Z3n.h"
#include "Objects/Characters/Tashia.h"
#include "Objects/UI/Stamina.h"
#include "Objects/UI/UIObject.h"
#include "Objects/Stationary/StationaryObj.h"
#include "Objects/UI/Menu.h"

#include "Post-Processing/FrameBuffer.h"
#include "Post-Processing/PostProcessing.h"

//#include "FBO.h"

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
		bool load(std::string filename, std::vector<Mesh::Sptr>& meshes, bool isChar);
		void bufferCreation();
		void preProcess();
		void postProcess();

	private:
		Window* gameWindow;
		glm::vec4 clearColour;
		std::string windowName;
		AudioEngine::Sptr audioEngine;

		//A shared pointer to our mesh
		Mesh::Sptr mesh;
		std::vector<Mesh::Sptr> menuMeshList;
		std::vector<Mesh::Sptr> stageMeshList;
		std::vector<Mesh::Sptr> uiMeshList;
		std::vector<Mesh::Sptr> z3nMeshList;
		std::vector<Mesh::Sptr> tashiaMeshList;

		// A shared pointer to our shader
		Shader::Sptr myShader;
		Shader::Sptr myNormalShader;
		//std::vector<Vatista::Shader::Sptr> shaderList;
		Texture2D::Sptr textureStamina;//place holder texture will add more complex later
		Texture2D::Sptr z3nTexture;
		Texture2D::Sptr tashiaTexture;
		Texture2D::Sptr propTexture;
		Texture2D::Sptr z3nMap;
		Texture2D::Sptr tashiaMap;
		Texture2D::Sptr stamUIText;
		std::vector<Material::Sptr> stamUIMats;
		Texture::Sptr textures;
		Material::Sptr mats;
		Camera::Sptr mainCamera;
		Camera::Sptr orthoCamera;

		std::vector<GameObject::Sptr> ObjectList;
		std::vector<GameObject::Sptr> UIList;
		Character::Sptr P1;
		Character::Sptr P2;
		StationaryObj::Sptr stage;
		Stamina::Sptr S1;
		UIObject::Sptr UI1;
		UIObject::Sptr charProfile1;
		Stamina::Sptr S2;
		UIObject::Sptr UI2;
		UIObject::Sptr charProfile2;
		float x = 100.0f;
		float y = 100.0f;

		//GameObject::Sptr StaminaBackground;


		FrameBuffer::Sptr buffer;
		FrameBuffer::Sptr depthbuffer;
		FrameBuffer::Sptr pingpongBufferH;
		FrameBuffer::Sptr pingpongBufferV;

		Mesh::Sptr fullscreenQuad;

		//basic post-process
		Shader::Sptr basePost;

		//post-process effects
		Shader::Sptr hdrShader;
		Shader::Sptr brightShader;
		Shader::Sptr blurShader;
		Shader::Sptr additiveShader;
		Shader::Sptr depthShader;
		//Shader::Sptr vblurShader;

		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

		//toggleables
		float exposure = 0.5f;
		int passes = 2;
		float brightness = 1.0f;
		//ambient, diffuse, specular, directional light, point light, spot light, texture
		std::vector<bool> toggles{1, 1, 1, 1, 1, 1, 1, 0, 1};
		Texture::Sptr blankwhite;


		glm::vec3 lightPos = glm::vec3(0.2f, 1.0f, 0.3f);
	

		float time = 0.0f;

		//file on what to load
		std::fstream loadingFile;
		//file with actual data
		std::fstream dataFile;
	};
}