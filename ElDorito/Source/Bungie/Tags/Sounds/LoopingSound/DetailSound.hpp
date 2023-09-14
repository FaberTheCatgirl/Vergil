#pragma once

#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Math\Bounds.hpp"
#include "Bungie\Text\StringID.hpp"

namespace Bungie::Tags::Sounds
{
	struct DetailSound {
		Text::StringID Name;
		TagReference Sound;
		Math::Bounds<float> RandomPeriodBounds;
		float Unknown1;
		int32_t Flags;
		Math::Bounds<float> YawBounds;
		Math::Bounds<float> PitchBounds;
		Math::Bounds<float> DistanceBounds;
	};
	//TAG_STRUCT_SIZE_ASSERT(DetailSound, 0x3C);
}