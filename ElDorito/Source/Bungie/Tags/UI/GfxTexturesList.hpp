#pragma once
#include <cstdint>
#include "Bungie\Tags\Tags.hpp"

namespace Bungie::Tags::UI
{
	using Bungie::Tags::TagBlock;
	using Bungie::Tags::TagGroup;
	using Bungie::Tags::TagReference;

	struct GfxTexturesList : TagGroup<'gfxt'>
	{
		struct Texture;

		TagBlock<Texture> Textures;
		float Unknown;

		struct Texture
		{
			char FileName[256];
			TagReference Bitmap;
		};
		//TAG_STRUCT_SIZE_ASSERT(Texture, 0x110);
	};
}
