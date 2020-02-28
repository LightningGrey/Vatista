#include "FrameBuffer.h"


#include "FrameBuffer.h"

namespace Vatista {


	void Vatista::FrameBuffer::init()
	{
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



	void FrameBuffer::Bind(uint32_t slot, RenderTargetAttachment attachment) {
		GetAttachment(attachment)->Bind(slot);//binding a specific attachment to texture for reading
	}


	void FrameBuffer::Bind(RenderTargetBinding bindMode) const {//binding for drawing
		myBinding = bindMode;
		glBindFramebuffer((GLenum)bindMode, myRendererID);
	}

}