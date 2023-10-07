#pragma once

#include "Modules\ModuleBase.hpp"

namespace Modules
{
	class HaloOnlineModules : public Utils::Singleton<HaloOnlineModules>, public ModuleBase
	{
	public:
		HaloOnlineModules();

	private:
	};
}