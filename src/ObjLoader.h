#pragma once
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include "GLM/glm.hpp"

namespace Vatista {
	struct ObjLoader {
		//load model
		bool load(std::string filename, std::vector<glm::vec3>& objVertices,
			std::vector<glm::vec2>& objUVs, std::vector<glm::vec3>& objNormals,
			std::vector<uint32_t>& objVertIndices, std::vector<uint32_t>& objUVIndices,
			std::vector<uint32_t>& objNormIndices);
	};
}

#endif 