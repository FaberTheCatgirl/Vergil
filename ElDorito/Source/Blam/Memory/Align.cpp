#include "Bungie\Memory\Align.hpp"

namespace Bungie::Memory {
	int32_t Align16Bits(int32_t value) {
		int32_t result = value;
		if (value & 0xF)
			result = (value | 0xF) + 1;
		return result;
	}
}