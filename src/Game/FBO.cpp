#include "FBO.h"
#include "DisplayHandler.h"

//--- Constructors and Destructors ---//
FBO::FBO()
{
	//Handles
	fboHandle = 0;
	depthTextureHandle = 0;

	//Descriptors
	width = 0;
	height = 0;
	numTextureHandles = 0;
	numTextureAttachments = 0;
	depthFlag = false;
	clearColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

	//Init arrays
	memset(textureHandles, 0, MAX_NUM_TEXTURES);
	memset(textureAttachments, 0, MAX_NUM_TEXTURES);
}

void FBO::construct(int fboWidth, int fboHeight, int numTextures, bool useDepth)
{
	//--- Save Descriptors ---//
	width = fboWidth;
	height = fboHeight;
	numTextureHandles = numTextures;
	depthFlag = useDepth;

	//--- Error Checking ---//
	//Ensure the number of requested colour textures is not over the maximum possible number
	if (numTextureHandles > MAX_NUM_TEXTURES)
	{
		std::cout << "WARNING: The number of textures you requested <" << numTextureHandles << "> for the FBO with ID <" << fboHandle << "> is over the maximum of <" << MAX_NUM_TEXTURES << ">! Trucating to the maximum number!" << std::endl;
		numTextureHandles = MAX_NUM_TEXTURES;
	}

	//--- Create the FBO ---//
	//Create the FBO handle
	glGenFramebuffers(1, &fboHandle);

	//Bind and initialize the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	{
		//--- Colour Textures ---//
		//Generate the colour texture handles
		glGenTextures(numTextureHandles, textureHandles);

		//Set the colour texture properties
		for (int i = 0; i < numTextureHandles; i++)
		{
			//Bind and generate texture properties
			glBindTexture(GL_TEXTURE_2D, textureHandles[i]);
			{
				//Create the texture within OpenGL
				glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGBA8, width, height, NULL, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

				//Set up filtering
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Minification
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Magnification

				//Set up wrapping
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //Horizontal
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //Vertical

				//Add the generated texture to the FBO
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureHandles[i], NULL);

				//Track attachments
				numTextureAttachments++;
				textureAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
			}
			glBindTexture(GL_TEXTURE_2D, NULL); //Unbind the texture for safety / cleanliness
		}

		//--- Depth Texture ---//
		if (depthFlag)
		{
			//Generate depth texture handle
			glGenTextures(1, &depthTextureHandle);

			//Bind depth texture and generate the properties
			glBindTexture(GL_TEXTURE_2D, depthTextureHandle);
			{
				//Create the texture within OpenGL
				glTexImage2D(GL_TEXTURE_2D, NULL, GL_DEPTH_COMPONENT, width, height, NULL, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

				//Set up filtering (don't need wrapping for depth texture)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Minification
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Magnification

				//Set up wrapping
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //Horizontal
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //Vertical

				//Add the depth texture to the FBO
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureHandle, 0);
			}
			glBindTexture(GL_TEXTURE_2D, NULL); //Unbind the texture for safety / cleanliness
		}

		//--- Finish Setting Up ---//
		//Set up the layout locations for each texture by passing the attachments we generated
		if (numTextureAttachments == 0)
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		else
			glDrawBuffers(numTextureAttachments, textureAttachments);

		//Ensure the FBO was created correctly
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR: FBO with ID <" << fboHandle << "> failed to initialize!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, NULL); //Unbind the FBO for safety / cleanliness
}

void FBO::release()
{
	//Destroy the colour textures
	if (numTextureHandles > 0)
	{
		glDeleteTextures(numTextureHandles, textureHandles);
		memset(textureHandles, 0, MAX_NUM_TEXTURES);
	}

	//Destroy the colour attachments
	if (numTextureAttachments > 0)
		memset(textureAttachments, 0, MAX_NUM_TEXTURES);

	//Destroy the depth texture
	if (depthTextureHandle)
		glDeleteTextures(1, &depthTextureHandle);

	//Destroy the FBO itself
	glDeleteFramebuffers(1, &fboHandle);

	//Clear the handles
	fboHandle = 0;
	depthTextureHandle = 0;

	//Clear the descriptors
	width = 0;
	height = 0;
	numTextureHandles = 0;
	depthFlag = false;

	//Revert to the back buffer
	useBackBufferForDrawing();
}

FBO::~FBO()
{
	release();
}



//--- Setters and Getters ---//
//Dimensions
int FBO::getWidth() const {
	return width;
}

int FBO::getHeight() const {
	return height;
}


//Depth Texture
bool FBO::getDepthFlag() const {
	return depthFlag;
}

GLuint FBO::getDepthTextureHandle() const 
{
	//Return the depth texture handle if it exists
	if (depthTextureHandle)
		return depthTextureHandle;
	else
	{
		std::cout << "There is no depth texture in this FBO! Returning 0!" << std::endl;
		return 0;
	}	
}


//Colour Textures
GLuint FBO::getColourTextureHandle(int textureNumber) const 
{
	//Ensure there are colour textures
	if (!numTextureHandles)
	{
		std::cout << "No colour textures bound to this FBO! Cannot return a handle so returning 0 instead!" << std::endl;
		return 0;
	}

	//Ensure the texture number isn't too high
	if (textureNumber > numTextureHandles)
	{
		std::cout << "The texture number you want is greater than the number in this FBO! Capping to the max number!" << std::endl;
		textureNumber = numTextureHandles;
	}
	
	//Return the desired handle if the number is valid
	return textureHandles[textureNumber];
}

//FBO Handle
GLuint FBO::getHandle() const {
	return fboHandle;
}


//Clear Colour
void FBO::setClearColour(glm::vec4 newClearColour) {
	clearColour = newClearColour;
}

glm::vec4 FBO::getClearColour() const {
	return clearColour;
}



//--- Methods ---//
void FBO::useForDrawing()
{
	//Bind the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	//Clear the newly bound FBO
	clearActiveFrameBuffer();

	//Use a viewport the size of the FBO
	glViewport(0, 0, width, height);
}

void FBO::useColourAsTexture(int textureNumber, GLenum textureUnit)
{
	//Set the active texture unit to the one specified
	glActiveTexture(textureUnit);

	//Bind the specified texture to the now active unit
	glBindTexture(GL_TEXTURE_2D, textureHandles[textureNumber]);
}

void FBO::useDepthAsTexture(GLenum textureUnit)
{
	//Bind the depth texture if there is one
	if (depthTextureHandle)
	{
		//Set the active texture unit to the one specified
		glActiveTexture(textureUnit);

		//Bind the specified texture to the now active unit
		glBindTexture(GL_TEXTURE_2D, depthTextureHandle);
	}
	else
		std::cout << "No depth handle attached to the FBO with ID <" << fboHandle << ">! It can't be used as a texture!" << std::endl;
}

void FBO::useBackBufferForDrawing()
{
	//Unbind any active frame buffers
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);

	//Clear the back buffer
	clearActiveFrameBuffer();

	//Use a fullscreen viewport
	glViewport(0, 0, DH::windowWidth, DH::windowHeight);
}

void FBO::clearActiveFrameBuffer()
{
	//Set the new clear colour
	glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
	
	//Clear the active frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBO::unbindTextureInUnit(GLenum textureUnit)
{
	//Set the active unit to the one specified
	glActiveTexture(textureUnit);

	//Unbind the texture
	glBindTexture(GL_TEXTURE_2D, NULL);
}