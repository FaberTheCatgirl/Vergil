#pragma once

#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Math\Bounds.hpp"
#include "Bungie\Text\StringID.hpp"
#include "Bungie\Tags\Sounds\Sound\PermutationChunk.hpp"

namespace Bungie::Tags::Sounds
{
	struct Permutation
	{
		Text::StringID Name;
		Math::Bounds<float> SkipFraction;
		uint32_t SampleSize;
		uint32_t PermutationNumber;
		uint32_t IsNotFirstPermutation;
		TagBlock<PermutationChunk> PermutationChunks;
		uint32_t Unknown1;
		uint32_t Unknown2;
	};
	//TAG_STRUCT_SIZE_ASSERT(Permutation, 0x2C);
}