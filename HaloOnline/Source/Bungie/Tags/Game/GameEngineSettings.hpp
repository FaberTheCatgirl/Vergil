#pragma once
#include "Bungie\Padding.hpp"
#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Text\StringID.hpp"
#include "Bungie\Tags\Game\TraitsProfile.hpp"
#include "Bungie\Tags\Game\GameVariant.hpp"
#include "Bungie\Tags\Game\SlayerVariant.hpp"
#include "Bungie\Tags\Game\OddballVariant.hpp"
#include "Bungie\Tags\Game\CTFVariant.hpp"
#include "Bungie\Tags\Game\AssaultVariant.hpp"
#include "Bungie\Tags\Game\InfectionVariant.hpp"
#include "Bungie\Tags\Game\KOTHVariant.hpp"
#include "Bungie\Tags\Game\TerritoriesVariant.hpp"
#include "Bungie\Tags\Game\JuggernautVariant.hpp"
#include "Bungie\Tags\Game\VIPVariant.hpp"
#include "Bungie\Tags\Game\SandboxVariant.hpp"

namespace Bungie::Tags::Game
{
	using Bungie::Tags::Tag;
	using Bungie::Tags::TagBlock;
	using Bungie::Tags::TagData;
	using Bungie::Tags::TagGroup;
	using Bungie::Tags::TagReference;
	using Bungie::Text::StringID;

	struct GameEngineSettingsDefinition : TagGroup<'wezr'>
	{
		PAD32;
		TagBlock<TraitsProfile> TraitsProfiles;
		TagBlock<SlayerVariant> SlayerVariants;
		TagBlock<OddballVariant> OddballVariants;
		TagBlock<CTFVariant> CTFVariants;
		TagBlock<AssaultVariant> AssaultVariants;
		TagBlock<InfectionVariant> InfectionVariants;
		TagBlock<KOTHVariant> KOTHVariants;
		TagBlock<TerritoriesVariant> TerritoriesVariants;
		TagBlock<JuggernautVariant> JuggernautVariants;
		TagBlock<VIPVariant> VIPVariants;
		TagBlock<SandboxVariant> SandboxVariants;
		PAD32;
	};
	//TAG_STRUCT_SIZE_ASSERT(struct GameEngineSettingsDefinition, 0x8C);
}

