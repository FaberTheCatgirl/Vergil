#include "Patches\CharacterHud.hpp"
#include <cstdint>
#include "Patch.hpp"
#include "Bungie\Tags\Bitmap\Bitmap.hpp"
#include "Bungie\Tags\TagInstance.hpp"

namespace
{
	float* _cdecl GetSensorBlipBitmapSpriteOffset(int sequenceIndex, float* positionOffset);
}

namespace Patches::CharacterHud
{
	void ApplyAll()
	{
		Hook(0x6C5665, GetSensorBlipBitmapSpriteOffset, HookFlags::IsCall).Apply();
	}
}

namespace
{
	float* _cdecl GetSensorBlipBitmapSpriteOffset(int sequenceIndex, float* positionOffset) {

		int* chudPersistentGlobalData = reinterpret_cast<int*>(*reinterpret_cast<int*>(0x05257C40));	// go to 0x05257C40, obtain first int, convert to address

		int bitmapTagIndex = *(chudPersistentGlobalData + 0x30);

		if (chudPersistentGlobalData && bitmapTagIndex != -1) {
			Bungie::Tags::Bitmap::Bitmap bitmap = *Bungie::Tags::TagInstance(bitmapTagIndex).GetDefinition<Bungie::Tags::Bitmap::Bitmap>();
			if (sequenceIndex >= 0 && sequenceIndex < bitmap.Sequences.Count) {
				Bungie::Tags::Bitmap::Sequence sequence = bitmap.Sequences[sequenceIndex];
				if (sequence.Sprites.Count > 0) {
					Bungie::Tags::Bitmap::Sprite sprite = sequence.Sprites[0];
					positionOffset[0] = sprite.Left;
					positionOffset[1] = sprite.Right;
					positionOffset[2] = sprite.Top;
					positionOffset[3] = sprite.Bottom;
				}
				else
					goto default_position_offset;
			}
			else 
				goto default_position_offset;
		}
		else {
default_position_offset:
			positionOffset[0] = 0.0;
			positionOffset[1] = 1.0;
			positionOffset[2] = 0.0;
			positionOffset[3] = 1.0;
		}

		return positionOffset;
	}
}