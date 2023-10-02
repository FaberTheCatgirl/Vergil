#include "Bungie\BungieObjects.hpp"
#include "HaloOnline.hpp"

namespace Bungie::Objects
{
	DataArray<ObjectHeader>& GetObjects()
	{
		return *HaloOnline::Instance().GetMainTls(0x448).Read<DataArray<ObjectHeader>*>();
	}
}
