#include "Patches\Tweaks.hpp"

#include "Blam\Tags\TagInstance.hpp"
#include "Blam\Tags\Camera\CameraFxSettings.hpp"
#include "Blam\Tags\Game\Globals.hpp"
#include "Blam\Tags\Game\MultiplayerGlobals.hpp"
#include "Blam\Tags\Globals\CacheFileGlobalTags.hpp"
#include "Blam\Tags\Models\Model.hpp"
#include "Blam\Tags\Objects\Biped.hpp"
#include "Blam\Tags\Objects\Projectile.hpp"
#include "Blam\Tags\Sounds\SoundClasses\SoundClasses.hpp"
#include "Blam\Tags\Scenario\Scenario.hpp"
#include "Blam\Tags\UI\ChudGlobalsDefinition.hpp"
#include "Blam\Tags\UI\ChudDefinition.hpp"
#include "Modules\ModuleTweaks.hpp"
#include "Modules\ModuleServer.hpp"
#include "Utils\Logger.hpp"

namespace
{
	bool tagsloaded = false;

	void EnableHitmarkersInternal(bool enabled);
	void EnableIntelBloomFix();
	void EnableReachStyleFrags();
	void EnableAggressiveAudioDiscarding();
}

namespace Patches::Tweaks
{
	void ApplyAfterTagsLoaded()
	{
		tagsloaded = true;

		if (Modules::ModuleTweaks::Instance().VarIntelBloomPatch->ValueInt)
		{
			EnableIntelBloomFix();
		}

		if (Modules::ModuleTweaks::Instance().VarAggressiveAudioDiscarding->ValueInt)
		{
			EnableAggressiveAudioDiscarding();
		}

		if (Modules::ModuleTweaks::Instance().VarReachStyleFrags->ValueInt)
		{
			EnableReachStyleFrags();
		}

		EnableHitmarkers(Modules::ModuleServer::Instance().VarHitMarkersEnabledClient->ValueInt != 0);

		UpdateUnitHUD();
	}

	void EnableHitmarkers(bool enabled)
	{
		EnableHitmarkersInternal(enabled);
	}

	void UpdateUnitHUD()
	{ 
		using namespace Blam::Tags; 
 
		using Blam::Tags::Objects::Biped; 
 
		auto masterChief = TagInstance::Find('bipd', "objects\\characters\\masterchief\\mp_masterchief\\mp_masterchief"); 
		if (masterChief.Index == 0xFFFF) 
			return; 
 
		auto masterChiefDefinition = masterChief.GetDefinition<Biped>(); 
		if (masterChiefDefinition->Unit.HudInterfaces.Count < 3) 
			return; 
 
		if (Modules::ModuleTweaks::ModuleTweaks::Instance().VarChudGlobalsIndex->ValueInt) 
			masterChiefDefinition->Unit.HudInterfaces[0].UnitHudInterface = masterChiefDefinition->Unit.HudInterfaces[2].UnitHudInterface; 
		else 
			masterChiefDefinition->Unit.HudInterfaces[0].UnitHudInterface = masterChiefDefinition->Unit.HudInterfaces[1].UnitHudInterface; 
	}
}

namespace
{
	using namespace Blam::Tags;

	Blam::Tags::Game::Globals *GetGlobalsDefinition()
	{
		auto matgTags = TagInstance::GetInstancesInGroup('matg');
		if (matgTags.size() < 1)
			return nullptr;
		return matgTags[0].GetDefinition<Blam::Tags::Game::Globals>();
	}

	void EnableIntelBloomFix()
	{
		using CameraFxSettings = Blam::Tags::Camera::FxSettings;

		auto cfxsTags = TagInstance::GetInstancesInGroup('cfxs');
		for (auto &cfxsTag : cfxsTags)
		{
			auto cfxsDefintion = cfxsTag.GetDefinition<CameraFxSettings>();
			if (cfxsDefintion)
				cfxsDefintion->Flags &= ~uint16_t(CameraFxSettings::FlagsValue::DisableOverexposure);
		}
	}

