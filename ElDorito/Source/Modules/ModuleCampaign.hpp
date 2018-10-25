#pragma once

#include "ModuleBase.hpp"

namespace Modules
{
	class ModuleCampaign : public Utils::Singleton<ModuleCampaign>, public ModuleBase
	{
	public:
		Command* VarDifficultyLevel;
		Command* VarDifficultyLevelClient;
		Command* VarInsertionPoint;
		Command* VarInsertionPointClient;
		Command* VarMetagameScoringOption;
		Command* VarMetagameScoringOptionClient;
		Command* VarMetagameEnabled;
		Command* VarMetagameEnabledClient;
		Command* VarSurvivalModeEnabled;
		Command* VarSurvivalModeEnabledClient;

		ModuleCampaign();
	};
}