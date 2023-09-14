#include "Bungie\Tags\Globals\CacheFileGlobalTags.hpp"
#include "Definitions\EnumDefinition.hpp"

using namespace Bungie::Tags::Globals;
using namespace Definitions;

namespace Bungie::Tags
{
	const StructDefinition GlobalsTagStruct =
	{
		"GlobalsTag", sizeof(CacheFileGlobalTags::GlobalsTag),
		{
			{ FieldType::TagReference, "Tag" }
		}
	};

	const StructDefinition TagGroup<CacheFileGlobalTags::GroupTag>::Definition =
	{
		"CacheFileGlobalTags", sizeof(CacheFileGlobalTags),
		{
			{ FieldType::Block, "GlobalsTags", &GlobalsTagStruct },
			{ FieldType::LongInteger, "Unknown" }
		}
	};
}