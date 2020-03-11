#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include "Utilities/Utils.h"
#include "Texture.h"
#include "Utilities/TextureEnums.h"

namespace Vatista {

	struct Texture1DDesc {

		//The width or height of the texture, in texels
		uint32_t       Width;


		/*
				 number of samples to use if multisampling for texture
		 Default is 1
				*/
		uint32_t       NumSamples;

		//The number of mip levels to generate 1 will disable mipmapping default 0
		uint32_t       MipmapLevels;


		InternalFormat Format;

		//horrizontal texture axis wrap
		WrapMode       WrapS;

		//The wrap parameter for the vertical texture axis

		WrapMode       WrapT;

		MinFilter      MinFilter;

		MagFilter      MagFilter;

		/*
			The default amount of anisotropic filtering on this texture
			default 1.0f
		*/
		float          Anisotropic;

		/*
				 * The default border color for this texture when using WrapMode::ClampToBorder, and no
				 * sampler is specified
				 * @default (0,0,0,1)
				 */
		glm::vec4      BorderColor;

		Texture1DDesc() :
			Width(0),
			NumSamples(1), // NEW
			MipmapLevels(0),
			Format(InternalFormat::RGBA8),
			WrapS(WrapMode::Repeat), WrapT(WrapMode::Repeat),
			MinFilter(MinFilter::NearestMipLinear),
			MagFilter(MagFilter::Linear),
			Anisotropic(1.0f),
			BorderColor({ 0.0f, 0.0f, 0.0f, 1.0f }) {}

	};


	class Texture1D : public Texture {
	public:
		SharedPtr(Texture1D);

		Texture1D();
		Texture1D(const Texture1DDesc& description);
		~Texture1D();

		void loadFile(const std::string& filename, bool alpha = true);

	protected:
		int texWidth, texHeight, texChannels;
		Texture1DDesc texDesc;
		TextureSampler sample = TextureSampler();
		void setup();
	};

}