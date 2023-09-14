#pragma once

#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Math\Bounds.hpp"

namespace Bungie::Tags::Sounds
{
	struct PermutationChunk
	{
		int32_t Offset;
		int32_t EncodedSize;
		int32_t RuntimeIndex;
		int32_t UnknownA;
		int32_t UnknownSize;
	};
	//TAG_STRUCT_SIZE_ASSERT(PermutationChunk, 0x14);
}