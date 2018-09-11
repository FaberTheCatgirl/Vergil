#pragma once
#include "..\Tags\Sounds\Sound.hpp"
#include "..\Tags\Sounds\SoundClasses.hpp"
#include "..\Math\Random.hpp"
#include <math.h>
#include "..\Memory\Align.hpp"
#include "..\Sounds\SoundDatum.hpp"
#include "..\Memory\DatumHandle.hpp"


using namespace Blam::Tags::Sounds;

namespace Blam::SoundCache {

	//0x65FCB0
	int32_t AllocateSound(int32_t SoundTagIndex, Sound::PermutationChunk *Chunk, uint32_t a3);

	//0x6602B0
	int32_t AllocateSoundInArray(int32_t SoundTagIndex, Sound::PermutationChunk *Chunk, int8_t *a3);
}