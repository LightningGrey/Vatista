
////
#pragma once
#include <GLM/vec2.hpp>
#include <GLM/vec4.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
<<<<<<< Updated upstream
#include <stdint.h>
#include "Texture.h"
#include <cstdint> 
//frame buffer configuration

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


struct RenderBufferDesc {
	/*
	 * If this is set to true, we will generate an OpenGL texture instead of a render buffer
	 */
	bool ShaderReadable;
	/*
	 * The format internal format of the render buffer or texture
	 */
	uint32_t Format;
	/*
	 * Where the buffer will be attached to
	 */
	uint32_t Attachment;
};

namespace Vatista {
	class FrameBuffer : public Texture {
	public:
		typedef std::shared_ptr<FrameBuffer> Sptr;

		FrameBuffer(GLuint units);

		void init();

	private:

		GLuint FramebufferName;
		GLuint renderedTexture;

		GLuint depthrenderbuffer;

=======
#include <cstdint>
#include "Graphics/Rendering/Texture.h"
#include <cstdint> 
#include "EnumToString.h"

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

ENUM(RenderTargetType, uint32_t,
	Color32 = GL_RGBA8,
	Color24 = GL_RGB8,
	Color16 = GL_RG8,
	Color8 = GL_R8,
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
	class FrameBuffer : public Texture {
	public:
		typedef std::shared_ptr<FrameBuffer> Sptr;

		void init();

		virtual void bind(int slot) const override {

		};//todo we probably need to change this to a base texture class
		virtual void Bind(uint32_t slot, RenderTargetAttachment attachment);

		Texture::Sptr GetAttachment(RenderTargetAttachment attachment);

		//Texture::Sptr  basicily(florp::graphics::Texture2D::Sptr) is whatever our standard shared pointer for our base texture class,
		//if it changes in the future when texture.h gets altered all of it will have to change
	private:
		uint32_t myWidth, myHeight;

		uint8_t  myNumSamples;

		bool     isValid;

		mutable RenderTargetBinding myBinding;

		GLuint FramebufferName;
		GLuint renderedTexture;

		GLuint depthrenderbuffer;
		struct RenderBuffer {
			GLuint  RendererID;//when we create a texture we get a number but instead of needing to remembmer a texture (texture 1,2)
			//we can have an object represent the textures.
			Texture::Sptr Resource;
			bool            IsRenderBuffer;
			RenderBufferDesc Description;

			RenderBuffer();
		};

	};

	// missions
	//add  InternalFormat, renderer ID, and Bind

}
>>>>>>> Stashed changes

	};

	// missions
	//add  InternalFormat, renderer ID, and Bind





	
}
