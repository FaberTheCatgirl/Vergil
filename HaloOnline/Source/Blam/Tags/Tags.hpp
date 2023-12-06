#pragma once
#include "Blam\Tags\Tag.hpp"
#include "Blam\Tags\TagBlock.hpp"
#include "Blam\Tags\TagData.hpp"
#include "Blam\Tags\TagGroup.hpp"
#include "Blam\Tags\TagInstance.hpp"
#include "Blam\Tags\TagReference.hpp"

// Asserts that a tag structure is the correct size.
//#define //TAG_STRUCT_SIZE_ASSERT(type, size)           \
//	static_assert                                    \
//	(                                                \
//		sizeof(type) == (size),                      \
//		"Tag structure " #type " has incorrect size" \
//	)