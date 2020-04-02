#include "FrameBuffer.h"
#include <GLM/glm.hpp>
#include "Utilities/Log.h"


namespace Vatista {

	FrameBuffer::FrameBuffer()
	{
		setup();

	}

	FrameBuffer::~FrameBuffer()
	{
		//LOG_INFO("Deleting frame buffer with ID: {}", textureID);
		glDeleteFramebuffers(1, &this->renderID);
	}

	void FrameBuffer::createAttachment(int width, int height, RenderTargetAttachment attach)
	{
		GLuint textureColorbuffer;
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attach, GL_TEXTURE_2D, textureColorbuffer, 0);
		colours.push_back(textureColorbuffer);
	} 

	void FrameBuffer::createFloatAttachment(int width, int height, RenderTargetAttachment attach)
	{
		GLuint textureColorbuffer;
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attach, GL_TEXTURE_2D, textureColorbuffer, 0);
		colours.push_back(textureColorbuffer);
	}

	void FrameBuffer::createRenderBuffer(int width, int height, RenderTargetAttachment attach,
		RenderTargetType type)
	{
		glGenRenderbuffers(1, &this->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)type, width, height); 
		// use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, (GLenum)attach, GL_RENDERBUFFER, this->rbo);
		// now actually attach it
	}


	void FrameBuffer::setup()
	{
		glGenFramebuffers(1, &this->renderID);
		glBindFramebuffer(GL_FRAMEBUFFER, this->renderID);
	}

	void FrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->renderID);
	}


	void FrameBuffer::bindDefault()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::bindColour(int slot, int texLocation)
	{
		glBindTextureUnit(slot, colours[texLocation]);
	}

}