#pragma once
#include <glad/glad.h>
#include "Utilities/TextureEnums.h"
#include "Utilities/Utils.h"
#include "TextureSampler.h"

namespace Vatista {
	class Texture {  
	public:
		SharedPtr(Texture);

		Texture();
		~Texture();

		virtual void bind(int slot);
		static void unbind(int slot);

		virtual void loadFile(const std::string& filename, bool alpha = true) {};

		GLuint getTexID() { return textureID; };

	protected:
		//int texWidth, texHeight, texChannels;
		GLuint textureID;
		InternalFormat format;
		bool mipEnable = false;
		int mipLevels = -1;
		
		//virtual void setup() = 0;
	};
}