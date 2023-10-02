#pragma once
#include "Bungie\Tags\Tag.hpp"
#include "Bungie\Tags\TagBlock.hpp"
#include "Bungie\Tags\TagData.hpp"
#include "Bungie\Tags\TagGroup.hpp"
#include "Bungie\Tags\TagInstance.hpp"
#include "Bungie\Tags\TagReference.hpp"

// Asserts that a tag structure is the correct size.
#define //TAG_STRUCT_SIZE_ASSERT(type, size)           \
	static_assert                                    \
	(                                                \
		sizeof(type) == (size),                      \
		"Tag structure " #type " has incorrect size" \
	)