#include "Campaign.hpp"
#include "../Patch.hpp"
#include "../ElDorito.hpp"
#include "../Blam/BlamTypes.hpp"

namespace
{
	void game_insertion_point_get_hook();
	int insertionPoint;
}

namespace Patches::Campaign
{
	void ApplyAll() 
	{
		Hook(0x345C9B, game_insertion_point_get_hook, HookFlags::IsCall).Apply();
	}

	void SetInsertion(int insertion)
	{
		insertionPoint = insertion;
	}
}

namespace
{
	__declspec(naked) void game_insertion_point_get_hook() {
		__asm {
			mov eax, insertionPoint
			ret
		}
	}
}
