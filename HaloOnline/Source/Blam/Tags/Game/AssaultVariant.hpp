#pragma once
#include <cstdint>
#include "Blam\Tags\Tags.hpp"
#include "Blam\Text\StringID.hpp"
#include "Blam\Tags\Game\GameVariant.hpp"

namespace Blam::Tags::Game
{
	using Blam::Text::StringID;

	struct AssaultVariant : GameVariant
	{
		int Unknown58;
		int Unknown5C;
		int Unknown60;
		int Unknown64;
		int Unknown68;
		int Unknown6C;
		int Unknown70;
		int Unknown74;
		int Unknown78;
		int Unknown7C;
	};
	//TAG_STRUCT_SIZE_ASSERT(struct AssaultVariant, 0x80);
}

