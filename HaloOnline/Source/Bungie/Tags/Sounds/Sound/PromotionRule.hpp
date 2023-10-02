#pragma once

#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Math\Bounds.hpp"

namespace Bungie::Tags::Sounds
{
	struct PromotionRule
	{
		int16_t PitchRangeIndex;
		int16_t MaximumPlayingCount;
		float SupressionTime;
		int32_t Unknown1;
		int32_t Unknown2;
	};
	//TAG_STRUCT_SIZE_ASSERT(PromotionRule, 0x10);
}