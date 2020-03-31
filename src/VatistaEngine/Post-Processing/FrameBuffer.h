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
	class FrameBuffer : public Vatista::Texture {
	public:
		SharedPtr(FrameBuffer);

		FrameBuffer(uint32_t width, uint32_t height, uint8_t numSamples = 1);
		virtual ~FrameBuffer();

		//void init();


		uint32_t GetWidth() const { return myWidth; }

		uint32_t GetHeight() const { return myHeight; }

		glm::ivec2 GetSize() const { return { myWidth, myHeight }; }


		Texture::Sptr GetAttachment(RenderTargetAttachment attachment);


		void Resize(uint32_t newWidth, uint32_t newHeight);

		void AddAttachment(const RenderBufferDesc& desc);

		//returns True if the FrameBuffer is ready for drawing, false if otherwise
			
		bool Validate();



		virtual void bind(int slot) override;
		//todo we probably need to change this to a base texture class

		virtual void bind(uint32_t slot, RenderTargetAttachment attachment);

		void bind(RenderTargetBinding bindMode = RenderTargetBinding::Draw) const;


		void unBind() const;

		
		Sptr Clone() const;
		static void Blit(const glm::ivec4& srcBounds, const glm::ivec4& dstBounds, 
			BufferFlags flags = BufferFlags::All, MagFilter filterMode = MagFilter::Linear);
		//clones the buffer but no idea if it runs or clones properly yet



	private:
		uint32_t myWidth, myHeight;//dimensions of the framebuffer texture size of the screen

		uint8_t  myNumSamples;

		bool     isValid;

		mutable RenderTargetBinding myBinding;

		/*GLuint FramebufferName;
		GLuint renderedTexture;

		GLuint depthrenderbuffer;*/
		
		
		//checks to see if the attachment point you're asking for is a color attachment

	//store a pointer to another FBO if this one is multisampled
		Sptr  myUnsampledFrameBuffer;

		struct RenderBuffer {
			GLuint  RendererID;//when we create a texture we get a number but instead of needing to remembmer a texture (texture 1,2)
			//we can have an object represent the textures. replaced ment for igraphics version of the same variable
			//replaced by textureid

			Texture::Sptr Resource;
			bool            IsRenderBuffer;
			RenderBufferDesc Description;

			RenderBuffer();
		};
		//can't use std unordered_map for some reason
		std::map<RenderTargetAttachment, RenderBuffer> myLayers;
		std::vector<RenderTargetAttachment> myDrawBuffers;  

	};


}



