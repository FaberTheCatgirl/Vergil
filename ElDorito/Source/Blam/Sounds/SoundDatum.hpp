#pragma once
#include <stdint.h>
#include "../Tags/Sounds/Sound.hpp"
#include "../BlamData.hpp"


using namespace Blam::Tags::Sounds;

namespace Blam {

	// Total size of 0xC
	struct XboxSoundDatum : DatumBase{
		int16_t NumberTimesRequested;
		int32_t SoundTagIndex;
		Sound::PermutationChunk *Chunk;
	};

	// Total size of 0xC8
	struct SoundDatum : DatumBase {
		//0x0 used for the salt

		//0x2
		int8_t Flags6;	//0x2 Means loop

		//0x4
		int32_t Flags4;

		//0x6
		int8_t Flags5;	//0x1 Means loop
		

		//0x8
		int8_t Flags;	//0x100 used for 

		// Known for sure

		//0x10
		int32_t SoundTagIndex;

		//0x14
		int32_t *SoundDefinition;

		//0x18
		int16_t SelfDataArrayIndex;

		//0x20
		int32_t *PitchRangePermutationBlock;

		//0x14 related to looping sound array datum
		//DatumHandle GlobalLoopingSoundArrayHandle;

		

		//Used with DistanceC and D.
		//0x1C
		int8_t Flags3;

		//0x1D
		int8_t Unknown8;

		

		//0x20 
		float Unknown4;;

		//0x24
		float Unknown5;

		//0x28
		float Unknown6;

		//0x58
		//Seems related to sncl

		//0x68
		int32_t Unknown1;

		//0x6C
		int8_t lsndTrackIndex;

		//0x6D
		int8_t Flags2;

		//0x6E
		int8_t What2;

		//0x74
		float What3;

		//0x78
		int32_t What4;

		//0x98
		int32_t What6;

		//0x9C
		int32_t What5;

		//0xA0
		int32_t Unknown2;

		//0xA4
		int32_t Unknown3;

		//0xAC
		int8_t PitchRangeIndex;

		//0xAD
		int8_t PermutationIndex;

		//0xAE
		int16_t ChunkIndex;

		//0xB1
		int8_t What;

		//0xB8
		int16_t UnknownDWORDSoundGlobalIndex;

		// sub_519B20

		//0xBA
		int8_t What9;

		//0xBB
		int8_t What10;

		//0xBC
		int32_t What11;

		//0xC0
		int32_t What7;

		//0xC4
		int32_t What8;

	};

	// Total size of 0x128
	struct LoopingSoundDatum : DatumBase {
		//0x4
		int32_t Unknown3;

		//0xC
		int32_t Unknown2;


		//0x14
		int32_t Flags2;

		//0x18
		float Unknown4;

		//0x54
		float Unknown1;

		//0x60
		int8_t Flags1;

		//0x64
		int32_t PlaybackControllerArrayIndex;

	};

	// Total size of 0x10
	struct SoundEffectDatum : DatumBase {

	};

	// Total size of 0x180
	struct SoundTrackerDatum : DatumBase {

	};
	 
	// Total size of 0x1C
	struct SoundPlaybackController : DatumBase {
		
		
	};


	// Total size of 0x38. Called dword_238E85C
	struct SoundDataUnknown {

		// Known
		//0x0 
		int16_t SoundData0xC8Index;

		//0x14
		int16_t SoundState; // Used in the Halo Sound System class

		//0x18
		float Unknown;






		//0x11
		int8_t Flags;

		

		//0x28
		int32_t SoundTagIndex;

		//0x30
		int32_t Unknown1; //used in an if statement

		//0x2E
		int16_t ChunkIndex;
	};
}
