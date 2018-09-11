#include "Functions.hpp"

namespace Blam::Sounds
{
	void* tag_get(uint32_t tagGroup, int16_t tagIndex)
	{
		const auto sub_503370 = (int32_t(*)(uint32_t tag_group, int16_t tag_index))(0x503370);
		int32_t address = sub_503370(tagGroup, tagIndex);
		return reinterpret_cast<void*>(address);
	};

	int32_t *global_sound_sample_rates = reinterpret_cast<int32_t*>(0x400000 + 0x0166B1F0);

	Classes::Unknown* sound_class_get(int16_t type)
	{
		const auto sub_660CC0 = (Classes::Unknown*(*)(int16_t type))(0x660CC0);
		Classes::Unknown* soundClass = sub_660CC0(type);
		return soundClass;
	}

	double sound_sub_661D70(int16_t soundTagIndex, int32_t pitchRangeIndex)
	{
		Sound* sound = (Sound*)tag_get('snd!', soundTagIndex);
		Sound::PitchRange pitchRange = sound->PitchRanges[pitchRangeIndex];
		float unknown = pitchRange.Unknown3;
		if (pitchRangeIndex == -1 || pitchRange.Name == -20 || unknown == 0.0)
		{
			if ((((int8_t)sound->OverrideFlags) >> 1) & 1)
			{
				Classes::Unknown* soundClass = sound_class_get((int16_t)sound->SoundClass);
				return soundClass->Unknown15;
			}
			unknown = sound->DistanceB;
		}
		return unknown;


	};

	int8_t sound_get_sample_rate(Sound* sound)
	{
		return (int8_t)(sound->SampleRate);
	}

	int16_t sound_get_pitch_range_by_pitch(Sound* sound, float pitchModifier, int16_t fallbackIndex)
	{
		bool hasFlag14 = (sound->Flags) & 0x4000 == 0;
		auto pitchRangeCount = sound->PitchRanges.Count;
		auto curPitchRangeIndex = -1;
		auto failure = -1;


		if (!hasFlag14)
		{
			curPitchRangeIndex = 0;

			if (pitchRangeCount > 0)
			{
				auto curPitchRange = sound->PitchRanges[0];
				while (curPitchRange.Name != 1236) // "lfe"
				{
					curPitchRangeIndex++;
					curPitchRange = sound->PitchRanges[curPitchRangeIndex];
					if (curPitchRangeIndex >= pitchRangeCount)
					{
						goto lfe_NOT_FOUND;
					}
				}
			}

		lfe_NOT_FOUND:

			//Will return if the fallback Index satisfies the right conditions
			if (fallbackIndex != -1 && fallbackIndex < pitchRangeCount)
			{
				auto curPitchRange = sound->PitchRanges[fallbackIndex];
				if ((curPitchRangeIndex == fallbackIndex || pitchModifier >= curPitchRange.MaxGainPitch.Lower && pitchModifier <= curPitchRange.MaxGainPitch.Upper) && curPitchRange.Unknown7>0)
				{
					return fallbackIndex;
				}
				return failure;
			}

			float max = FLT_MAX;
			auto result = 0;

			if (pitchRangeCount < 0)
				return failure;

			auto curIndex = 0;
			auto curPitchRange = sound->PitchRanges[curIndex];
			auto bendsMax = curPitchRange.Bend.Upper;
			auto bendsMin = 0;

			while (curPitchRangeIndex == curIndex || curPitchRange.PermutationCount <= 0)
			{
			INCR_INDEX:
				curIndex++;
				curPitchRange = sound->PitchRanges[curIndex];
				if (curIndex >= pitchRangeCount)
					return failure;
			}

			if (pitchModifier < curPitchRange.MaxGainPitch.Lower || curPitchRange.MaxGainPitch.Upper < pitchModifier)
			{
				bendsMax = curPitchRange.Bend.Upper;
				if (pitchModifier <= bendsMax)
					bendsMin = curPitchRange.Bend.Lower - pitchModifier;
				else
					bendsMin = pitchModifier - bendsMax;

				if (max > bendsMin)
				{
					failure = curIndex;
					max = bendsMin;
				}
				goto INCR_INDEX;
			}

			//Verify that curIndex != -1 

			return curIndex;
		}




	};

	Sound::PitchRange* sound_get_pitch_range(Sound* sound, int32_t pitchRangeIndex)
	{
		return &sound->PitchRanges[pitchRangeIndex];
	}

	int32_t sound_get_pitch_range_count(Sound* sound)
	{
		return sound->PitchRanges.Count;
	}

	Sound::Permutation* sound_get_permutation(Sound::PitchRange* pitchRange, int32_t permutationIndex)
	{
		return &pitchRange->Permutations[permutationIndex];
	}

