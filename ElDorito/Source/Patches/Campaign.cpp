#include "Campaign.hpp"

#include "../Patch.hpp"
#include "../ElDorito.hpp"
#include "../Blam/BlamTypes.hpp"
#include "../Blam/BlamData.hpp"
#include "../Blam/BlamPlayers.hpp"
#include "../Blam/Memory/DatumHandle.hpp"
#include "../Blam/Memory/TlsData.hpp"

namespace
{
	int g_difficulty_level = Blam::eCampaignDifficultyLevelNormal;
	int g_insertion_point = Blam::eCampaignInsertionPointMissionStart;
	int g_metagame_scoring_option = Blam::eCampaignMetagameScoringOptionOff;
	int g_metagame_enabled = false;
	int g_survival_mode_enabled = false;

	void *__cdecl level_data_get();

	void __fastcall campaign_scoring_sub_6E59A0(char *scoreboard, void *, Blam::DatumHandle handle, int a3, short a4, int a5, char a6);
}

namespace Patches::Campaign
{
	void ApplyAll() 
	{
		Hook(0x2E59A0, campaign_scoring_sub_6E59A0).Apply();
		Hook(0x1322D0, level_data_get).Apply();
	}

	void SetDifficultyLevel(int difficulty_level)
	{
		g_difficulty_level = difficulty_level;
	}

	void SetInsertionPoint(int insertion_point)
	{
		g_insertion_point = insertion_point;
	}

	void SetMetagameScoringOption(int metagame_scoring_option)
	{
		g_metagame_scoring_option = metagame_scoring_option;
	}

	void SetMetagameEnabled(bool metagame_enabled)
	{
		g_metagame_enabled = metagame_enabled;
	}

	void SetSurvivalModeEnabled(bool survival_mode_enabled)
	{
		g_survival_mode_enabled = survival_mode_enabled;
	}
}

namespace
{
	void *__cdecl level_data_get()
	{
		auto *tls = (Blam::Memory::tls_data *)ElDorito::Instance().GetMainTls();
		auto level_data = &tls->game_globals->level_data;
		if (!level_data)
			return false;

		if (level_data->MapType == Blam::eMapTypeCampaign)
		{
			level_data->CampaignDifficultyLevel = (Blam::CampaignDifficultyLevel)g_difficulty_level;
			level_data->CampaignInsertionPoint = (Blam::CampaignInsertionPoint)g_insertion_point;
			level_data->CampaignMetagameScoringOption = (Blam::CampaignMetagameScoringOption)g_metagame_scoring_option;
			level_data->CampaignMetagameEnabled = g_metagame_enabled;
			level_data->SurvivalModeEnabled = g_survival_mode_enabled;
		}

		return (void *)level_data;
	}

	void __fastcall campaign_scoring_sub_6E59A0(char *scoreboard, void *, Blam::DatumHandle handle, int a3, short a4, int a5, char a6)
	{
		static const auto data_array_sub_55B710 = reinterpret_cast<unsigned long(__cdecl *)(Blam::DataArrayBase *, Blam::DatumHandle)>(0x55B710);
		static const auto game_get_current_engine = reinterpret_cast<int(*)()>(0x5CE150);
		static const auto game_is_team_game = reinterpret_cast<bool(__cdecl *)()>(0x5565E0);
		static const auto scoreboard_sub_6E5A90 = reinterpret_cast<void(__thiscall *)(char *, unsigned int, int, short, int)>(0x6E5A90);

		if (!scoreboard)
			return;

		auto *scoreboard_unknown = &scoreboard[2 * (a3 + 26 * handle.Index)];
		auto v7 = a4 + *((short *)scoreboard_unknown + 2);

		short v8;

		if (v7 <= -30000 || v7 < 30000)
		{
			v8 = -30000;
			if (v7 > -30000)
				v8 = a4 + *((short *)scoreboard_unknown + 2);
		}
		else
		{
			v8 = 30000;
		}

		*((short *)scoreboard_unknown + 2) = v8;

		auto v9 = 0;

		if (handle.Index >= 0x20u)
			v9 = 1 << *((char *)&handle.Handle);

		auto v10 = v9 ^ (1 << *((char *)&handle.Handle));

		if (handle.Index >= 0x40u)
			v9 ^= 1 << *((char *)&handle.Handle);

		auto v13 = v10;
		auto v14 = v9;

		((char(__cdecl *)(void *))0x4B2A70)(&v13);

		if (a5 != -1)
			((void(__cdecl *)(int, int, int, int))0x5704A0)(handle.Index, -1, a5, *((signed __int16 *)scoreboard_unknown + 2));

		if (a6)
		{
			if (game_get_current_engine() && game_is_team_game())
			{
				auto v11 = data_array_sub_55B710(ElDorito::Instance().GetMainTls(0x40).Read<Blam::DataArray<Blam::Players::PlayerDatum> *>(), handle);

				if (v11)
				{
					auto v12 = *(unsigned long *)(v11 + 0xC8);

					if (v12 >= 0 && v12 < 8)
						scoreboard_sub_6E5A90(scoreboard, v12, a3, a4, a5);
				}
			}
		}
	}
}
