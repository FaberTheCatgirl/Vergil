#pragma once
#include "Blam\Padding.hpp"
#include "Blam\Tags\Tags.hpp"
#include "Blam\Text\StringID.hpp"
#include "Blam\Tags\Game\TraitsProfile.hpp"
#include "Blam\Tags\Game\GameVariant.hpp"
#include "Blam\Tags\Game\SlayerVariant.hpp"
#include "Blam\Tags\Game\OddballVariant.hpp"
#include "Blam\Tags\Game\CTFVariant.hpp"
#include "Blam\Tags\Game\AssaultVariant.hpp"
#include "Blam\Tags\Game\InfectionVariant.hpp"
#include "Blam\Tags\Game\KOTHVariant.hpp"
#include "Blam\Tags\Game\TerritoriesVariant.hpp"
#include "Blam\Tags\Game\JuggernautVariant.hpp"
#include "Blam\Tags\Game\VIPVariant.hpp"
#include "Blam\Tags\Game\SandboxVariant.hpp"

namespace Blam::Tags::Game
{
	using Blam::Tags::Tag;
	using Blam::Tags::TagBlock;
	using Blam::Tags::TagData;
	using Blam::Tags::TagGroup;
	using Blam::Tags::TagReference;
	using Blam::Text::StringID;

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

