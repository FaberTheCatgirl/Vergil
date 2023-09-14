#pragma once
#include "Bungie\Tags\Tags.hpp"

namespace Bungie::Tags::Sounds
{
	enum class OutputEffect : int16_t {
		None,
		OutputFrontSpeakers,
		OutputRearSpeakers,
		OutputCenterSpeakers
	};
}