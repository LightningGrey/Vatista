#pragma once

#include <glad/glad.h>
#include <GLM/glm.hpp> // For vec3 and vec4
#include <cstdint> // Needed for uint32_t
#include <memory> // Needed for smart pointers
#include <cstddef>
#include <vector>
#include "Utilities/Utils.h"

namespace Vatista {
	struct Vertex {
		glm::vec3 Position;
		glm::vec2 UV;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		//glm::vec4 Color;

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

	//used to load morph vertices
	struct LoadMorphVertex {
		//start position
		//vertex is end
		glm::vec3 Position;
		glm::vec2 UV;
		glm::vec3 Normal;
		glm::vec3 PositionS;
		glm::vec3 NormalS;
	};

	//used to load morph vertices
	struct MorphVertex2 : public Vertex {
		//start position
		//vertex is end
		glm::vec3 PositionS;
		glm::vec3 NormalS;
		glm::vec3 Tangent;
		glm::vec3 TangentS;


		MorphVertex2(Vertex vert,
			glm::vec3 posS, glm::vec3 normS, glm::vec3 tan, glm::vec3 tanS) : Vertex(vert),
			PositionS(posS), NormalS(normS), Tangent(tan), TangentS(tanS) {}
	};

	//used to load morph vertices
	struct LoadMorphVertex2 {
		//start position
		//vertex is end
		glm::vec3 Position;
		glm::vec2 UV;
		glm::vec3 Normal;
		glm::vec3 PositionS;
		glm::vec3 NormalS;
		glm::vec3 Tangent;
		glm::vec3 TangentS;
	};



	class Mesh {
	public:	
		SharedPtr(Mesh);
		// Creates a new mesh from the given vertices and indices
		Mesh(void* vertices, size_t numVerts, uint32_t* indices, size_t numIndices);
		Mesh(std::vector<uint32_t> indices, size_t numIndices,
			std::vector<Vertex> vertData, size_t numData);
		Mesh(std::vector<uint32_t> indices, size_t numIndices,
			std::vector<MorphVertex> vertData, size_t numData);
		Mesh(std::vector<uint32_t> indices, size_t numIndices,
			std::vector<MorphVertex2> vertData, size_t numData);
		~Mesh();


		//void resize();
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
}