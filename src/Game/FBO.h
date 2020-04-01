/*
Daniel MacCormick, Engine Programmer for Moonwalk Studios
Copyright Daniel MacCormick
Created for the Moonbase engine
March 2017
*/

#ifndef FBO_H
#define FBO_H

//Core libraries
#include <vector>

//3rd Party Libraries
#include "glm\glm.hpp"
#include "GL\glew.h"
#include "GLUT\glut.h"

#define MAX_NUM_TEXTURES 16

/*
FBO Class

Based off implementation by our TA, Michael Gharbaran
*/
class FBO
{
public:
	//--- Constructors and Destructors ---//
	FBO();
	void construct(int width, int height, int numTextures, bool useDepth); //Can't use normal overloaded constructor due to order of dtors & ctors destroying FBO during creation
	void release();
	~FBO();

	//--- Setters and Getters ---//
	//Dimensions
	int getWidth() const;
	int getHeight() const;

	//Depth texture
	bool getDepthFlag() const;
	GLuint getDepthTextureHandle() const;

	//Colour textures
	GLuint getColourTextureHandle(int textureNumber) const;

	//FBO Handle
	GLuint getHandle() const;

	//Clear Colour
	void setClearColour(glm::vec4 newClearColour);
	glm::vec4 getClearColour() const;

	//--- Methods ---//
	void useForDrawing();
	void useColourAsTexture(int textureNumber, GLenum textureUnit);
	void useDepthAsTexture(GLenum textureUnit);
	void clearActiveFrameBuffer();
	void useBackBufferForDrawing();
	void unbindTextureInUnit(GLenum textureUnit);

private:
	//--- Handles ---//
	GLuint fboHandle;
	GLuint depthTextureHandle;
	GLuint textureHandles[MAX_NUM_TEXTURES];
	GLuint textureAttachments[MAX_NUM_TEXTURES];

	//--- FBO Definition ---//
	int width;
	int height;
	int numTextureHandles;
	int numTextureAttachments;
	bool depthFlag;
	glm::vec4 clearColour;
};

#endif
