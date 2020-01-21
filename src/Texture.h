#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include "Utils.h"

namespace Vatista {
	class Texture {
	public:
		SharedPtr(Texture);

		Texture();
		~Texture();

		void bind(int slot) const;
		static void unbind(int slot);

		void loadFile(const std::string& filename, bool alpha = true);

		GLuint getTexID() { return texture; };

	private:
		int texWidth, texHeight, texChannels;
		GLuint texture;
		void setup();
	};
}