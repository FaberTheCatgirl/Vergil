#pragma once

#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Tags\Sounds\Sound\PromotionRule.hpp"
#include "Bungie\Tags\Sounds\Sound\RuntimeTimer.hpp"

namespace Bungie::Tags::Sounds
{
	struct Promotion {
		TagBlock<PromotionRule> PromotionsRules;
		TagBlock<RuntimeTimer> RuntimeTimers;
	};
	//TAG_STRUCT_SIZE_ASSERT(Promotion, 0x18);
}