#pragma once
#include <cstdint>
#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Text\StringID.hpp"
#include "Bungie\Tags\Game\GameVariant.hpp"

namespace Bungie::Tags::Game
{
	using Bungie::Text::StringID;

	struct SandboxVariant : GameVariant
	{
		int Unknown58;
		int Unknown5C;
		int Unknown60;
	};
	TAG_STRUCT_SIZE_ASSERT(struct SandboxVariant, 0x64);
}

