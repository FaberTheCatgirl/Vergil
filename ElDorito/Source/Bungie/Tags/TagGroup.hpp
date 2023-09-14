#pragma once
#include "Bungie\Tags\Tag.hpp"
#include "Definitions\StructDefinition.hpp"

namespace Bungie::Tags
{
	template <const Tag GroupTagValue>
	struct TagGroup
	{
		static const Tag GroupTag = GroupTagValue;
		static const Definitions::StructDefinition Definition;
	};
}
