#pragma once

#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Math\Bounds.hpp"

namespace Bungie::Tags::Sounds
{
	struct RuntimeTimer
	{
		int32_t Unknown;
	};
	TAG_STRUCT_SIZE_ASSERT(RuntimeTimer, 0x4);
}