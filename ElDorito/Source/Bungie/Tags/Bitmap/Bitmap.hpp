#pragma once

#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Math\Bounds.hpp"
#include "Bungie\Text\StringID.hpp"

#include "Bungie\Tags\Bitmap\TightBinding.hpp"
#include "Bungie\Tags\Bitmap\UsageOverride.hpp"
#include "Bungie\Tags\Bitmap\Sequence.hpp"
#include "Bungie\Tags\Bitmap\Image.hpp"
#include "Bungie\Tags\Bitmap\BitmapResource.hpp"

namespace Bungie::Tags::Bitmap {
	struct Bitmap : TagGroup<'bitm'>
	{
		int32_t Usage;
		int16_t BitmapRuntimeFlags;
		int16_t SpriteSpacing;
		float BumpMapHeight;
		float FadeFactor;
		int8_t BitmapCurveMode;
		uint8_t MaxMipMapLevel;
		int16_t MaxResolution;
		int16_t AtlasIndex;
		int16_t ForceBitmapFormat;
		TagBlock<TightBinding> TightBindingOld;
		TagBlock<UsageOverride> UsageOverrides;
		TagBlock<Sequence> ManualSequences;
		int8_t SourceData[0x14];
		int8_t ProcessedPixelData[0x14];
		TagBlock<Sequence> Sequences;
		TagBlock<Image> Images;
		int8_t XenonProcessedPixelData[0x14];
		TagBlock<Image> XenonImages;
		TagBlock<BitmapResource> Resources;
		TagBlock<BitmapResource> InterleavedResources;
		int32_t UnknownB4;

	};
	//TAG_STRUCT_SIZE_ASSERT(Bitmap, 0xB8);
}
