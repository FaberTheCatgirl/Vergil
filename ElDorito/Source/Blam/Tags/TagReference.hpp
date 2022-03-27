#pragma once
#include <cstdint>
#include "Blam\Padding.hpp"
#include "Blam\Tags\Tag.hpp"

namespace Blam::Tags
{
	struct TagReference
	{
		Tag GroupTag;
		PAD32;
		PAD32;
		int32_t TagIndex;

		TagReference();
		TagReference(const Tag &groupTag, const int32_t tagIndex);

		bool operator==(const TagReference &other) const;
		bool operator!=(const TagReference &other) const;

		explicit operator bool() const;

		template <typename T>
		inline T *GetDefinition()
		{
			if (TagIndex == -1)
				return nullptr;

			return TagInstance(TagIndex).GetDefinition<T>();
		}
	};
}
