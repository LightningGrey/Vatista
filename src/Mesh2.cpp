#include "Mesh2.h"

Mesh2::Mesh2(Vertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices) {
	myIndexCount2 = numIndices;
	myVertexCount2 = numVerts;
	// Create and bind our vertex array
	glCreateVertexArrays(1, &myVao2);
	glBindVertexArray(myVao2);
	// Create 2 buffers, 1 for vertices and the other for indices
	glCreateBuffers(2, myBuffers2);

	// Bind and buffer our vertex data
	glBindBuffer(GL_ARRAY_BUFFER, myBuffers2[0]);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	// Bind and buffer our index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myBuffers2[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW);


	// Get a null vertex to get member offsets from
	Vertex* vert = nullptr;

	// Enable vertex attribute 0
	glEnableVertexAttribArray(0);
	// Our first attribute is 3 floats, the distance between
	// them is the size of our vertex, and they will map to the position in our vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), &(vert->Position));
	// Enable vertex attribute 1
	glEnableVertexAttribArray(1);
	// Our second attribute is 4 floats, the distance between
	// them is the size of our vertex, and they will map to the color in our vertices
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), &(vert->Color));

	// Unbind our VAO
	glBindVertexArray(0);
}




Mesh2::~Mesh2() {
	// Clean up our buffers
	glDeleteBuffers(2, myBuffers2);
	// Clean up our VAO
	glDeleteVertexArrays(1, &myVao2);
}

void Mesh2::Draw() {
	// Bind the mesh
	glBindVertexArray(myVao2);
	// Draw all of our vertices as triangles, our indexes are unsigned ints (uint32_t)
	glDrawElements(GL_TRIANGLES, myIndexCount2, GL_UNSIGNED_INT, nullptr);
}
