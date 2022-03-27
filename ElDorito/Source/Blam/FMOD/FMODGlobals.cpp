#include "Blam\FMOD\FMODGlobals.hpp"

namespace FMOD {

	void* GetSystemPointer() {
		return (void*) *((int *)0x0244F980 + 1);
	}

}