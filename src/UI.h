#pragma once

#include "Mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <GLM/glm.hpp>
//#include <imgui/imgui.h>


	class Stamina {
	public:
		Stamina(int _texture, glm::vec3 _position, glm::vec3 _scale);

		glm::vec3 position; // =  glm::vec2(0.0, 1.0);
		glm::vec3 scale;

		void setTexture(int value);
		int getTexture() const;

		

	private:
				int texture;

	};



//place holder so i can find a plcae to fit an extra 2d texture  thats from texture.cpp
//void Vatista::Texture::loadFile(const std::string& filename, bool alpha)
//{
//	stbi_set_flip_vertically_on_load(true);
//
//	int width, height, channels;
//	unsigned char* data = stbi_load(filename.c_str(), &width, &height,
//		&channels, 0);
//
//	if (data != nullptr && width != 0 && height != 0 && channels != 0) {
//		this->texWidth = width;
//		this->texHeight = height;
//		this->texChannels = channels;
//
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
//			GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else {
//		VATISTA_LOG_WARN("Failed to load texture.");
//	}
//	stbi_image_free(data);
//}