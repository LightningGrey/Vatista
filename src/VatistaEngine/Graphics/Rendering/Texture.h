#pragma once
//#define STB_IMAGE_IMPLEMENTATION
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

		void bind(int slot) const;
		static void unbind(int slot);

		virtual void loadFile(const std::string& filename, bool alpha = true) = 0;

		GLuint getTexID() { return textureID; };

	protected:
		//int texWidth, texHeight, texChannels;
		GLuint textureID;
		InternalFormat format;
		bool mipEnable = false;
		int mipLevels = -1;
		
		virtual void setup() = 0;
	};
}