#pragma once  
#include "Bungie\Tags\Tags.hpp"  

namespace Bungie::Tags::Globals
{
	using Bungie::Tags::TagBlock;
	using Bungie::Tags::TagGroup;
	using Bungie::Tags::TagReference;

	struct CacheFileGlobalTags : TagGroup<'cfgt'>
	{
		struct GlobalsTag;

		TagBlock<GlobalsTag> GlobalsTags;
		int Unknown;

		struct GlobalsTag
		{
			TagReference Tag;
		};
		TAG_STRUCT_SIZE_ASSERT(GlobalsTag, 0x10);
	};
	TAG_STRUCT_SIZE_ASSERT(CacheFileGlobalTags, 0x10);
}