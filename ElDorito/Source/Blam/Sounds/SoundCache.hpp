#pragma once
#include "Bungie\Tags\Sounds\Sound\Sound.hpp"
#include "Bungie\Tags\Sounds\SoundClasses\SoundClasses.hpp"
#include "Bungie\Math\Random.hpp"
#include <math.h>
#include "Bungie\Memory\Align.hpp"
#include "Bungie\Sounds\SoundDatum.hpp"
#include "Bungie\Memory\DatumHandle.hpp"
#include "Bungie\Sounds\SoundGlobals.hpp"



namespace Bungie::SoundCache {

	using namespace Bungie::Tags::Sounds;

	//0x65FCB0
	int32_t AllocateSound(int32_t SoundTagIndex, PermutationChunk *Chunk, uint32_t a3);

	//0x6602B0
	int32_t AllocateSoundInArray(int32_t SoundTagIndex, PermutationChunk *Chunk, int8_t *a3);
}