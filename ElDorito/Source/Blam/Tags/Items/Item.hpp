#pragma once
#include "Bungie\Math\Bounds.hpp"
#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Text\StringID.hpp"
#include "Bungie\Tags\Objects\Object.hpp"

namespace Bungie::Tags::Items
{
	using Bungie::Math::Bounds;
	using Bungie::Tags::TagBlock;
	using Bungie::Tags::TagGroup;
	using Bungie::Tags::TagReference;
	using Bungie::Text::StringID;
	using Bungie::Tags::Objects::Object;

	struct Item : TagGroup<'item'>
	{
		Object Object;

		enum class Flags : int32_t;
		struct PredictedBitmap;

		Item::Flags ItemFlags : 32;
		PAD16; // old message index
		int16_t SortOrder;
		PAD32; // old multiplayer on-ground scale
		PAD32; // old single player on-ground scale
		StringID PickupMessage;
		StringID SwapMessage;
		StringID PickupOrDualWieldMessage;
		StringID SwapOrDualWieldMessage;
		StringID PickedUpMessage;
		StringID SwitchToMessage;
		StringID SwitchToFromAIMessage;
		StringID AllWeaponsEmptyMessage;
		TagReference CollisionSound;
		TagBlock<Item::PredictedBitmap> PredictedBitmaps;
		TagReference DetonationDamageEffect;
		Bounds<float> DetonationDelayBounds;
		TagReference DetonatingEffect;
		TagReference DetonationEffect;
		float GroundScaleSP;
		float GroundScaleMP;
		float SmallHoldScale;
		float SmallHolsterScale;
		float MediumHoldScale;
		float MediumHolsterScale;
		float LargeHoldScale;
		float LargeHolsterScale;
		float HugeHoldScale;
		float HugeHolsterScale;
		float GroundedFrictionLength;
		float GroundedFrictionUnknown;

		enum class Item::Flags : int32_t
		{
			None,
			AlwaysMaintainsZUp = 1 << 0,
			DestroyedByExplosions = 1 << 1,
			UnaffectedByGravity = 1 << 2
		};

		struct Item::PredictedBitmap
		{
			TagReference Bitmap;
		};
		TAG_STRUCT_SIZE_ASSERT(Item::PredictedBitmap, 0x10);
	};
	TAG_STRUCT_SIZE_ASSERT(Item, 0x1D4);
}

