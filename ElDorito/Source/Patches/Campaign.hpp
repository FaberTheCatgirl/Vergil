#pragma once

namespace
{
	extern int g_difficulty_level;
	extern int g_insertion_point;
	extern int g_metagame_scoring_option;
	extern int g_metagame_enabled;
	extern int g_survival_mode_enabled;
}

namespace Patches::Campaign
{
	void ApplyAll();
	void SetDifficultyLevel(int difficulty_level);
	void SetInsertionPoint(int insertion_point);
	void SetMetagameScoringOption(int metagame_scoring_option);
	void SetMetagameEnabled(bool metagame_enabled);
	void SetSurvivalModeEnabled(bool survival_mode_enabled);
};