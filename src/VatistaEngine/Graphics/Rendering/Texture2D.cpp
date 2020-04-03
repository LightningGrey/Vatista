#include "Texture2D.h"
#include "Utilities/Log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbs/stb_image.h"
#include <GLM/gtc/type_ptr.hpp>

Vatista::Texture2D::Texture2D() : Texture()
{
	setup();
}

Vatista::Texture2D::Texture2D(const Texture2DDesc& description) : Texture()
{
	texDesc = description;
	setup();
}


Vatista::Texture2D::~Texture2D()
{
}

void Vatista::Texture2D::loadFile(const std::string& filename, bool alpha)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height,
		&channels, 0);

	if (data != nullptr && width != 0 && height != 0 && channels != 0) {
		this->texWidth = width;
		this->texHeight = height;
		this->texChannels = channels;

		//glTextureSubImage2D(textureID, 0, 0, 0, texWidth, texHeight, GL_RGBA, 
		//	GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, texWidth, texHeight, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		VATISTA_LOG_WARN("Failed to load texture.");
	}
	stbi_image_free(data);
}

void Vatista::Texture2D::setup()
{

	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	//glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	//glTextureStorage2D(textureID, 1, GL_RGBA, texWidth, texHeight);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)sample.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)sample.wrapT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)sample.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)sample.magFilter);

	glTextureParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(sample.borderColor));
	if (sample.anisotropicEnabled)
		glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, sample.maxAnisotropy);
}
