#pragma once
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include "GLM/glm.hpp"
#include "Mesh.h"

namespace Vatista {
	struct ObjLoader {
		//load model
		bool load(std::string filename,
			std::vector<uint32_t>& objIndices, std::vector<Vertex>& vertData);
	};
}

#endif 