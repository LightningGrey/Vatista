#pragma once
#include<vector>
#include <Graphics/Rendering/LightComponent.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

#include "Graphics/Rendering/Mesh.h"
#include "Graphics/Rendering/Shader.h"
#include "Graphics/Rendering/Texture2D.h"

#include "Utilities/Utils.h"

#include "Objects/GameObject.h"
#include "Objects/Characters/Character.h"
#include "Objects/Stationary/StationaryObj.h"
#include "Graphics/Rendering/LightComponent.h"
#include "Post-Processing/FrameBuffer.h"
#include <string>

namespace Vatista {


	class LightSpawner : public Light {

		std::vector<Light>vecLight;//testing dynamic vector

		std::vector<LightSpawner>lightList;//testing dynamic vector

		bool load(std::string filename);

		void addLight();
	protected:

		//file on what to load
		std::fstream loadingFile;
		//file with actual data
		std::fstream dataFile;

		int meshNumber; //volume
		glm::vec3 position(0.0f);
		glm::vec3 color(0.0f);
		float attenuation;
		float shininess;

	};



}

bool Vatista::LightSpawner::load(std::string filename)
{
	std::string line;

	//vector of all data to load 
	std::vector<std::string> dataList;

	//error checking
	loadingFile.open(filename, std::ios::in | std::ios::binary);
	if (!loadingFile) {
		VATISTA_LOG_ERROR("No file");
		throw new std::runtime_error("File open failed");
		return false;
	}

	while (std::getline(loadingFile, line)) {
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.pop_back();
		dataList.push_back(line);
	}
	loadingFile.close();

	//error checking
	for (int i = 0; i < dataList.size(); i++) {
		dataFile.open("./res/Objects/" + dataList[i], std::ios::in | std::ios::binary);
		if (!dataFile) {
			VATISTA_LOG_ERROR("No file");
			throw new std::runtime_error("File open failed");
			return false;
		}

		

		sscanf("%f %f %f", volumetype, position, color, attenuation, shininess);


	}

	return true;
}