#include "Mesh.h"
namespace Vatista {
	Mesh::Mesh(std::vector<glm::vec3> vertices, size_t numVerts,
		std::vector<glm::vec2> uvs, size_t numUVs,
		std::vector<glm::vec3> normals, size_t numNormals,
		std::vector<uint32_t> vertIndices, size_t numVertIndices,
		std::vector<uint32_t> uvIndices, size_t numUVIndices,
		std::vector<uint32_t> normIndices, size_t numNormIndices) {

		//counts for each element
		myVertexCount = numVerts;
		myUVCount = numUVs;
		myNormalCount = numNormals;
		myVertIndexCount = numVertIndices;

		// Create and bind our vertex array
		glGenVertexArrays(1, &myVao);
		glBindVertexArray(myVao);

		// Create 2 buffers, 1 for vertices and the other for indices
		glGenBuffers(4, &myBuffers[0]);


		// Get a null vertex to get member offsets from
		Vertex* vert = nullptr;

		// Bind and buffer our vertex data
		glBindBuffer(GL_ARRAY_BUFFER, myBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

		// Enable vertex attribute 0
		glEnableVertexAttribArray(0);
		// Our first attribute is 3 floats, the distance between 
		// them is the size of our vertex, and they will map to the position in our vertices
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)0);


		glBindBuffer(GL_ARRAY_BUFFER, myBuffers[1]);
		glBufferData(GL_ARRAY_BUFFER, numUVs * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

		// Enable vertex attribute 0
		glEnableVertexAttribArray(1);
		// Our first attribute is 3 floats, the distance between 
		// them is the size of our vertex, and they will map to the position in our vertices
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(glm::vec2), (void*)0);


		glBindBuffer(GL_ARRAY_BUFFER, myBuffers[2]);
		glBufferData(GL_ARRAY_BUFFER, numNormals * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

		// Enable vertex attribute 0
		glEnableVertexAttribArray(2);
		// Our first attribute is 3 floats, the distance between 
		// them is the size of our vertex, and they will map to the position in our vertices
		glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)0);


		// Bind and buffer our index data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myBuffers[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numVertIndices * sizeof(uint32_t), vertIndices.data(), GL_STATIC_DRAW);
		
		
		// Unbind our VAO
		glBindVertexArray(0);
	}

	Mesh::~Mesh() {
		// Clean up our buffers
		glDeleteBuffers(4, myBuffers);
		// Clean up our VAO
		glDeleteVertexArrays(1, &myVao);
	}

	void Mesh::Draw() {
		// Bind the mesh
		glBindVertexArray(myVao);
		// Draw all of our vertices as triangles, our indexes are unsigned ints (uint32_t)
		glDrawElements(GL_TRIANGLES, myVertIndexCount, GL_UNSIGNED_INT, nullptr);
	}
}