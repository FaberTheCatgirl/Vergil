#pragma once

#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Math\Bounds.hpp"

namespace Bungie::Tags::Sounds
{
	struct Scale {
		Math::Bounds<float> GainModifier;
		Math::Bounds<int16_t> PitchModifier;
		Math::Bounds<float> SkipFractionModifier;
	};
	TAG_STRUCT_SIZE_ASSERT(Scale, 0x14);
}