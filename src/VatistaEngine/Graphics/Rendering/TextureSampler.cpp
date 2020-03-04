#include "TextureSampler.h"
#include <GLM/gtc/type_ptr.hpp>

namespace Vatista {

	TextureSampler::TextureSampler()
	{
	}


	TextureSampler::~TextureSampler()
	{
		glDeleteSamplers(1, &texHandle);
	}

	void TextureSampler::bind(uint32_t slot)
	{
		glBindSampler(slot, texHandle);

	}

	void TextureSampler::unbind(uint32_t slot)
	{
		glBindSampler(slot, 0);

	}

	void TextureSampler::createSampler() {
		glCreateSamplers(1, &texHandle);

		glSamplerParameteri(texHandle, GL_TEXTURE_WRAP_S, (GLenum)wrapS);
		glSamplerParameteri(texHandle, GL_TEXTURE_WRAP_T, (GLenum)wrapT);
		glSamplerParameteri(texHandle, GL_TEXTURE_WRAP_R, (GLenum)wrapR);

		glSamplerParameteri(texHandle, GL_TEXTURE_MIN_FILTER, (GLenum)minFilter);
		glSamplerParameteri(texHandle, GL_TEXTURE_MAG_FILTER, (GLenum)magFilter);

		glSamplerParameterfv(texHandle, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));

		if (anisotropicEnabled)
			glSamplerParameterf(texHandle, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);
	}

}