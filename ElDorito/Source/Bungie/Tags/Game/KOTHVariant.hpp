#pragma once
#include <cstdint>
#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Text\StringID.hpp"
#include "Bungie\Tags\Game\GameVariant.hpp"

namespace Bungie::Tags::Game
{
	using Bungie::Text::StringID;

	struct KOTHVariant : GameVariant
	{
		int Unknown58;
		int Unknown5C;
		int Unknown60;
		int Unknown64;
		int Unknown68;
		int Unknown6C;
	};
	//TAG_STRUCT_SIZE_ASSERT(struct KOTHVariant, 0x70);
}
