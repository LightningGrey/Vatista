#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include "Utilities/Utils.h"
#include "EnumToString.h"
#include "TextureEnum.h"

namespace Vatista {
	class Texture {
	public:
		SharedPtr(Texture);

		Texture();
		~Texture();

		virtual void bind(int slot) const;
		static void unbind(int slot);

		void loadFile(const std::string& filename, bool alpha = true);

		GLuint getTexID() { return textureID; };

		void createTexture(int width, int height, Vatista::graphics::InternalFormat format);//place holder,
		//similar to loadfile function but it doesn't actually load anything

	protected:
		GLuint textureID;
		int texWidth, texHeight, texChannels;
		void setup();
	};
}