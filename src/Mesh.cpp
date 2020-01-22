#include "Mesh.h"
namespace Vatista {
	Mesh::Mesh(std::vector<uint32_t> indices, size_t numIndices,
		std::vector<Vertex> vertData, size_t numData) {

		//counts for each element
		myIndexCount = numIndices;		


		// Create and bind our vertex array
		glGenVertexArrays(1, &myVao);
		glBindVertexArray(myVao);

		// Create 2 buffers, 1 for vertices and the other for indices
		glGenBuffers(2, &myBuffers[0]);
		
		// Bind and buffer our vertex data
		glBindBuffer(GL_ARRAY_BUFFER, myBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, numData * sizeof(Vertex), vertData.data(), GL_STATIC_DRAW);

		// Bind and buffer our index data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myBuffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

		// Enable vertex attribute 0
		glEnableVertexAttribArray(0);
		// Our first attribute is 3 floats, the distance between 
		// them is the size of our vertex, and they will map to the position in our vertices
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, Position));

		// Enable vertex attribute 0
		glEnableVertexAttribArray(1);
		// Our first attribute is 3 floats, the distance between 
		// them is the size of our vertex, and they will map to the position in our vertices
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, UV));

		// Enable vertex attribute 0
		glEnableVertexAttribArray(2);
		// Our first attribute is 3 floats, the distance between 
		// them is the size of our vertex, and they will map to the position in our vertices
		glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, Normal));


		// Unbind our VAO
		glBindVertexArray(0);
	}

	Mesh::Mesh(std::vector<uint32_t> indices, size_t numIndices,
		std::vector<MorphVertex> vertData, size_t numData) {

		//counts for each element
		myIndexCount = numIndices;

		// Create and bind our vertex array
		glGenVertexArrays(1, &myVao);
		glBindVertexArray(myVao);

		// Create 2 buffers, 1 for vertices and the other for indices
		glGenBuffers(2, &myBuffers[0]);

		// Bind and buffer our vertex data
		glBindBuffer(GL_ARRAY_BUFFER, myBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, numData * sizeof(MorphVertex), vertData.data(), GL_STATIC_DRAW);

		// Bind and buffer our index data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myBuffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

		// Enable vertex attribute 0
		glEnableVertexAttribArray(0);
		// Our first attribute is 3 floats, the distance between 
		// them is the size of our vertex, and they will map to the position in our vertices
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(MorphVertex), (void*)offsetof(MorphVertex, Position));

		// Enable vertex attribute 0
		glEnableVertexAttribArray(1);
		// Our first attribute is 3 floats, the distance between 
		// them is the size of our vertex, and they will map to the position in our vertices
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(MorphVertex), (void*)offsetof(MorphVertex, UV));

		// Enable vertex attribute 0
		glEnableVertexAttribArray(2);
		// Our first attribute is 3 floats, the distance between 
		// them is the size of our vertex, and they will map to the position in our vertices
		glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(MorphVertex), (void*)offsetof(MorphVertex, Normal));

		// Enable vertex attribute 0
		glEnableVertexAttribArray(3);
		// Our first attribute is 3 floats, the distance between 
		// them is the size of our vertex, and they will map to the position in our vertices
		glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(MorphVertex), (void*)offsetof(MorphVertex, PositionS));

		// Enable vertex attribute 0
		glEnableVertexAttribArray(4);
		// Our first attribute is 3 floats, the distance between 
		// them is the size of our vertex, and they will map to the position in our vertices
		glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(MorphVertex), (void*)offsetof(MorphVertex, NormalS));

		// Unbind our VAO
		glBindVertexArray(0);
	}

	Mesh::~Mesh() {
		// Clean up our buffers
		glDeleteBuffers(2, myBuffers);
		// Clean up our VAO
		glDeleteVertexArrays(1, &myVao);
	}

	void Mesh::Draw() {
		// Bind the mesh
		glBindVertexArray(myVao);
		// Draw all of our vertices as triangles, our indexes are unsigned ints (uint32_t)
		glDrawElements(GL_TRIANGLES, myIndexCount, GL_UNSIGNED_INT, nullptr);
	}
}