#include "Utils\VersionInfo.hpp"

#define V(a, b, c, d) (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))
#define V1(v) (((v) >> 24) & 0xFF)
#define V2(v) (((v) >> 16) & 0xFF)
#define V3(v) (((v) >> 8) & 0xFF)
#define V4(v) ((v) & 0xFF)

#define CURRENT_VERSION V(3, 0, 0, 0)

#ifdef _DEBUG
#define VERSION_SUFFIX "-debug"
#elif !defined(ELDEWRITO_RELEASE)
#define VERSION_SUFFIX "-dev"
#else
#define VERSION_SUFFIX ""
#endif

namespace Utils::Version
{
	uint32_t GetVersionInt()
	{
		return CURRENT_VERSION;
	}

	std::string GetVersionString()
	{
		auto v = CURRENT_VERSION;
		return
			std::to_string(V1(v)) + "." +
			std::to_string(V2(v)) + "." +
			std::to_string(V3(v)) + "." +
			std::to_string(V4(v)) + VERSION_SUFFIX;
	}
}
