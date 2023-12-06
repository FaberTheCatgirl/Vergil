#pragma once

#include "Blam\Tags\Tags.hpp"
#include "Blam\Tags\Sounds\Sound\PromotionRule.hpp"
#include "Blam\Tags\Sounds\Sound\RuntimeTimer.hpp"

namespace Blam::Tags::Sounds
{
	struct Promotion {
		TagBlock<PromotionRule> PromotionsRules;
		TagBlock<RuntimeTimer> RuntimeTimers;
	};
	//TAG_STRUCT_SIZE_ASSERT(Promotion, 0x18);
}