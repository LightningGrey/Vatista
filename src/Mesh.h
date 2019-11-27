#pragma once

#include <glad/glad.h>
#include <GLM/glm.hpp> // For vec3 and vec4
#include <cstdint> // Needed for uint32_t
#include <memory> // Needed for smart pointers
#include <cstddef>
#include <vector>

namespace Vatista {
	struct Vertex {
		glm::vec3 Position;
		glm::vec2 UV;
		glm::vec3 Normal;
		Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 norm) : Position(pos), UV(uv),
			Normal(norm) {};
	};

	struct MorphVertex : public Vertex {
		//start position
		//vertex is end
		glm::vec3 PositionS;
		glm::vec3 NormalS;

		MorphVertex(Vertex vert, 
			glm::vec3 posS, glm::vec3 normS) : Vertex(vert),
		PositionS(posS), NormalS(normS) {}
	};

	class Mesh {
	public:	
		// Creates a new mesh from the given vertices and indices
		Mesh(std::vector<uint32_t> indices, size_t numIndices,
			std::vector<Vertex> vertData, size_t numData);
		Mesh(std::vector<uint32_t> indices, size_t numIndices,
			std::vector<MorphVertex> vertData, size_t numData);

		// Draws this mesh
		void Draw();

	private:
		// Our GL handle for the Vertex Array Object
		GLuint myVao;
		// 0 is vertices, 1 is indices
		GLuint myBuffers[2];
		// The number of vertices and indices in this mesh
		size_t myVertexCount, myUVCount, myNormalCount, myIndexCount;
	};

	// Shorthand for shared_ptr
	typedef std::shared_ptr<Mesh> Mesh_sptr;
}