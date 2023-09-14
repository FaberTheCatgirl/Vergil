#include "Bungie\Tags\Tags.hpp"

namespace Bungie::Tags::Bitmap {
	struct BitmapResource
	{
		int test;
	};
	TAG_STRUCT_SIZE_ASSERT(BitmapResource, 0x4);
}