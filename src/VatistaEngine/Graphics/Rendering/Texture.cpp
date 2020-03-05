#include "Texture.h"
#include "Utilities/Log.h"

Vatista::Texture::Texture()
{
	textureID = 0;
}

Vatista::Texture::~Texture()
{
	glDeleteTextures(1, &this->textureID);
}

void Vatista::Texture::bind(int slot)
{
	glBindTextureUnit(slot, textureID);
}

void Vatista::Texture::unbind(int slot)
{
	glBindTextureUnit(slot, 0);
}