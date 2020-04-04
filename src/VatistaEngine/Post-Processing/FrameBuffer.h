////
#pragma once
#include <GLM/vec2.hpp>
#include <GLM/vec4.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <stdint.h>
#include <cstdint> 
#include "Graphics/Rendering/Texture.h"
#include "Graphics/Rendering/Texture2D.h"
#include "Utilities/EnumToString.h"

//some enum stuff
ENUM(RenderTargetAttachment, uint32_t,
	Color0 = GL_COLOR_ATTACHMENT0,
	Color1 = GL_COLOR_ATTACHMENT1,
	Color2 = GL_COLOR_ATTACHMENT2,
	Color3 = GL_COLOR_ATTACHMENT3,
	Color4 = GL_COLOR_ATTACHMENT4,
	Color5 = GL_COLOR_ATTACHMENT5,
	Color6 = GL_COLOR_ATTACHMENT6,
	Color7 = GL_COLOR_ATTACHMENT7,
	Depth = GL_DEPTH_ATTACHMENT,
	DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT,
	Stencil = GL_STENCIL_ATTACHMENT
);

constexpr bool IsColorAttachment(RenderTargetAttachment attachment) {
	return attachment >= RenderTargetAttachment::Color0 && attachment <= RenderTargetAttachment::Color7;
}


ENUM(RenderTargetType, uint32_t,
	Color32 = GL_RGBA8,
	ColorRgb10 = GL_RGB10,
	ColorRgb8 = GL_RGB8,
	ColorRG8 = GL_RG8,
	ColorRed8 = GL_R8,
	ColorRgb16F = GL_RGB16F, // NEW
	ColorRgba16F = GL_RGBA16F,
	DepthStencil = GL_DEPTH24_STENCIL8,
	Depth16 = GL_DEPTH_COMPONENT16,
	Depth24 = GL_DEPTH_COMPONENT24,
	Depth32 = GL_DEPTH_COMPONENT32,
	Stencil4 = GL_STENCIL_INDEX4,
	Stencil8 = GL_STENCIL_INDEX8,
	Stencil16 = GL_STENCIL_INDEX16,
	Default = 0
);

ENUM_FLAGS(RenderTargetBinding, GLenum,
	None = 0,
	Draw = GL_DRAW_FRAMEBUFFER,
	Write = GL_DRAW_FRAMEBUFFER,
	Read = GL_READ_FRAMEBUFFER,
	Both = GL_FRAMEBUFFER
);

ENUM_FLAGS(BufferFlags, GLenum,
	None = 0,
	Color = GL_COLOR_BUFFER_BIT,
	Depth = GL_DEPTH_BUFFER_BIT,
	Stencil = GL_STENCIL_BUFFER_BIT,
	All = Color | Depth | Stencil
);


struct RenderBufferDesc {

	bool ShaderReadable;

	RenderTargetType Format;

	RenderTargetAttachment Attachment;
};

namespace Vatista {
	class FrameBuffer {
	public:
		SharedPtr(FrameBuffer);
		FrameBuffer();
		~FrameBuffer();

		void createAttachment(int width, int height, RenderTargetAttachment attach);
		void createFloatAttachment(int width, int height, RenderTargetAttachment attach);
		void createDepthAttachment(int width, int height, RenderTargetAttachment attach);
		void createRenderBuffer(int width, int height, RenderTargetAttachment attach, RenderTargetType type);
		void setup();

		void bind();
		void bindDefault();

		void bindColour(int slot, int texLocation);
		
		GLuint getID() {	return renderID;	};
		//GLuint getColour() { return textureColorbuffer; };

	private:
		GLuint renderID;
		//GLuint textureColorbuffer;
		GLuint rbo;
		std::vector<GLuint> colours;
	};
	
}



