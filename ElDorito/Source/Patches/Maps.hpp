#pragma once

#include "Bungie\BlamTypes.hpp"

namespace Patches::Maps
{
	void ApplyAll();
	void InitializeMapVariant(Bungie::MapVariant *mapv, int mapid);

	void StartupForceLoad(int type, std::string name);
}