	int32_t sound_get_permutation_count(Sound::PitchRange* pitchRange)
	{
		return pitchRange->Permutations.Count;
	};

	Sound::PermutationChunk* sound_get_permutation_chunk(Sound::Permutation* permutation, int32_t chunkIndex)
	{
		return &permutation->PermutationChunks[chunkIndex];
	};

	bool sound_is_valid_pitch_range_permutation(int32_t soundTagIndex, int8_t* data)
	{
		char pitchRangeIndex = *data;
		char permutationIndex = *(data + 1);

		if (soundTagIndex == -1 || pitchRangeIndex < 0 || permutationIndex < 0)
			return false;

		auto sound = (Blam::Tags::Sounds::Sound*)tag_get('snd!', soundTagIndex);

		if (pitchRangeIndex >= sound_get_pitch_range_count(sound))
			return false;

		auto pitchRange = sound_get_pitch_range(sound, pitchRangeIndex);
		auto permutationCount = sound_get_permutation_count(pitchRange);

		if (permutationIndex < permutationCount)
			return true;

		return false;
	};

	//0x662A40 not sure of the name
	double sound_compute_pitch_modifier_min(Sound* sound, Sound::PitchRange* pitchRange, uint32_t SampleSize)
	{
		auto pitchRandomMin = sound->RandomPitch.Upper;

		if (sound->RandomPitch.Lower <= pitchRandomMin)
			pitchRandomMin = sound->RandomPitch.Lower;

		auto PitchModifierMin = sound->PitchModifier.Upper;

		if (sound->PitchModifier.Lower <= PitchModifierMin)
			PitchModifierMin = sound->PitchModifier.Lower;

		double naturalPitchFactor = exp((pitchRange->NaturalPitch / 1200.0)*0.69314718);
		double SampleSizeFactor = exp((double)SampleSize);

		double result = (SampleSizeFactor*1000.0*naturalPitchFactor) / (((pitchRandomMin + pitchRandomMin) / 1200.0)*0.69314718 * global_sound_sample_rates[(int)sound->SampleRate]);
		return result;
	}

	//0x662120
	double unknown(int32_t soundTagIndex, int32_t pitchRangeIndex, int32_t permutationIndex, float parameter)
	{
		auto sound = (Sound*)tag_get('snd!', soundTagIndex);
		auto pitchRange = sound_get_pitch_range(sound, pitchRangeIndex);
		auto permutation = sound_get_permutation(pitchRange, permutationIndex);

		float permutationPitchModifierMin = sound_compute_pitch_modifier_min(sound, pitchRange, permutation->SampleSize);
		float naturalPitchModifier = exp(parameter - pitchRange->NaturalPitch / 1200.0f * 0.69314718f);

		return naturalPitchModifier * permutationPitchModifierMin;

	};

	//Find better names
	double sound_get_distance_d(int16_t soundTagIndex)
	{
		Sound* sound = (Sound*)tag_get('snd!', soundTagIndex);
		if (((int8_t)(sound->PlaybackParameterFlags) >> 3 & 1))
		{
			Classes::Unknown* soundClass = sound_class_get((int16_t)sound->SoundClass);
			return soundClass->DistanceBoundsMax;
		}
		else
			return sound->DistanceD;
	}

	double sound_get_distance_c(int16_t soundTagIndex)
	{
		Sound* sound = (Sound*)tag_get('snd!', soundTagIndex);
		if (((int8_t)(sound->PlaybackParameterFlags) >> 2 & 1))
		{
			Classes::Unknown* soundClass = sound_class_get((int16_t)sound->SoundClass);
			return soundClass->DistanceBoundsMin;
		}
		else
			return sound->DistanceC;
	}

	Math::Bounds<int16_t>* sound_get_bend_bounds(Sound::PitchRange* pitchRange)
	{
		return &(pitchRange->Bend);
	};

	int32_t sound_get_next_permutation_chunk_index(Sound::Permutation* permutation, int16_t currentChunkIndex)
	{
		if (permutation && currentChunkIndex < permutation->PermutationChunks.Count - 1)
			return currentChunkIndex + 1;
		else
			return -1;

	};

	void PredictFirstPermutation(int16_t soundTagIndex) {
		if (soundTagIndex != -1) {
			Sound *sound = (Sound*)tag_get('snd!', soundTagIndex);
			if (sound_get_pitch_range_count(sound)) {
				Sound::PitchRange *pitchRange = sound_get_pitch_range(sound, 0);
				if (sound_get_permutation_count(pitchRange)) {
					Sound::Permutation *permutation = sound_get_permutation(pitchRange, 0);
					Sound::PermutationChunk *chunk = sound_get_permutation_chunk(permutation, 0);
					AllocateSound(soundTagIndex, chunk, 1);
				}
			}
		}
	}
}
