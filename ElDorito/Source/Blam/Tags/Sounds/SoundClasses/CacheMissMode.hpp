#pragma once
#include "Blam\Tags\Tags.hpp"

namespace Blam::Tags::Sounds {
	enum class CacheMissMode : int8_t
	{
		Discard,
		Postpone
	};
}