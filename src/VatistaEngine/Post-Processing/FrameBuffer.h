#pragma once
#include <GLM/vec2.hpp>
#include <GLM/vec4.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//frame buffer configuration

class FrameBuffer {
public:	
	
	
	FrameBuffer(GLuint asdf);

	void init();

private:

	GLuint FramebufferName;
	GLuint renderedTexture;

	GLuint depthrenderbuffer;

};



//ENUM(RenderTargetAttachment, uint32_t,
//	Color0 = GL_COLOR_ATTACHMENT0,
//	Color1 = GL_COLOR_ATTACHMENT1,
//	Color2 = GL_COLOR_ATTACHMENT2,
//	Color3 = GL_COLOR_ATTACHMENT3,
//	Color4 = GL_COLOR_ATTACHMENT4,
//	Color5 = GL_COLOR_ATTACHMENT5,
//	Color6 = GL_COLOR_ATTACHMENT6,
//	Color7 = GL_COLOR_ATTACHMENT7,
//	Depth = GL_DEPTH_ATTACHMENT,
//	DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT,
//	Stencil = GL_STENCIL_ATTACHMENT
//);
//
//ENUM(RenderTargetType, uint32_t,
//	Color32 = GL_RGBA8,
//	Color24 = GL_RGB8,
//	Color16 = GL_RG8,
//	Color8 = GL_R8,
//	DepthStencil = GL_DEPTH24_STENCIL8,
//	Depth16 = GL_DEPTH_COMPONENT16,
//	Depth24 = GL_DEPTH_COMPONENT24,
//	Depth32 = GL_DEPTH_COMPONENT32,
//	Stencil4 = GL_STENCIL_INDEX4,
//	Stencil8 = GL_STENCIL_INDEX8,
//	Stencil16 = GL_STENCIL_INDEX16,
//	Default = 0
//);
//
//ENUM_FLAGS(RenderTargetBinding, GLenum,
//	None = 0,
//	Draw = GL_DRAW_FRAMEBUFFER,
//	Write = GL_DRAW_FRAMEBUFFER,
//	Read = GL_READ_FRAMEBUFFER,
//	Both = GL_FRAMEBUFFER
//);
//
//ENUM_FLAGS(BufferFlags, GLenum,
//	None = 0,
//	Color = GL_COLOR_BUFFER_BIT,
//	Depth = GL_DEPTH_BUFFER_BIT,
//	Stencil = GL_STENCIL_BUFFER_BIT,
//	All = Color | Depth | Stencil
//);

