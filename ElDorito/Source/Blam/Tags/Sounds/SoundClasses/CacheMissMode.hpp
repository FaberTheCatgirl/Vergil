#pragma once
#include "Bungie\Tags\Tags.hpp"

namespace Bungie::Tags::Sounds {
	enum class CacheMissMode : int8_t
	{
		Discard,
		Postpone
	};
}