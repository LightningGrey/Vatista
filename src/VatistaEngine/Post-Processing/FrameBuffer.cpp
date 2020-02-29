#include "FrameBuffer.h"


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
		LOG_ASSERT(myWidth > 0, "Width must be greater than zero!");
		LOG_ASSERT(myHeight > 0, "Height must be greater than zero!");
		int maxSamples = 0;
		glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
		myNumSamples = glm::clamp((int)numSamples, 1, maxSamples);
		isValid = false;

		glCreateFramebuffers(1, &myRendererID);

		if (myNumSamples > 1) {
			myUnsampledFrameBuffer = std::make_shared<FrameBuffer>(width, height, 1);
		}
	}
	FrameBuffer::~FrameBuffer()
	{
		LOG_INFO("Deleting frame buffer with ID: {}", myRendererID);
		glDeleteFramebuffers(1, &myRendererID);
	}
	Texture::Sptr FrameBuffer::GetAttachment(RenderTargetAttachment attachment) {

		if (myNumSamples > 1) {
			return myUnsampledFrameBuffer->GetAttachment(attachment);
		}
		else {
			if (myLayers.find(attachment) != myLayers.end()) {
				if (!myLayers[attachment].IsRenderBuffer) {
					return std::dynamic_pointer_cast<florp::graphics::Texture2D>(myLayers[attachment].Resource);
				}
				else
					return nullptr;
			}
			else
				return nullptr;
		}
	}

	//void FrameBuffer::bind(int slot) {
	//		GetAttachment(RenderTargetAttachment::Color0)->Bind(slot);
	//	}

	void FrameBuffer::bind(uint32_t slot, RenderTargetAttachment attachment) {
		GetAttachment(attachment)->bind(slot);//binding a specific attachment to texture for reading
	}

	void FrameBuffer::UnBind() const
	{
		if (myBinding != RenderTargetBinding::None) {
			if (myNumSamples > 1) {
				glBindFramebuffer(GL_READ_FRAMEBUFFER, myRendererID);

				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, myUnsampledFrameBuffer->myRendererID);

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


	void FrameBuffer::bind(RenderTargetBinding bindMode) const {//binding for drawing
		myBinding = bindMode;
		glBindFramebuffer((GLenum)bindMode, myRendererID);
	}


	//void FrameBuffer::Blit(const glm::ivec4& srcBounds, const glm::ivec4& dstBounds, BufferFlags flags, MagFilter filterMode) {
	//	glBlitFramebuffer(
	//		srcBounds.x, srcBounds.y, srcBounds.z, srcBounds.w,
	//		dstBounds.x, dstBounds.y, dstBounds.z, dstBounds.w,
	//		*flags, *filterMode);
	//}

}