	void EnableReachStyleFrags()
	{
		using Blam::Tags::Objects::Projectile;

		auto fragProjectile = TagInstance::Find('proj', "objects\\weapons\\grenade\\frag_grenade\\frag_grenade");
		auto trailEffect = TagInstance::Find('effe', "objects\\equipment\\bombrun\\projectiles\\bombrun_grenade\\fx\\projectile");

		if (fragProjectile.Index != 0xFFFF && trailEffect.Index != 0xFFFF)
		{
			auto fragDefinition = fragProjectile.GetDefinition<Projectile>();
			auto trailTagReference = Blam::Tags::TagReference('effe', trailEffect.Index);

			if (fragDefinition->Attachments.Count > 0)
				fragDefinition->Attachments[0].Tag = trailTagReference;
		}
	}

	void EnableAggressiveAudioDiscarding()
	{
		using SoundClasses = Blam::Tags::Sounds::Classes;

		auto matgDefinition = GetGlobalsDefinition();
		if (!matgDefinition)
			return;

		if (matgDefinition->SoundGlobals.Count < 1)
		{
			Utils::Logger::Instance().Log(Utils::LogTypes::Debug, Utils::LogLevel::Warning, "no sound globals defined in tag 'globals\\globals.globals'!");
			return;
		}

		auto snclDefinition = matgDefinition->SoundGlobals[0].SoundClasses.GetDefinition<SoundClasses>();

		if (!snclDefinition)
		{
			Utils::Logger::Instance().Log(Utils::LogTypes::Debug, Utils::LogLevel::Warning, "tag 'sound\\sound_classes.sound_classes' not found!");
			return;
		}

		for (int i = 0; i < snclDefinition->ClassProperties.Count; i++)
		{
			snclDefinition->ClassProperties[i].CacheMissMode = Blam::Tags::Sounds::CacheMissMode::Discard;
			snclDefinition->ClassProperties[i].Priority = 0;

			switch (i)
			{
			case 4:
				snclDefinition->ClassProperties[i].MaxSoundsPerTag = 32;
				snclDefinition->ClassProperties[i].MaxSoundsPerObject = 4;
				break;

			case 23:
				snclDefinition->ClassProperties[i].MaxSoundsPerTag = 16;
				snclDefinition->ClassProperties[i].MaxSoundsPerObject = 8;
				break;

			default:
				snclDefinition->ClassProperties[i].MaxSoundsPerTag = 1;
				snclDefinition->ClassProperties[i].MaxSoundsPerObject = 1;
				break;
			}
		}
	}

	void EnableHitmarkersInternal(bool enabled)
	{
		using Blam::Tags::Objects::Biped;
		using Blam::Tags::UI::ChudDefinition;

		auto matgDefinition = GetGlobalsDefinition();
		if (!matgDefinition)
			return;

		if (matgDefinition->PlayerRepresentation.Count < 1)
		{
			Utils::Logger::Instance().Log(Utils::LogTypes::Debug, Utils::LogLevel::Warning, "no player repreentations defined in tag 'global\\globals'!");
			return;
		}

		auto bipdDefinition = matgDefinition->PlayerRepresentation[0].ThirdPersonUnit.GetDefinition<Biped>();
		if (bipdDefinition->Unit.HudInterfaces.Count < 1)
		{
			Utils::Logger::Instance().Log(Utils::LogTypes::Debug, Utils::LogLevel::Warning, "no hud interfaces defined in tag 'objects\\characters\\masterchief\\mp_masterchief\\mp_masterchief.biped'!");
			return;
		}

		auto chudDefinition = bipdDefinition->Unit.HudInterfaces[0].UnitHudInterface.GetDefinition<ChudDefinition>();

		if (!chudDefinition)
		{
			Utils::Logger::Instance().Log(Utils::LogTypes::Debug, Utils::LogLevel::Warning, "tag 'ui\\chud\\spartan.chud_definition' not found!");
			return;
		}

		auto forceDisabled = Modules::ModuleTweaks::Instance().VarDisableHitMarkers->ValueInt != 0;

		for (auto &hudWidget : chudDefinition->HudWidgets)
		{
			if (hudWidget.NameStringID == 0x2AAD) // hit_marker
			{
				if (enabled && !forceDisabled)
				{
					hudWidget.PlacementData[0].ScaleX = 1.75;
					hudWidget.PlacementData[0].ScaleY = 1.75;
				}
				else
				{
					hudWidget.PlacementData[0].ScaleX = 0;
					hudWidget.PlacementData[0].ScaleY = 0;
				}
				break;
			}
		}
	}
}
