#include "Texture.h"
#include "Log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbs/stb_image.h"

Vatista::Texture::Texture()
{
	textureID = 0;
	setup();
}

Vatista::Texture::~Texture()
{
	glDeleteTextures(1, &this->textureID);
}

void Vatista::Texture::bind(int slot) const
{
	glBindTextureUnit(slot, textureID);
}

void Vatista::Texture::unbind(int slot)
{
	glBindTextureUnit(slot, 0);
}

void Vatista::Texture::loadFile(const std::string& filename, bool alpha)
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		VATISTA_LOG_WARN("Failed to load texture.");
	}
	stbi_image_free(data);
}

void Vatista::Texture::setup()
{
	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	//glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	//glTextureStorage2D(textureID, 1, GL_RGBA, texWidth, texHeight);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
