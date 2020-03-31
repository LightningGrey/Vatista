#include "FrameBuffer.h"
#include <GLM/glm.hpp>
#include "Utilities/Log.h"


namespace Vatista {

	FrameBuffer::RenderBuffer::RenderBuffer() :
		RendererID(0),
		Resource(Texture::Sptr()),
		IsRenderBuffer(false),
		Description(RenderBufferDesc()) { 
	//n/a
		}

	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, uint8_t numSamples)
	{
		myWidth = width;
		myHeight = height;
		/*LOG_ASSERT(myWidth > 0, "Width must be greater than zero!");
		LOG_ASSERT(myHeight > 0, "Height must be greater than zero!");*/
		int maxSamples = 0;
		glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
		myNumSamples = glm::clamp((int)numSamples, 1, maxSamples);
		isValid = false;

		glCreateFramebuffers(1, &textureID);

		if (myNumSamples > 1) {
			myUnsampledFrameBuffer = std::make_shared<FrameBuffer>(width, height, 1);
		}
	}
	FrameBuffer::~FrameBuffer()
	{
		//LOG_INFO("Deleting frame buffer with ID: {}", textureID);
		glDeleteFramebuffers(1, &textureID);
	}
	Texture::Sptr FrameBuffer::GetAttachment(RenderTargetAttachment attachment) {

		if (myNumSamples > 1) {
			return myUnsampledFrameBuffer->GetAttachment(attachment);
		}
		else {
			if (myLayers.find(attachment) != myLayers.end()) {
				if (!myLayers[attachment].IsRenderBuffer) {
					return Texture::Sptr(myLayers[attachment].Resource);//no cast needed
				}
				else
					return nullptr;
			}
			else
				return nullptr;
		}
	}

	void FrameBuffer::Resize(uint32_t newWidth, uint32_t newHeight)
	{
		/*LOG_ASSERT(newWidth > 0, "Width must be greater than zero!");
		LOG_ASSERT(newHeight > 0, "Height must be greater than zero!");*/

		if (newWidth != myWidth || newHeight != myHeight) {
			myWidth = newWidth;
			myHeight = newHeight;
			for (auto& kvp : myLayers) {
				AddAttachment(kvp.second.Description);
			}
			Validate();
		}

		if (myNumSamples > 1) {
			myUnsampledFrameBuffer->Resize(newWidth, newHeight);
		}
	}

	void FrameBuffer::AddAttachment(const RenderBufferDesc& desc)
	{
		// Remove any existing render buffers bound to that slot
		if (myLayers.find(desc.Attachment) != myLayers.end()) {
			//LOG_WARN("A target is already bound to slot, deleting existing target");
			if (myLayers[desc.Attachment].IsRenderBuffer)
				glDeleteRenderbuffers(1, &myLayers[desc.Attachment].RendererID);
			else
				myLayers[desc.Attachment].Resource = nullptr;
		}
		// If this is a new attachment, and it's a color, we need to update our DrawBuffers
		else if (desc.Attachment >= RenderTargetAttachment::Color0 && desc.Attachment <= RenderTargetAttachment::Color7) {  // NEW
			myDrawBuffers.push_back(desc.Attachment);
			glNamedFramebufferDrawBuffers(textureID, myDrawBuffers.size(), reinterpret_cast<const GLenum*>(myDrawBuffers.data()));
		}

		RenderBuffer& buffer = myLayers[desc.Attachment];
		buffer.Description = desc;
		buffer.IsRenderBuffer = !desc.ShaderReadable;

		// Handling for when we can use renderbuffers instead of textures
		if (buffer.IsRenderBuffer) {
			glCreateRenderbuffers(1, &buffer.RendererID);

			// Enable multisampling on the buffer if required
			if (myNumSamples > 1)
				glNamedRenderbufferStorageMultisample(buffer.RendererID, myNumSamples, *desc.Format, myWidth, myHeight);
			else
				glNamedRenderbufferStorage(buffer.RendererID, *desc.Format, myWidth, myHeight);

			// Attach the renderbuffer to  RenderTarget
			glNamedFramebufferRenderbuffer(textureID, *desc.Attachment, GL_RENDERBUFFER, buffer.RendererID);
		}
		// We are going to use a texture as a backing resource
		else {
			// Create a descriptor for the image ///PLACE HOLDRES
			Vatista::Texture2DDesc imageDesc = Vatista::Texture2DDesc();
			imageDesc.Width = myWidth;
			imageDesc.Height = myHeight;
			imageDesc.WrapS = imageDesc.WrapT = WrapMode::ClampToEdge;
			imageDesc.MinFilter = /*florp::graphics::*/MinFilter::Linear;
			imageDesc.Format = (/*within TextureEnum.h*/InternalFormat)desc.Format;
			imageDesc.NumSamples = myNumSamples;
			imageDesc.MipmapLevels = 1; // NEW


			// Create the image, and store it's info in our buffer tag
			Vatista::Texture2D::Sptr image = std::make_shared<Vatista::Texture2D>(imageDesc);

			buffer.Resource = image;
			buffer.RendererID = image->getTexID();

			glNamedFramebufferTexture(textureID, *desc.Attachment, image->getTexID(), 0);

			if (myNumSamples > 1) {
				myUnsampledFrameBuffer->AddAttachment(desc);
			}
		}
		isValid = false;
	}

	bool FrameBuffer::Validate()
	{

		if (myNumSamples > 1) {
			myUnsampledFrameBuffer->Validate();
		}
			GLenum result = glCheckNamedFramebufferStatus(textureID, GL_FRAMEBUFFER);
			if (result != GL_FRAMEBUFFER_COMPLETE) {
				switch (result) {
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					VATISTA_LOG_ERROR("Rendertarget failed to validate. One of the attachment points is framebuffer incomplete."); break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					VATISTA_LOG_ERROR("Rendertarget failed to validate. There are no attachments!"); break;
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
					VATISTA_LOG_ERROR("Rendertarget failed to validate. Draw buffer is incomplete."); break;
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
					VATISTA_LOG_ERROR("Rendertarget failed to validate. Read buffer is incomplete."); break;
				case GL_FRAMEBUFFER_UNSUPPORTED:
					VATISTA_LOG_ERROR("Rendertarget failed to validate. Check the formats of the attached targets"); break;
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
					VATISTA_LOG_ERROR("Rendertarget failed to validate. Check the multisampling parameters on all attached targets"); break;
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
					VATISTA_LOG_ERROR("Rendertarget failed to validate for unknown reason!"); break;
				//case GL_FRAMEBUFFER_INCOMPLETE_VIEW_TARGETS_OVR:
				//	VATISTA_LOG_ERROR("Rendertarget failed to validate. Multiview targets issue!"); break;
				default: VATISTA_LOG_ERROR("Rendertarget failed to validate for unknown reason!");
				}
				isValid = false;
				return false;
			}
			else {
				isValid = true;
				return true;
			}
	}


	void FrameBuffer::bind(int slot) {
		GetAttachment(RenderTargetAttachment::Color0)->bind(slot);
	}



	void FrameBuffer::bind(uint32_t slot, RenderTargetAttachment attachment) {
		GetAttachment(attachment)->bind(slot);//binding a specific attachment to texture for reading
	}

	

	void FrameBuffer::bind(RenderTargetBinding bindMode) const {//binding for drawing
		myBinding = bindMode;
		glBindFramebuffer((GLenum)bindMode, textureID);
	}


	void FrameBuffer::unBind() const
	{
		if (myBinding != RenderTargetBinding::None) {
			if (myNumSamples > 1) {
				glBindFramebuffer(GL_READ_FRAMEBUFFER, textureID);

				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, myUnsampledFrameBuffer->textureID);

				//Blit({ 0, 0, myWidth, myHeight }, { 0, 0, myWidth, myHeight }, BufferFlags::All, MagFilter::Nearest); //no magfilter
				for (auto& kvp : myLayers) {
					if (IsColorAttachment(kvp.first)) {
						glReadBuffer(*kvp.first);
						glDrawBuffer(*kvp.first);
						//Blit({ 0, 0, myWidth, myHeight }, { 0, 0, myWidth, myHeight }, BufferFlags::Color, MagFilter::Linear);
					// no mag filter yet
					}
				}
				glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			}
			glBindFramebuffer((GLenum)myBinding, 0);
			myBinding = RenderTargetBinding::None;
		}
	}

	FrameBuffer::Sptr FrameBuffer::Clone() const
	{
		auto result = std::make_shared<FrameBuffer>(myWidth, myHeight, myNumSamples);

		for (auto& kvp : myLayers) {
			result->AddAttachment(kvp.second.Description);
		}
		result->Validate();
		return result;

	}




	void FrameBuffer::Blit(const glm::ivec4& srcBounds, const glm::ivec4& dstBounds, BufferFlags flags, MagFilter filterMode) {
		glBlitFramebuffer(
			srcBounds.x, srcBounds.y, srcBounds.z, srcBounds.w,
			dstBounds.x, dstBounds.y, dstBounds.z, dstBounds.w,
			*flags, *filterMode);
	}

}