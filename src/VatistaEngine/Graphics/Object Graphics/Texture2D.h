#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include "Utilities/Utils.h"
#include "Texture.h"

namespace Vatista {
	class Texture2D : public Texture {
	public:
		SharedPtr(Texture2D);

		Texture2D();
		~Texture2D();

		void loadFile(const std::string& filename, bool alpha = true);

	protected:
		int texWidth, texHeight, texChannels;
		void setup();
	};
}