#include "Blam\BlamObjects.hpp"
#include "HaloOnline.hpp"

namespace Blam::Objects
{
	DataArray<ObjectHeader>& GetObjects()
	{
		return *HaloOnline::Instance().GetMainTls(0x448).Read<DataArray<ObjectHeader>*>();
	}
}
