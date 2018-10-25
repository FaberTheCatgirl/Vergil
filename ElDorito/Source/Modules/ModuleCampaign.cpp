#include "ModuleCampaign.hpp"

#include "../Blam/BlamTypes.hpp"
#include "../Patches/Campaign.hpp"
#include "../Server/VariableSynchronization.hpp"

namespace
{
	bool DifficultyLevelChanged(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		auto &campaignModule = Modules::ModuleCampaign::Instance();
		Patches::Campaign::SetDifficultyLevel(campaignModule.VarDifficultyLevel->ValueInt);
		return true;
	}
	bool InsertionPointChanged(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		auto &campaignModule = Modules::ModuleCampaign::Instance();
		Patches::Campaign::SetInsertionPoint(campaignModule.VarInsertionPoint->ValueInt);
		return true;
	}
	bool MetagameScoringOptionChanged(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		auto &campaignModule = Modules::ModuleCampaign::Instance();
		Patches::Campaign::SetMetagameScoringOption(campaignModule.VarMetagameScoringOption->ValueInt);
		return true;
	}
	bool MetagameEnabledChanged(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		auto &campaignModule = Modules::ModuleCampaign::Instance();
		Patches::Campaign::SetMetagameEnabled(campaignModule.VarMetagameEnabled->ValueInt);
		return true;
	}
	bool SurvivalModeEnabledChanged(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		auto &campaignModule = Modules::ModuleCampaign::Instance();
		Patches::Campaign::SetSurvivalModeEnabled(campaignModule.VarSurvivalModeEnabled->ValueInt);
		return true;
	}
}

namespace Modules
{
	ModuleCampaign::ModuleCampaign() : ModuleBase("Campaign")
	{
		VarDifficultyLevel = AddVariableInt("DifficultyLevel", "difficulty", "Difficulty Level", static_cast<CommandFlags>(eCommandFlagsArchived | eCommandFlagsReplicated), 0);
		VarDifficultyLevelClient = AddVariableInt("DifficultyLevelClient", "insertion_client", "", eCommandFlagsInternal, Blam::eCampaignDifficultyLevelNormal, DifficultyLevelChanged);
		Server::VariableSynchronization::Synchronize(VarDifficultyLevel, VarDifficultyLevelClient);

		VarInsertionPoint = AddVariableInt("InsertionPoint", "insertion", "Rally Point", static_cast<CommandFlags>(eCommandFlagsArchived | eCommandFlagsReplicated), 0);
		VarInsertionPointClient = AddVariableInt("InsertionPointClient", "insertion_client", "", eCommandFlagsInternal, Blam::eCampaignInsertionPointMissionStart, InsertionPointChanged);
		Server::VariableSynchronization::Synchronize(VarInsertionPoint, VarInsertionPointClient);

		VarMetagameScoringOption = AddVariableInt("MetagameScoringOption", "metagame_scoring_option", "Metagame Scoring Option", static_cast<CommandFlags>(eCommandFlagsArchived | eCommandFlagsReplicated), 0);
		VarMetagameScoringOptionClient = AddVariableInt("MetagameScoringOptionClient", "metagame_scoring_option_client", "", eCommandFlagsInternal, Blam::eCampaignMetagameScoringOptionOff, MetagameScoringOptionChanged);
		Server::VariableSynchronization::Synchronize(VarMetagameScoringOption, VarMetagameScoringOptionClient);

		VarMetagameScoringOption = AddVariableInt("MetagameEnabled", "metagame_enabled", "Rally Point", static_cast<CommandFlags>(eCommandFlagsArchived | eCommandFlagsReplicated), 0);
		VarMetagameScoringOptionClient = AddVariableInt("MetagameEnabledClient", "metagame_enabled_client", "", eCommandFlagsInternal, false, MetagameEnabledChanged);
		Server::VariableSynchronization::Synchronize(VarMetagameScoringOption, VarMetagameScoringOptionClient);

		VarMetagameScoringOption = AddVariableInt("SurvivalModeEnabled", "survival_mode_enabled", "Rally Point", static_cast<CommandFlags>(eCommandFlagsArchived | eCommandFlagsReplicated), 0);
		VarMetagameScoringOptionClient = AddVariableInt("SurvivalModeEnabledClient", "survival_mode_enabled_client", "", eCommandFlagsInternal, false, SurvivalModeEnabledChanged);
		Server::VariableSynchronization::Synchronize(VarMetagameScoringOption, VarMetagameScoringOptionClient);
	}
}