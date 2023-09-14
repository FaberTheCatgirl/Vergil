#include "Bungie\BlamObjects.hpp"
#include "ElDorito.hpp"

namespace Bungie::Objects
{
	DataArray<ObjectHeader>& GetObjects()
	{
		return *ElDorito::Instance().GetMainTls(0x448).Read<DataArray<ObjectHeader>*>();
	}
}
