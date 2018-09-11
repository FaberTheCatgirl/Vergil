#pragma once
#include "..\Tags\Sounds\Sound.hpp"
#include "..\Tags\Sounds\SoundClasses.hpp"
#include "..\Math\Random.hpp"
#include <math.h>
#include "SoundCache.hpp"

using namespace Blam::Tags::Sounds;
using namespace Blam::SoundCache;

namespace Blam::Sounds
{
	void* tag_get(uint32_t tagGroup, int16_t tagIndex);

	//0x661980
	int8_t sound_get_sample_rate(Sound* sound);

	//0x661C20
	int16_t sound_get_pitch_range_by_pitch(Sound* sound, float pitchModifier, int16_t fallbackIndex);

	//0x661D70
	double sound_sub_661D70(int16_t soundTagIndex, int32_t pitchRangeIndex);

	//0x661EF0
	double sound_get_distance_d(int16_t soundTagIndex);

	//0x661FC0
	double sound_get_distance_c(int16_t soundTagIndex);

	//0x6621B0
	Sound::PitchRange* sound_get_pitch_range(Sound* sound, int32_t pitchRangeIndex);

	//0x6621D0
	int32_t sound_get_pitch_range_count(Sound* sound);

	//0x662B40
	Sound::Permutation* sound_get_permutation(Sound::PitchRange* pitchRange, int32_t permutationIndex);

	//0x662C50, 0x662B60
	int32_t sound_get_permutation_count(Sound::PitchRange* pitchRange);

	//0x662890
	Sound::PermutationChunk* sound_get_permutation_chunk(Sound::Permutation* permutation, int32_t chunkIndex);

	//0x68CE40
	bool sound_is_valid_pitch_range_permutation(int32_t soundTagIndex, int8_t* data);

	//0x662A40
	double sound_compute_pitch_modifier_min(Sound* sound, Sound::PitchRange* pitchRange, uint32_t SampleSize);

	//0x662120
	double unknown(int32_t soundTagIndex, int32_t pitchRangeIndex, int32_t permutationIndex, float parameter);

	//0x662A30
	Math::Bounds<int16_t>* sound_get_bend_bounds(Sound::PitchRange* pitchRange);

	//0x6628F0
	int32_t sound_get_next_permutation_chunk_index(Sound::Permutation* permutation, int16_t currentChunkIndex);

	//0x515910
	void PredictFirstPermutation(int16_t soundTagIndex);


}
