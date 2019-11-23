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

		Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 norm) : Position(pos), UV(uv),
			Normal(norm) {}
	};

	class Mesh {
	public:
		SharedPtr(Mesh);

		// Creates a new mesh from the given vertices and indices
		//Mesh(Vertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices);
		Mesh(std::vector<glm::vec3> vertices, size_t numVerts,
			std::vector<glm::vec2> uvs, size_t numUVs,
			std::vector<glm::vec3> normals, size_t numNormals,
			std::vector<uint32_t> indices, size_t numIndices,
			std::vector<Vertex> vertData, size_t numData);
		~Mesh();

		// Draws this mesh
		void Draw();

	private:
		// Our GL handle for the Vertex Array Object
		GLuint myVao;
		// 0 is vertices, 1 is indices
		// 2 is uvs, 3 is indices
		// 4 is normals, 5 is indices
		GLuint myBuffers[2];
		// The number of vertices and indices in this mesh
		size_t myIndexCount;
	};
}