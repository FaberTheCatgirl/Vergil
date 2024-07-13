#pragma once

#include "Modules\ModuleBase.hpp"

namespace Modules
{
	class ModulePlayer : public Utils::Singleton<ModulePlayer>, public ModuleBase
	{
	public:
		Command* VarRepresentation;

		Command* VarArmorHelmet;
		Command* VarArmorChest;
		Command* VarArmorRightShoulder;
		Command* VarArmorLeftShoulder;
		Command* VarArmorArms;
		Command* VarArmorLegs;
		Command* VarArmorPelvis;
		Command* VarArmorUpperBody;

		Command* VarColorsPrimary;
		Command* VarColorsSecondary;
		Command* VarColorsVisor;
		Command* VarColorsLights;
		Command* VarColorsHolo;
		Command* VarPlayerName;
		Command* VarPlayerGender;
		Command* VarPlayerServiceTag;
		Command* VarPlayerPrivKey;
		Command* VarPlayerPubKey;

		wchar_t UserName[17];

		ModulePlayer();

		static bool ValidServiceTag(const std::string &serviceTag);
	};
}
