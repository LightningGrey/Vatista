#include "FrameBuffer.h"



FrameBuffer::FrameBuffer(GLuint asdf) {


	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture to render to

	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}
//void FrameBufferObject::createID()
//{
//	if (!this->isValid())
//	{
//		asdftest(glGenFramebuffers(1, &m_id));
//	}
//}
//gausian blur
//void MyWindow::setupFBO() {
//	// Generate and bind the framebuffer
//	glGenFramebuffers(1, &mFBOHandle);
//	glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
//
//	// Create the texture object    
//	glGenTextures(1, &renderTex);
//	glActiveTexture(GL_TEXTURE0);  // Use texture unit 0
//	glBindTexture(GL_TEXTURE_2D, renderTex);
//	mFuncs->glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, this->width(), this->height());
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//	// Bind the texture to the FBO
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);
//
//	// Create the depth buffer
//	GLuint depthBuf;
//	glGenRenderbuffers(1, &depthBuf);
//	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
//	mFuncs->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width(), this->height());
//
//	// Bind the depth buffer to the FBO
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
//		GL_RENDERBUFFER, depthBuf);
//
//	// Set the targets for the fragment output variables
//	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
//	mFuncs->glDrawBuffers(1, drawBuffers);
//
//	// Unbind the framebuffer, and revert to default framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	// Generate and bind the intermediate framebuffer
//	glGenFramebuffers(1, &intermediateFBO);
//	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
//
//	// Create the texture object
//	glGenTextures(1, &intermediateTex);
//	glActiveTexture(GL_TEXTURE0);  // Use texture unit 0
//	glBindTexture(GL_TEXTURE_2D, intermediateTex);
//	mFuncs->glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, this->width(), this->height());
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//	// Bind the texture to the FBO
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateTex, 0);
//
//	// No depth buffer needed for this FBO
//
//	// Set the targets for the fragment output variables
//	mFuncs->glDrawBuffers(1, drawBuffers);
//
//	// Unbind the framebuffer, and revert to default framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}


//
//bool FrameBuffer::init(uint8_t fid, unsigned int width, unsigned int height)
//{
//	_fid = fid;
//	_width = width;
//	_height = height;
//
//	GLint oldfbo;
//	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldfbo);
//
//	glGenFramebuffers(1, &_fbo);
//	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
//	glBindFramebuffer(GL_FRAMEBUFFER, oldfbo);
//
//	//    _rt = RenderTarget::create(width, height);
//	//    if(nullptr == _rt) return false;
//
//#if CC_ENABLE_CACHE_TEXTURE_DATA
//	_dirtyFBOListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event) {
//		if (isDefaultFBO()) return;
//		GLint oldfbo;
//		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldfbo);
//
//		glGenFramebuffers(1, &_fbo);
//		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
//		glBindFramebuffer(GL_FRAMEBUFFER, oldfbo);
//		CCLOG("Recreate FrameBufferObject _fbo is %d", _fbo);
//		_fboBindingDirty = true;
//		});
//
//	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_dirtyFBOListener, -1);
//#endif
//	return true;
//}