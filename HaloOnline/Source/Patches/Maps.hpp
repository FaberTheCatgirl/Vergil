#pragma once

#include "Bungie\BungieTypes.hpp"

namespace Patches::Maps
{
	void ApplyAll();
	void InitializeMapVariant(Bungie::MapVariant *mapv, int mapid);

	void StartupForceLoad(int type, std::string name);
}