#pragma once
#include <cstdint>
#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Text\StringID.hpp"

namespace Bungie::Tags::UI
{
	using Bungie::Tags::TagBlock;
	using Bungie::Tags::TagData;
	using Bungie::Tags::TagGroup;
	using Bungie::Text::StringID;

	struct MultilingualUnicodeStringList : TagGroup<'unic'>
	{
		struct LocalizedString;

		TagBlock<LocalizedString> Strings;
		TagData<int8_t> Data;

		struct LocalizedString
		{
			StringID StringID;
			char StringIDStr[32]; //This should really be stored as a string.
			int32_t Offsets[12];
		};
		TAG_STRUCT_SIZE_ASSERT(LocalizedString, 0x54);
	};
}
