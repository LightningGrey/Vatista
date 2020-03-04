#pragma once
#include <glad/glad.h>
#include <Utilities/TextureEnums.h>
#include <GLM/glm.hpp>
#include "Utilities/Utils.h"


namespace Vatista {

	class TextureSampler
	{
	public:
		SharedPtr(TextureSampler);

		TextureSampler();
		~TextureSampler();

		void bind(uint32_t slot);
		static void unbind(uint32_t slot);
		void createSampler();

		//struct variables
		WrapMode wrapS = WrapMode::Repeat;
		WrapMode wrapT = WrapMode::Repeat;
		WrapMode wrapR = WrapMode::Repeat;

		MinFilter minFilter = MinFilter::NearestMipLinear;
		MagFilter magFilter = MagFilter::Linear;

		glm::vec4 borderColor = glm::vec4(0.0f);
		bool anisotropicEnabled = true;
		float maxAnisotropy = 1.0f;
	private:
		GLuint texHandle;
	};

}