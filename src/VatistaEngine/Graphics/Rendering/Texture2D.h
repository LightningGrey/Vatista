#pragma once
//#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include "Utilities/Utils.h"
#include "Texture.h"
#include "Utilities/TextureEnums.h"

namespace Vatista {

struct Texture2DDesc{

	//The width or height of the texture, in texels
	uint32_t       Width, Height;


	/*
			 number of samples to use if multisampling for texture
	 Default is 1
			*/
	uint32_t       NumSamples;

	//The number of mip levels to generate 1 will disable mipmapping default 0
	uint32_t       MipmapLevels;


	InternalFormat Format;

	//horrizontal texture axis wrap
	WrapMode       WrapHorizontal;

	//The wrap parameter for the vertical texture axis

	WrapMode       WrapVertical;

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

	Texture2DDesc() :
		Width(0), Height(0),
		NumSamples(1), // NEW
		MipmapLevels(0),
		Format(InternalFormat::RGBA8),
		WrapHorizontal(WrapMode::Repeat), WrapVertical(WrapMode::Repeat),
		MinFilter(MinFilter::NearestMipLinear),
		MagFilter(MagFilter::Linear),
		Anisotropic(1.0f),
		BorderColor({ 0.0f, 0.0f, 0.0f, 1.0f }) {			}

};


	class Texture2D : public Texture {
	public:
		SharedPtr(Texture2D);

		Texture2D();
		~Texture2D();

		void loadFile(const std::string& filename, bool alpha = true);

	protected:
		int texWidth, texHeight, texChannels;
		TextureSampler sample = TextureSampler();
		void setup();
	};

}