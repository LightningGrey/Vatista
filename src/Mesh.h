#pragma once

#include <glad/glad.h>
#include <GLM/glm.hpp> // For vec3 and vec4
#include <cstdint> // Needed for uint32_t
#include <cstddef>
#include <vector>
#include "Utils.h"

namespace Vatista {
	struct Vertex {
		glm::vec3 Position;
		glm::vec2 UV;
		glm::vec3 Normal;
	};

	class Mesh {
	public:
		SharedPtr(Mesh);

		// Creates a new mesh from the given vertices and indices
		//Mesh(Vertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices);
		Mesh(std::vector<glm::vec3> vertices, size_t numVerts,
			std::vector<glm::vec2> uvs, size_t numUVs,
			std::vector<glm::vec3> normals, size_t numNormals,
			std::vector<uint32_t> vertIndices, size_t numVertIndices,
			std::vector<uint32_t> uvIndices, size_t numUVIndices,
			std::vector<uint32_t> normIndices, size_t numNormIndices);
		~Mesh();

		// Draws this mesh
		void Draw();

	private:
		// Our GL handle for the Vertex Array Object
		GLuint myVao;
		// 0 is vertices, 1 is indices
		// 2 is uvs, 3 is indices
		// 4 is normals, 5 is indices
		GLuint myBuffers[6];
		// The number of vertices and indices in this mesh
		size_t myVertexCount, myUVCount, myNormalCount, myIndexCount;
	};
}