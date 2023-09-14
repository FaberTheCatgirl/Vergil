#include "Bungie\BlamGraphics.hpp"

namespace Bungie::Graphics
{
	wchar_t* TakeScreenshot()
	{
		typedef wchar_t *(*TakeScreenshotPtr)();
		auto TakeScreenshotImpl = reinterpret_cast<TakeScreenshotPtr>(0x60F420);
		return TakeScreenshotImpl();
	}
}