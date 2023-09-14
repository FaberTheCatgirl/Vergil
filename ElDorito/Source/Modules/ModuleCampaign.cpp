#include "Modules\ModuleCampaign.hpp"

#include "Bungie\BlamTypes.hpp"
#include "Bungie\Memory\TlsData.hpp"

#include "Pointer.hpp"

namespace
{
	bool VariableFrameLimitUpdate(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		auto *tls = (Bungie::Memory::tls_data *)ElDorito::Instance().GetMainTls();
		auto gameGlobalsPtr = tls->game_globals;
		if (!gameGlobalsPtr)
			return false;

		// these changes take effect on map load
		if (gameGlobalsPtr->game_options.MapType == Bungie::eMapTypeCampaign)
			if (Pointer(0x165C83C).Read<int>() != Modules::ModuleCampaign::Instance().VarFrameLimit->ValueInt)
				Pointer(0x165C83C).Write<int>(Modules::ModuleCampaign::Instance().VarFrameLimit->ValueInt);
		else
			if (Pointer(0x165C83C).Read<int>() != 60)
				Pointer(0x165C83C).Write<int>(60);

		return true;
	}
}

namespace Modules
{
	ModuleCampaign::ModuleCampaign() : ModuleBase("Campaign")
	{
		VarFrameLimit = AddVariableInt("FrameLimit", "campaign_frame_limit", "Changes the frame limit.", eCommandFlagsNone, 60, VariableFrameLimitUpdate);
		VarFrameLimit->ValueIntMin = 30;
		VarFrameLimit->ValueIntMax = 144;

		VarDifficultyLevel = AddVariableInt("DifficultyLevel", "campaign_difficulty", "Changes the difficulty level.", eCommandFlagsNone, Bungie::eCampaignDifficultyLevelNormal);
		VarDifficultyLevel->ValueIntMin = Bungie::eCampaignDifficultyLevelEasy;
		VarDifficultyLevel->ValueIntMax = Bungie::eCampaignDifficultyLevelCount-1;

		VarInsertionPoint = AddVariableInt("InsertionPoint", "campaign_insertion_point", "Changes the current insertion point.", eCommandFlagsNone, Bungie::eCampaignInsertionPointMissionStart);
		VarInsertionPoint->ValueIntMin = Bungie::eCampaignInsertionPointMissionStart;
		VarInsertionPoint->ValueIntMax = Bungie::eCampaignInsertionPointRallyPointCount-1;

		VarMetagameScoringOption = AddVariableInt("MetagameScoringOption", "campaign_metagame_scoring_option", "Changes the metagame scoring options.", eCommandFlagsNone, Bungie::eCampaignMetagameScoringOptionOff);
		VarMetagameScoringOption->ValueIntMin = Bungie::eCampaignMetagameScoringOptionOff;
		VarMetagameScoringOption->ValueIntMax = Bungie::eCampaignMetagameScoringOptionCount-1;

		VarMetagameEnabled = AddVariableInt("MetagameEnabled", "campaign_metagame_enabled", "Enables or disables metagame.", eCommandFlagsNone, false);
		VarMetagameEnabled->ValueIntMin = false;
		VarMetagameEnabled->ValueIntMax = true;

		VarSurvivalModeEnabled = AddVariableInt("SurvivalModeEnabled", "campaign_survival_mode_enabled", "Enables or disables firefight.", eCommandFlagsNone, false);
		VarSurvivalModeEnabled->ValueIntMin = false;
		VarSurvivalModeEnabled->ValueIntMax = true;
	}
}