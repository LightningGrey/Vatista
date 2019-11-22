#include "Texture.h"
#include "Log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbs/stb_image.h"

Vatista::Texture::Texture()
{
	texture = 0;
	setup();
}

Vatista::Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

void Vatista::Texture::bind(int slot) const
{
	glBindTextureUnit(slot, texture);
}

void Vatista::Texture::unbind(int slot)
{
	glBindTextureUnit(slot, 0);
}

Vatista::Texture::Sptr Vatista::Texture::loadFile(const std::string& filename, bool alpha)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, 
		&channels, 0);

	if (data != nullptr && width != 0 && height != 0 && channels != 0) {
		Sptr loadedTex = std::make_shared<Texture>();
		loadedTex->texWidth = width;
		loadedTex->texHeight = height;
		loadedTex->texChannels = channels;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_INT, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		return loadedTex;
	}
	else {
		VATISTA_LOG_WARN("Failed to load texture.");
		return nullptr;
	}

}

void Vatista::Texture::setup()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
