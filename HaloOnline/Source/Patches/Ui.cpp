#include <iostream>
#include <string>
#include <iomanip>
#include <cassert>
#include <vector>
#include <unordered_map>

#include "Patches\Ui.hpp"

#include "HaloOnline.hpp"
#include "Patch.hpp"
#include "Patches\Core.hpp"
#include "Patches\Input.hpp"
#include "Patches\Events.hpp"
#include "Blam\BlamEvents.hpp"
#include "Blam\BlamInput.hpp"
#include "Blam\BlamNetwork.hpp"
#include "Blam\BlamObjects.hpp"
#include "Blam\BlamTime.hpp"
#include "Blam\Tags\TagInstance.hpp"
#include "Blam\Tags\UI\ChudGlobalsDefinition.hpp"
#include "Blam\Tags\UI\ChudDefinition.hpp"
#include "Blam\Tags\Globals\CacheFileGlobalTags.hpp"
#include "Blam\Tags\Game\Globals.hpp"
#include "Blam\\Tags\Objects\Biped.hpp"
#include "Blam\Tags\Items\DefinitionWeapon.hpp"
#include "Blam\Tags\Globals\CacheFileGlobalTags.hpp"
#include "Blam\Tags\Game\Globals.hpp"
#include "Blam\Tags\Game\MultiplayerGlobals.hpp"
#include "Blam\Tags\UI\GfxTexturesList.hpp"
#include "Blam\Tags\UI\MultilingualUnicodeStringList.hpp"
#include "Modules\ModuleGraphics.hpp"
#include "Modules\ModuleInput.hpp"
#include "Modules\ModuleGame.hpp"
#include "Modules\ModuleVoIP.hpp"
#include "Web\Ui\ScreenLayer.hpp"
#include "Modules\ModuleTweaks.hpp"
#include "Modules\ModuleCamera.hpp"
#include "Patches\Tweaks.hpp"
#include "Web\Ui\ScreenLayer.hpp"

#include "new\game\game.hpp"
#include <algorithm>

using namespace Patches::Ui;

namespace
{
	void __fastcall UI_MenuUpdateHook(void* a1, int unused, int menuIdToLoad);

	void OnEvent(Blam::DatumHandle player, const Blam::Events::Event *event, const Blam::Events::EventDefinition *definition);

	int UI_ShowHalo3PauseMenu(uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5);
	void UI_EndGame();
	char __fastcall UI_Forge_ButtonPressHandlerHook(void* a1, int unused, uint8_t* controllerStruct);
	void LocalizedStringHook();
	void LobbyMenuButtonHandlerHook();
	void WindowTitleSprintfHook(char* destBuf, char* format, char* version);
	void ResolutionChangeHook();
	int __fastcall UI_SetPlayerDesiredTeamHook(void *thisPtr, int unused, int a2, int teamIndex, int a4);
	void OnUiInputUpdated();
	void __fastcall UI_UpdateRosterColorsHook(void *thisPtr, int unused, void *a0);
	HWND CreateGameWindowHook();
	void __cdecl UI_UpdateH3HUDHook(int playerMappingIndex);
	void GetActionButtonNameHook();
	void UI_GetHUDGlobalsIndexHook();
	void __fastcall UI_GameVariantSavePromptFix(void *thisptr, void *unused, int a2);

	void __fastcall c_main_menu_screen_widget_item_select_hook(void* thisptr, void* unused, int a2, Blam::Text::StringID a3, void* a4, void* a5);
	void __fastcall c_start_menu_pane_screen_widget__handle_spinner_chosen_hook(void *thisptr, void *unused, uint8_t *widget);
	void __fastcall c_ui_view_draw_hook(void* thisptr, void* unused);
	void __fastcall c_gui_bitmap_widget_update_render_data_hook(void* thisptr, void* unused, void* renderData, int a3, int a4, int a5, int a6, int a7);
	void __fastcall c_gui_alert_manager__show(void *thisptr, void *unused, uint32_t name, int a3, wchar_t *customMessage, int profileIndex, bool a6, int a7);

	void __fastcall c_hud_camera_view__render_outlines_hook(void *thisptr, void *unused, int localPlayerIndex, int playerMappingIndex, void *a3);

	void chud_talking_player_name_hook();
	void __fastcall chud_add_player_marker_hook(void *thisptr, void *unused, uint8_t *data);
	void chud_update_player_marker_state_hook();
	void chud_update_player_marker_sprite_hook();
	void chud_update_marker_sprite_hook();
	void chud_update_player_marker_icon_height_hook();
	void chud_update_player_marker_name_height_hook();

	template <int MaxItems>
	struct c_gui_generic_category_datasource
	{
		uint8_t order_date_source[0x10c];
		wchar_t RecentName[48];
		wchar_t RecentDescription[256];
		wchar_t FileshareName[48];
		wchar_t FileshareDescription[256];
		struct
		{
			int Type;
			int MapId;
			wchar_t Name[48];
			wchar_t Description[256];
			int field_268;
		} items[MaxItems];
		int ItemCount;
	};
	using c_gui_map_category_datasource = c_gui_generic_category_datasource<50>;
	using c_gui_game_variant_category_datasource = c_gui_generic_category_datasource<11>;

	bool __fastcall c_gui_map_category_datasource_init(c_gui_map_category_datasource *thisptr, void* unused, int datasource);
	bool __fastcall c_gui_game_variant_category_datasource_init(c_gui_game_variant_category_datasource *thisptr, void* unused, int datasource);

	void CameraModeChangedHook();
	void StateDataFlags2Hook();
	void StateDataFlags3Hook();
	void StateDataFlags5Hook();
	void StateDataFlags21Hook();
	void StateDataFlags31Hook();
	int GetBrokenChudStateFlags2Values();
	int GetBrokenChudStateFlags3Values();
	int GetBrokenChudStateFlags5Values();
	int GetBrokenChudStateFlags21Values();
	int GetBrokenChudStateFlags31Values();
	int GetBrokenChudStateFlags33Values();
	void c_gui_map_subitem_selectable_item_datasource__vftable01__player_select_actions();
	void GetGlobalDynamicColorHook();
	void GetWeaponOutlineColorHook();

	bool HUDStateDisplayHook(int hudIndex, wchar_t* buff, int len, int a4);

	void FindUiTagIndices();
	void FindHUDDistortionTagData();
	void FindHUDResolutionTagData();
	void FindMapImages();

	void ToggleHUDDistortion(bool enabled);

	void c_gui_screen_pregame_lobby_switch_network_hook();

	std::vector<CreateWindowCallback> createWindowCallbacks;

	Patch unused; // for some reason a patch field is needed here (on release builds) otherwise the game crashes while loading map/game variants, wtf?

	uint32_t hudMessagesUnicIndex;
	uint32_t spartanChdtIndex;
	uint32_t eliteChdtIndex; //Uncomment as needed.
	uint32_t odstChdtIndex;
	uint32_t monitorChdtIndex;
	uint32_t scoreboardChdtIndex;
	uint32_t chgdIndex;
	uint32_t pttLsndIndex;

	bool tagsInitiallyLoaded = false;

	int localDesiredTeam = -1;
	int teamChangeTicks = 0;

	bool foundMapImages = false;
	int multiplayerPlaceholderIndex;
	std::unordered_map<int, int> mapImages = std::unordered_map<int, int>();

	int HUDResolutionWidth = 0;
	int HUDResolutionHeight = 0;
	float HUDResolutionScaleX = 0;
	float HUDResolutionScaleY = 0;
	float HUDMotionSensorOffsetX = 0;
	float HUDBottomVisorOffsetY = 0;

	bool firstHUDDistortionUpdate = true;
	bool validHUDDistortionTags = false;
	bool lastDistortionEnabledValue = true;
	//Distortion direction for spartan, monitor, elite.
	//If more are added, this needs to be increased or stored differently.
	float hudDistortionDirection[3]{ 0,0,0 };
}

namespace Patches::Ui
{
	PlayerMarkersOption playerMarkers;

	bool enableCustomHUDColors = false;
	int customPrimaryHUDColor = -1;
	int customSecondaryHUDColor = 0;

	int chudIndex = -1;

	void ApplyAfterTagsLoaded()
	{
		if (!tagsInitiallyLoaded)
		{
			FindUiTagData();
		}

		tagsInitiallyLoaded = true;

		//reset HUD distortion value when tags are reloaded
		lastDistortionEnabledValue = true;
		UpdateHUDDistortion();
	}

	void ApplyAll()
	{
		// Allows you to press B to close the H3 pause menu
		// TODO: find out what the byte that's being checked does, we're just patching out the check here but maybe it's important
		Patch::NopFill(Pointer::Base(0x6E05F3), 2);

		// Fix "Network" setting in lobbies (change broken 0x100B7 menuID to 0x100B6)
		Patch(0x6C34B0, { 0xB6 }).Apply();
		Patch(0x705B21, { 0xB6 }).Apply();

		// Fix gamepad option in settings (todo: find out why it doesn't detect gamepads
		// and find a way to at least enable pressing ESC when gamepad is enabled)
		Patch::NopFill(Pointer::Base(0x20D7F2), 2);

		// Fix menu update code to include missing mainmenu code
		Hook(0x6DFB73, &UI_MenuUpdateHook, HookFlags::IsCall).Apply();

		// Sorta hacky way of getting game options screen to show when you press X on lobby
		// TODO: find real way of showing the [X] Edit Game Options text, that might enable it to work without patching
		Hook(0x721B8A, LobbyMenuButtonHandlerHook, HookFlags::IsJmpIfEqual).Apply();

		// Hook UI vftable's forge menu button handler, so arrow keys can act as bumpers
		// added side effect: analog stick left/right can also navigate through menus
		DWORD temp;
		DWORD temp2;
		auto writeAddr = Pointer(0x169EFD8);
		if (!VirtualProtect(writeAddr, 4, PAGE_READWRITE, &temp))
		{
			std::stringstream ss;
			ss << "Failed to set protection on memory address " << std::hex << (void*)writeAddr;
			OutputDebugString(ss.str().c_str());
		}
		else
		{
			writeAddr.Write<uint32_t>((uint32_t)&UI_Forge_ButtonPressHandlerHook);
			VirtualProtect(writeAddr, 4, temp, &temp2);
		}

		// Remove Xbox Live from the network menu
		//Patch::NopFill(Pointer::Base(0x723D85), 0x17);
		//Pointer::Base(0x723DA1).Write<uint8_t>(0);
		//Pointer::Base(0x723DB8).Write<uint8_t>(1);
		//Patch::NopFill(Pointer::Base(0x723DFF), 0x3);
		//Pointer::Base(0x723E07).Write<uint8_t>(0);
		//Pointer::Base(0x723E1C).Write<uint8_t>(0);

		// Localized string override hook
		Hook(0x11E040, LocalizedStringHook).Apply();

		// Hook window title sprintf to replace the dest buf with our string
		Hook(0x2EB84, WindowTitleSprintfHook, HookFlags::IsCall).Apply();

		// Runs when the game's resolution is changed
		Hook(0x621303, ResolutionChangeHook, HookFlags::IsCall).Apply();

		if (Modules::ModuleGraphics::Instance().VarUIScaling->ValueInt == 1) {
			// Enable H3UI scaling
			Patch::NopFill(Pointer::Base(0x61FAD1), 2);
		}

		// Change the way that Forge handles dpad up so that it doesn't mess with key repeat
		// Comparing the action tick count to 1 instead of using the "handled" flag does roughly the same thing and lets the menu UI read the key too
		Patch(0x19F17F, { 0x75 }).Apply();
		Patch::NopFill(Pointer::Base(0x19F198), 4);

		// Reimplement the function that assigns lobby roster colors
		Hook(0x726100, UI_UpdateRosterColorsHook).Apply();

		// Game window creation callbacks
		Hook(0x622057, CreateGameWindowHook, HookFlags::IsCall).Apply();

		// disable saber's weapon hud
		//Patch(0x6866F2, { 0xEB }).Apply();
		// restore h3 weapon hud
		//Hook(0x006970BB, UI_UpdateH3HUDHook, HookFlags::IsCall).Apply();
		// fix equipment swap message
		Pointer(0xABD324).Write<uint32_t>(0xABD2F7);
		// fix dual-wield message
		Pointer(0xABD388).Write<uint32_t>(0xABD300);
		// add support for mouse buttons in HUD messages
		Hook(0x6BC83F, GetActionButtonNameHook, HookFlags::IsJmpIfEqual).Apply();
		// enable controller buttons in HUD messages
		Patch(0x6BC5D7, { 0x98,0xDE, 0x44, 0x02, 01 }).Apply();

		Pointer(0x0169FCE0).Write(uint32_t(&c_main_menu_screen_widget_item_select_hook));

		// fix change team/gametype spinner display value
		Pointer(0x016A0158).Write(uint32_t(&c_start_menu_pane_screen_widget__handle_spinner_chosen_hook)); // game_editor
		Pointer(0x0169FFE8).Write(uint32_t(&c_start_menu_pane_screen_widget__handle_spinner_chosen_hook)); // multiplayer

		Hook(0x2047BF, c_ui_view_draw_hook, HookFlags::IsCall).Apply();

		Hook(0x721D38, UI_SetPlayerDesiredTeamHook, HookFlags::IsCall).Apply();
		Patches::Input::RegisterDefaultInputHandler(OnUiInputUpdated);

		//Fix HUD Distortion in third person.
		Hook(0x193370, CameraModeChangedHook, HookFlags::IsCall).Apply();

		// use the correct hud globals for the player representation
		Hook(0x6895E7, UI_GetHUDGlobalsIndexHook, HookFlags::IsJmpIfEqual).Apply();
		Hook(0x6895FF, UI_GetHUDGlobalsIndexHook, HookFlags::IsJmpIfEqual).Apply();

		/* TODO: Fix these: Various color fixes.
		Hook(0x6D5B5F, GetGlobalDynamicColorHook).Apply();
		Hook(0x6CA009, GetWeaponOutlineColorHook).Apply();*/

            //Show the talking player's name on the HUD
		Hook(0x6CA978, chud_talking_player_name_hook, HookFlags::IsCall).Apply();

		Hook(0x686FA4, StateDataFlags2Hook, HookFlags::IsJmpIfEqual).Apply();
		Hook(0x686E7B, StateDataFlags3Hook).Apply();
		Hook(0x687094, StateDataFlags5Hook).Apply();
		Hook(0x687BF0, StateDataFlags21Hook).Apply();
		Hook(0x685A5A, StateDataFlags31Hook).Apply();

		//Show speaking player markers
		Hook(0x349450, chud_update_player_marker_state_hook).Apply();

		//Restore player marker waypoints1 bitmap.
		Hook(0x349469, chud_update_player_marker_sprite_hook).Apply();
		Hook(0x6CED6A, chud_update_marker_sprite_hook).Apply();

		//Jump over player marker waypoints2 bitmap code.
		Patch(0x6C6A11, { 0xEB }).Apply();

		// TODO: FIX THIS: Stop the assault bomb from overwriting the player marker bitmap sprite index.
		//Patch(0x2E805F, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }).Apply();

		// TODO: FIX THESE: Moves the icon above the name on player markers.
		//Patch(0x6CF0B3, { 0x90, 0x90, 0x90 }).Apply(); //Keep name at bottom
		//Hook(0x6CF0A0, chud_update_player_marker_icon_height_hook).Apply(); //Move icon above name if necessary

		// TODO: FIX THIS: allow hiding nametags
		//Hook(0x68AA21, chud_add_player_marker_hook, HookFlags::IsCall).Apply();

		// TODO: FIX THIS: Fixes monitor crosshair position.
		//Patch(0x25F9D5, { 0x4c }).Apply();

		// Fix map images in the selection menu.
		Hook(0x6DA0FE, c_gui_map_subitem_selectable_item_datasource__vftable01__player_select_actions).Apply();

		//Fix map images in lobby.
		Pointer(0x016A6240).Write(uint32_t(&c_gui_bitmap_widget_update_render_data_hook));

		// remove recent maps, fileshare menu items
		//Pointer(0x0169E270).Write(uint32_t(&c_gui_map_category_datasource_init));
		// remove game variants, fileshare menu items
		//Pointer(0x0169E510).Write(uint32_t(&c_gui_game_variant_category_datasource_init));

		//Hook(0x721F03, c_gui_screen_pregame_lobby_switch_network_hook).Apply();

		// prevent transition_out immediately after saving
		Hook(0x6A8706, UI_GameVariantSavePromptFix, HookFlags::IsCall).Apply();

		Hook(0x691FD0, c_gui_alert_manager__show).Apply();

		Hook(0x62D58D, c_hud_camera_view__render_outlines_hook, HookFlags::IsCall).Apply();

		// fix 'none' game variant weapon option
		Patch(0x1CE52D, { 0xEB }).Apply();

		// Fixes some broken hud state strings, including the respawn timer. 
		Hook(0x6963C6, HUDStateDisplayHook, HookFlags::IsCall).Apply();

		Patch::NopFill(Pointer::Base(0x207D1F), 7);
		
		Patches::Events::OnEvent(OnEvent);
	}

	const auto UI_Alloc = reinterpret_cast<void *(__cdecl *)(int32_t)>(0xAB4ED0);
	const auto UI_OpenDialogById = reinterpret_cast<void *(__thiscall *)(void *, Blam::Text::StringID, int32_t, int32_t, Blam::Text::StringID)>(0xA92780);
	const auto UI_PostMessage = reinterpret_cast<int(*)(void *)>(0xA93450);

	void *ShowDialog(const Blam::Text::StringID p_DialogID, const int32_t p_Arg1, const int32_t p_Flags, const Blam::Text::StringID p_ParentID)
	{
		auto *s_UIData = UI_Alloc(0x40);

		if (!s_UIData)
			return nullptr;

		UI_OpenDialogById(s_UIData, p_DialogID, p_Arg1, p_Flags, p_ParentID);
		UI_PostMessage(s_UIData);

		return s_UIData;
	}

	void OnCreateWindow(CreateWindowCallback callback)
	{
		createWindowCallbacks.push_back(callback);
	}

	//Functions that interact with Tags

	void FindUiTagData()
	{
		FindUiTagIndices(); //Call me first.
		FindHUDDistortionTagData();
		FindHUDResolutionTagData();
		FindMapImages();
	}

	bool isPttSoundPlaying;
	void TogglePTTSound(bool enabled)
	{
		if (Blam::game_is_map_loading() || Blam::game_is_mainmenu())
			return;

		if (Modules::ModuleVoIP::Instance().VarPTTSoundEnabled->ValueInt == 0)
			return;

		if (isPttSoundPlaying == enabled)
			return;

		static auto Sound_LoopingSound_Stop = (void(*)(uint32_t sndTagIndex, int a2))(0x5DC6B0);
		static auto Sound_LoopingSound_Start = (void(*)(uint32_t sndTagIndex, int a2, int a3, int a4, char a5))(0x5DC530);

		//Make sure the sound exists before playing
		if (Blam::Tags::TagInstance::IsLoaded('lsnd', pttLsndIndex))
		{
			if (enabled)
				Sound_LoopingSound_Start(pttLsndIndex, -1, 1065353216, 0, 0);
			else
				Sound_LoopingSound_Stop(pttLsndIndex, -1);

			isPttSoundPlaying = enabled;
		}
	}

	std::vector<std::string> speakingPlayers;
	char16_t chud_talking_player_name[32] = {};
	void ToggleSpeakingPlayerName(std::string name, bool speaking)
	{
		//Always remove, in case of duplicates.
		for (size_t i = 0; i < speakingPlayers.size(); i++)
		{
			if (speakingPlayers.at(i) == name)
			{
				speakingPlayers.erase(speakingPlayers.begin() + i);
			}
		}

		if (speaking)
			speakingPlayers.push_back(name);

		//Empty the HUD chud_talking_player_name string.
		//If someone is talking it will be reassigned below.
		memset(chud_talking_player_name, 0, sizeof(chud_talking_player_name));

		if (Modules::ModuleVoIP::Instance().VarSpeakingPlayerOnHUD->ValueInt == 0)
			return;

		//Setup HUD string.
		if (speakingPlayers.size() < 1)
			return;
		else if (speakingPlayers[speakingPlayers.size() - 1].length() < 15) // player names are limited to 15 anyway.
		{
			std::string hud_name = speakingPlayers[speakingPlayers.size() - 1];
			for (size_t i = 0; i < hud_name.length(); i++)
				chud_talking_player_name[i] = hud_name[i];
		}
		else
			return;
	}

	void ApplyUIResolution()
	{
		if (Modules::ModuleGraphics::Instance().VarUIScaling->ValueInt == 1) {
			if (!tagsInitiallyLoaded)
				return;

			using Blam::Tags::TagInstance;
			using Blam::Tags::UI::ChudGlobalsDefinition;
			using Blam::Tags::UI::ChudDefinition;

			if (!TagInstance::IsLoaded('chgd', chgdIndex))
				return;
			else if (!TagInstance::IsLoaded('chdt', spartanChdtIndex))
				return;

			auto *gameResolution = reinterpret_cast<int *>(0x19106C0);
			auto *globals = TagInstance(chgdIndex).GetDefinition<ChudGlobalsDefinition>();
			auto *spartanChud = Blam::Tags::TagInstance(spartanChdtIndex).GetDefinition<Blam::Tags::UI::ChudDefinition>();
			if (!globals || !spartanChud || globals->HudGlobals.Count == 0 || globals->HudGlobals[0].HudAttributes.Count == 0)
				return;

			// Make UI match it's original width of 1920 pixels on non-widescreen monitors.
			// Fixes the visor getting cut off.
			globals->HudGlobals[0].HudAttributes[0].ResolutionWidth = HUDResolutionWidth;

			// H3UI Resolution
			int* UIResolution = reinterpret_cast<int*>(0x19106C8);

			if (((float)gameResolution[0] / (float)gameResolution[1] > 16.0f / 9.0f)) {
				// On aspect ratios with a greater width than 16:9 center the UI on the screen
				globals->HudGlobals[0].HudAttributes[0].ResolutionHeight = HUDResolutionHeight;
				globals->HudGlobals[0].HudAttributes[0].HorizontalScale = (globals->HudGlobals[0].HudAttributes[0].ResolutionWidth / (float)gameResolution[0]) * HUDResolutionScaleX;
				globals->HudGlobals[0].HudAttributes[0].VerticalScale = (globals->HudGlobals[0].HudAttributes[0].ResolutionHeight / (float)gameResolution[1]) * HUDResolutionScaleY;

				UIResolution[0] = (int)(((float)gameResolution[0] / (float)gameResolution[1]) * 640);;
				UIResolution[1] = 640;
			}
			else
			{
				globals->HudGlobals[0].HudAttributes[0].ResolutionHeight = (int)(((float)gameResolution[1] / (float)gameResolution[0]) * globals->HudGlobals[0].HudAttributes[0].ResolutionWidth);
				globals->HudGlobals[0].HudAttributes[0].HorizontalScale = HUDResolutionScaleX;
				globals->HudGlobals[0].HudAttributes[0].VerticalScale = HUDResolutionScaleY;

				UIResolution[0] = 1152;//1152 x 640 resolution
				UIResolution[1] = (int)(((float)gameResolution[1] / (float)gameResolution[0]) * 1152);
			}

			// Adjust motion sensor blip to match the UI resolution
			globals->HudGlobals[0].HudAttributes[0].MotionSensorOffsetX = HUDMotionSensorOffsetX;
			globals->HudGlobals[0].HudAttributes[0].MotionSensorOffsetY = (float)(globals->HudGlobals[0].HudAttributes[0].ResolutionHeight - (globals->HudGlobals[0].HudAttributes[0].MotionSensorRadius - globals->HudGlobals[0].HudAttributes[0].MotionSensorScale) - 4);

			// Search for the visor bottom and fix it if found
			for (auto &widget : spartanChud->HudWidgets)
			{
				if (widget.NameStringID == 0x2ABD) // in_helmet_bottom_new
				{
					widget.PlacementData[0].OffsetY = (((float)globals->HudGlobals[0].HudAttributes[0].ResolutionHeight - HUDResolutionHeight) / 2) + HUDBottomVisorOffsetY;
					break;
				}
			}
		}
	}

	void UpdateHUDDistortion()
	{
		if (!validHUDDistortionTags)
			return;

		Pointer directorPtr(HaloOnline::GetMainTls(GameGlobals::Director::TLSOffset)[0]);
		auto cameraFunc = directorPtr(GameGlobals::Director::CameraFunctionIndex).Read<size_t>();

		if (Modules::ModuleCamera::Instance().VarCameraHideHud->ValueInt != 0)
		{
			ToggleHUDDistortion(true);
			return;
		}

		switch (cameraFunc) //Add cases as required.
		{
			//Unknown cameras:
			//01672130 - c_camera
			//01672920 - c_authored_camera
			//0165A64C - c_director?

		case 0x16724D4: //c_following_camera
		case 0x16725DC: //c_dead_camera
		case 0x167265C: //c_orbiting_camera
		case 0x167280C: //c_scripted_camera
			ToggleHUDDistortion(false);
			break;

		case 0x166ACB0: //c_first_person_camera
			if (Modules::ModuleTweaks::Instance().VarFlatHUD->ValueInt == 1)
				ToggleHUDDistortion(false);
			else
				ToggleHUDDistortion(true);
			break;

		case 0x165A6E4: //c_null_camera
		case 0x16726D0: //c_flying_camera
		case 0x16728A8: //c_static_camera
		default:
			ToggleHUDDistortion(true);
		}
	}

	void ShowLanBrowser()
	{
		typedef void*(__cdecl * UI_AllocPtr)(int size);
		auto UI_Alloc = reinterpret_cast<UI_AllocPtr>(0xAB4ED0);
		typedef void*(__fastcall* c_load_game_browser_screen_messagePtr)(void *thisPtr, int unused, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);
		auto c_load_game_browser_screen_message = reinterpret_cast<c_load_game_browser_screen_messagePtr>(0xADE0B0);
		typedef void(*UI_Dialog_QueueLoadPtr)(void *dialog);
		auto UI_Dialog_QueueLoad = reinterpret_cast<UI_Dialog_QueueLoadPtr>(0xA93450);

		auto dialog = UI_Alloc(0x44);
		if (!dialog)
			return;
		dialog = c_load_game_browser_screen_message(dialog, 0, 0x10355, 0, 4, 0x1000C, 0, 0);
		if (dialog)
			UI_Dialog_QueueLoad(dialog);
	}
}

namespace
{
	void FindUiTagIndices()
	{
		using Blam::Tags::Globals::CacheFileGlobalTags;
		using Blam::Tags::Game::Globals;
		using Blam::Tags::TagInstance;
		using Blam::Tags::Objects::Biped;
		using Blam::Tags::UI::ChudGlobalsDefinition;
		using Blam::Tags::Game::MultiplayerGlobals;

		auto cfgtInstances = TagInstance::GetInstancesInGroup('cfgt');
		for (auto &cfgtInstance : cfgtInstances)
		{
			auto *cfgtDefinition = cfgtInstance.GetDefinition<CacheFileGlobalTags>();
			for (size_t globalsTagIndex = 0; globalsTagIndex < cfgtDefinition->GlobalsTags.Count; globalsTagIndex++)
			{
				if (cfgtDefinition->GlobalsTags[globalsTagIndex].Tag.GroupTag == 'matg')
				{
					auto matgDefinition = TagInstance(cfgtDefinition->GlobalsTags[globalsTagIndex].Tag.TagIndex).GetDefinition<Globals>();

					for (size_t interfaceTagsIndex = 0; interfaceTagsIndex < matgDefinition->InterfaceTags.Count; interfaceTagsIndex++)
					{
						if (matgDefinition->InterfaceTags[interfaceTagsIndex].HudGlobals.TagIndex != NULL &&
							matgDefinition->InterfaceTags[interfaceTagsIndex].HudGlobals.GroupTag == 'chgd')
						{
							chgdIndex = matgDefinition->InterfaceTags[interfaceTagsIndex].HudGlobals.TagIndex;

							if (!TagInstance::IsLoaded('chgd', chgdIndex))
								continue;

							auto *chgd = TagInstance(chgdIndex).GetDefinition<ChudGlobalsDefinition>();

							if (chgd->HudGlobals.Count > 1) {
								if (chgd->HudGlobals[0].ScoreboardHud.TagIndex != NULL)
									scoreboardChdtIndex = chgd->HudGlobals[0].ScoreboardHud.TagIndex;
								if (chgd->HudGlobals[0].HudStrings.TagIndex != NULL)
									hudMessagesUnicIndex = chgd->HudGlobals[0].HudStrings.TagIndex;
							}

							//Can't break out of all these loops without setting an unnecessary variable.
							//So if a chgd is found, move to the next loop.
							goto findBipedChdts;
						}
					}

				findBipedChdts:

					for (size_t playerRepresentationIndex = 0; playerRepresentationIndex < matgDefinition->PlayerRepresentation.Count; playerRepresentationIndex++)
					{
						if (matgDefinition->PlayerRepresentation[playerRepresentationIndex].ThirdPersonUnit.TagIndex == NULL)
							continue;

						uint32_t bipdIndex = matgDefinition->PlayerRepresentation[playerRepresentationIndex].ThirdPersonUnit.TagIndex;

						if (!TagInstance::IsLoaded('bipd', bipdIndex))
							continue;

						auto *bipd = TagInstance(bipdIndex).GetDefinition<Biped>();

						if (bipd->Unit.HudInterfaces.Count < 1)
							continue;

						switch (matgDefinition->PlayerRepresentation[playerRepresentationIndex].Name)
						{
						case 4376: //mp_spartan
							spartanChdtIndex = bipd->Unit.HudInterfaces[0].UnitHudInterface.TagIndex;
							break;
						case 4377: //mp_elite
							eliteChdtIndex = bipd->Unit.HudInterfaces[0].UnitHudInterface.TagIndex;
							break;
						case 4378: //odst01
							odstChdtIndex = bipd->Unit.HudInterfaces[0].UnitHudInterface.TagIndex;
							break;
						case 4379: //monitor
							monitorChdtIndex = bipd->Unit.HudInterfaces[0].UnitHudInterface.TagIndex;
							break;
						default:
							continue;
						}

						//If all the tag indices are found, move on.
						if (spartanChdtIndex != NULL && eliteChdtIndex != NULL && monitorChdtIndex != NULL && odstChdtIndex != NULL)
							goto findPttLsnd;
					}

				findPttLsnd:

					if (matgDefinition->MultiplayerGlobals.TagIndex == NULL)
						return;
					if (!TagInstance::IsLoaded('mulg', matgDefinition->MultiplayerGlobals.TagIndex))
						return;

					auto *mulg = TagInstance(matgDefinition->MultiplayerGlobals.TagIndex).GetDefinition<MultiplayerGlobals>();

					if (mulg->Runtime.Count < 1 || mulg->Runtime[0].LoopingSounds.Count < 1 || mulg->Runtime[0].LoopingSounds[0].LoopingSound.TagIndex == NULL)
						return;

					pttLsndIndex = mulg->Runtime[0].LoopingSounds[0].LoopingSound.TagIndex;

					return;
				}
			}
		}
	}

	void FindMapImages()
	{
		using Blam::Tags::TagInstance;
		using Blam::Tags::UI::GfxTexturesList;

		auto gfxtInstances = TagInstance::GetInstancesInGroup('gfxt');
		for (auto &gfxtInstance : gfxtInstances)
		{
			//mapID|BitmapIndex
			std::unordered_map<int, int> tempMapImages = std::unordered_map<int, int>();

			auto *gfxtDefinition = gfxtInstance.GetDefinition<GfxTexturesList>();
			for each (GfxTexturesList::Texture texture in gfxtDefinition->Textures)
			{
				char *output = strstr(texture.FileName, "placeholder");

				if (output)
				{
					multiplayerPlaceholderIndex = texture.Bitmap.TagIndex;
					continue;
				}

				try
				{
					int mapID = std::stoi(texture.FileName);
					tempMapImages.insert(std::pair<int, int>(mapID, texture.Bitmap.TagIndex));
					continue;
				}
				catch (...)
				{
					goto invalid_gfxt;
				}
			}

			mapImages = tempMapImages;
			foundMapImages = true;
			return;

		invalid_gfxt:
			continue;
		}
	}

	void FindHUDDistortionTagData()
	{
		using Blam::Tags::UI::ChudGlobalsDefinition;
		using Blam::Tags::TagInstance;

		if (!TagInstance::IsLoaded('chgd', chgdIndex))
			return;

		auto *chgd = Blam::Tags::TagInstance(chgdIndex).GetDefinition<ChudGlobalsDefinition>();

		for each (ChudGlobalsDefinition::HudGlobal hudGlobal in chgd->HudGlobals)
		{
			if (hudGlobal.HudAttributes.Count < 1)
				continue;

			hudDistortionDirection[hudGlobal.Biped] = hudGlobal.HudAttributes[0].WarpDirection;
		}
		firstHUDDistortionUpdate = false;
		validHUDDistortionTags = true;
	}

	void FindHUDResolutionTagData()
	{
		using Blam::Tags::TagInstance;
		using Blam::Tags::UI::ChudGlobalsDefinition;
		using Blam::Tags::UI::ChudDefinition;

		if (!TagInstance::IsLoaded('chgd', chgdIndex)) // Chud Globals
			return;
		else if (!TagInstance::IsLoaded('chdt', spartanChdtIndex)) //Chud
			return;
		else if (!TagInstance::IsLoaded('chdt', odstChdtIndex))
			return;
		//else if (!TagInstance::IsLoaded(('chdt', eliteChdtIndex))
			//return;

		auto *globals = TagInstance(chgdIndex).GetDefinition<ChudGlobalsDefinition>();
		auto *spartanChud = Blam::Tags::TagInstance(spartanChdtIndex).GetDefinition<Blam::Tags::UI::ChudDefinition>();
		auto *eliteChud = Blam::Tags::TagInstance(eliteChdtIndex).GetDefinition<Blam::Tags::UI::ChudDefinition>();
		auto* odstChud = Blam::Tags::TagInstance(odstChdtIndex).GetDefinition < Blam::Tags::UI::ChudDefinition>();

		// Store initial HUD resolution values the first time the resolution is changed.
		HUDResolutionWidth = globals->HudGlobals[0].HudAttributes[0].ResolutionWidth;
		HUDResolutionHeight = globals->HudGlobals[0].HudAttributes[0].ResolutionHeight;
		HUDResolutionScaleX = globals->HudGlobals[0].HudAttributes[0].HorizontalScale;
		HUDResolutionScaleY = globals->HudGlobals[0].HudAttributes[0].VerticalScale;
		HUDMotionSensorOffsetX = globals->HudGlobals[0].HudAttributes[0].MotionSensorOffsetX;
		// Store bottom visor offset
		for (auto &widget : spartanChud->HudWidgets)
		{
			if (widget.NameStringID == 0x2ABD) // in_helmet_bottom_new
			{
				HUDBottomVisorOffsetY = widget.PlacementData[0].OffsetY;
			}
		}
	}

	void ToggleHUDDistortion(bool enabled)
	{
		if (!validHUDDistortionTags)
			return;

		if (enabled == lastDistortionEnabledValue)
			return;

		using Blam::Tags::UI::ChudGlobalsDefinition;
		using Blam::Tags::TagInstance;

		//Return if the tag cant be found, happens during loading.
		if (!TagInstance::IsLoaded('chgd', chgdIndex))
			return;

		auto *chgd = Blam::Tags::TagInstance(chgdIndex).GetDefinition<ChudGlobalsDefinition>();

		for (size_t hudGlobalsIndex = 0; hudGlobalsIndex < chgd->HudGlobals.Count; hudGlobalsIndex++)
		{
			if (chgd->HudGlobals[hudGlobalsIndex].HudAttributes.Count < 1)
				continue;

			chgd->HudGlobals[hudGlobalsIndex].HudAttributes[0].WarpDirection = enabled ? hudDistortionDirection[chgd->HudGlobals[hudGlobalsIndex].Biped] : 0;
		}
		lastDistortionEnabledValue = enabled;
	}

	void __fastcall UI_MenuUpdateHook(void* a1, int unused, int menuIdToLoad)
	{
		auto& dorito = HaloOnline::Instance();
		if (menuIdToLoad == 0x10083)
			dorito.OnMainMenuShown();

		bool shouldUpdate = *(DWORD*)((uint8_t*)a1 + 0x10) >= 0x1E;
		int uiData0x18Value = 1;
		//if (menuIdToLoad == 0x100A8) // TODO1: find what 0x100A8(H3E) stringid is in HO
		//	uiData0x18Value = 5;

		typedef void(__thiscall *UI_MenuUpdatePtr)(void* a1, int menuIdToLoad);
		auto UI_MenuUpdate = reinterpret_cast<UI_MenuUpdatePtr>(0xADF6E0);
		UI_MenuUpdate(a1, menuIdToLoad);

		if (shouldUpdate)
		{
			typedef void*(__cdecl * UI_AllocPtr)(int size);
			auto UI_Alloc = reinterpret_cast<UI_AllocPtr>(0xAB4ED0);
			void* UIData = UI_Alloc(0x3C);

			// fill UIData with proper data
			typedef void*(__thiscall * UI_OpenDialogByIdPtr)(void* a1, unsigned int dialogStringId, int a3, int dialogFlags, unsigned int parentDialogStringId);
			auto UI_OpenDialogById = reinterpret_cast<UI_OpenDialogByIdPtr>(0xA92780);
			UI_OpenDialogById(UIData, menuIdToLoad, 0xFF, 4, 0x1000D);

			// post UI message
			typedef int(*UI_PostMessagePtr)(void* uiDataStruct);
			auto UI_PostMessage = reinterpret_cast<UI_PostMessagePtr>(0xA93450);
			UI_PostMessage(UIData);

			*(uint32_t*)((char*)UIData + 0x18) = uiData0x18Value;
		}
	}

	int UI_ShowHalo3PauseMenu(uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5)
	{
		return Patches::Ui::ShowDialog(0x10084, 0, 4, 0x1000C) != nullptr;
	}

	void UI_EndGame()
	{
		auto session = Blam::Network::GetActiveSession();
		if (!session || !session->IsEstablished())
			return;
		if (session->IsHost())
			Blam::Network::EndGame();
		else
			Blam::Network::LeaveGame();
	}

	char __fastcall UI_Forge_ButtonPressHandlerHook(void* a1, int unused, uint8_t* controllerStruct)
	{
		uint32_t btnCode = *(uint32_t*)(controllerStruct + 0x1C);

		if (btnCode == Blam::Input::eUiButtonCodeLeft || btnCode == Blam::Input::eUiButtonCodeRight ||
			btnCode == Blam::Input::eUiButtonCodeDpadLeft || btnCode == Blam::Input::eUiButtonCodeDpadRight)
		{
			if (btnCode == Blam::Input::eUiButtonCodeLeft || btnCode == Blam::Input::eUiButtonCodeDpadLeft) // analog left / arrow key left
				*(uint32_t*)(controllerStruct + 0x1C) = Blam::Input::eUiButtonCodeLB;

			if (btnCode == Blam::Input::eUiButtonCodeRight || btnCode == Blam::Input::eUiButtonCodeDpadRight) // analog right / arrow key right
				*(uint32_t*)(controllerStruct + 0x1C) = Blam::Input::eUiButtonCodeRB;
		}

		typedef char(__thiscall *UI_Forge_ButtonPressHandler)(void* a1, void* controllerStruct);
		UI_Forge_ButtonPressHandler buttonHandler = (UI_Forge_ButtonPressHandler)0xAE2180;
		return buttonHandler(a1, controllerStruct);
	}

	bool LocalizedStringHookImpl(int tagIndex, int stringId, wchar_t* outputBuffer)
	{
		const size_t MaxStringLength = 0x400;

		switch (stringId)
		{
		case 0x1010A: // start_new_campaign
		{
			// Get the version string, convert it to uppercase UTF-16, and return it
			std::string version = Utils::Version::GetVersionString();
			std::transform(version.begin(), version.end(), version.begin(), toupper);
			std::wstring unicodeVersion(version.begin(), version.end());
			swprintf(outputBuffer, MaxStringLength, L"Vergil %s", unicodeVersion.c_str());
			return true;
		}
		}
		return false;
	}

	void WindowTitleSprintfHook(char* destBuf, char* format, char* version)
	{
		std::string windowTitle = "Vergil | Version: " + Utils::Version::GetVersionString() + " | Build Date: " __DATE__;
		strcpy_s(destBuf, 0x40, windowTitle.c_str());
	}

	__declspec(naked) void LocalizedStringHook()
	{
		__asm
		{
			// Run the hook implementation function and fallback to the original if it returned false
			push ebp
			mov ebp, esp
			push[ebp + 0x10]
			push[ebp + 0xC]
			push[ebp + 0x8]
			call LocalizedStringHookImpl
			add esp, 0xC
			test al, al
			jz fallback

			// Don't run the original function
			mov esp, ebp
			pop ebp
			ret

			fallback :
			// Execute replaced code and jump back to original function
			sub esp, 0x800
				mov edx, 0x51E049
				jmp edx
		}
	}

	__declspec(naked) void LobbyMenuButtonHandlerHook()
	{
		__asm
		{
			// call sub that handles showing game options
			mov ecx, esi
			push[edi + 0x10]
			mov eax, 0xB225B0
			call eax
			// jump back to original function
			mov eax, 0xB21B9F
			jmp eax
		}
	}

	unsigned int selectionMenuMapID = 0;
	__declspec(naked) void c_gui_map_subitem_selectable_item_datasource__vftable01__player_select_actions()
	{
		__asm
		{
			mov selectionMenuMapID, esi

			//perform original instruction
			mov ecx, [ebp - 0x52C]

			//return to ED code
			mov esi, 0xADA104
			jmp esi
		}
	}

	void __fastcall c_gui_bitmap_widget_update_render_data_hook(void* thisptr, void* unused, void* renderData, int a3, int a4, int a5, int a6, int a7)
	{
		static auto c_gui_bitmap_widget_get_render_data = (void(__thiscall*)(void* thisptr, void* renderData, int a3, int a4, char a5, char a6, char a7))(0x00B167B0);

		auto name = Pointer(thisptr)(0x40).Read<uint32_t>();
		c_gui_bitmap_widget_get_render_data(thisptr, renderData, a3, a4, a5, a6, a7);

		if (!foundMapImages)
			return;

		if (name != 67196 && name != 67149) // unknown_film_image, map_image_type
			return;

		static int bitmapIndex = 0;
		static int mapID = 0;

		if (name == 67196)
		{
			mapID = Pointer(0x18603F0).Read<uint32_t>();
		}
		else if (name == 67149)
		{
			mapID = selectionMenuMapID;
		}


		try
		{
			bitmapIndex = mapImages.at(mapID);
		}
		catch (...)
		{
			bitmapIndex = multiplayerPlaceholderIndex;
		}


		if (!Blam::Tags::TagInstance::IsLoaded('bitm', bitmapIndex))
			return;

		*(uint32_t*)((uint8_t*)renderData + 0x2C) = bitmapIndex; // bitmap tag index
	}

	void ResolutionChangeHook()
	{
		typedef void(__thiscall *ApplyResolutionChangeFunc)();
		ApplyResolutionChangeFunc ApplyResolutionChange = reinterpret_cast<ApplyResolutionChangeFunc>(0xA226D0);
		ApplyResolutionChange();

		// Update the ingame UI's resolution
		Patches::Ui::ApplyUIResolution();
		Web::Ui::ScreenLayer::Resize();
	}

	int UI_ListItem_GetIndex(void *item)
	{
		typedef int(__fastcall* UI_ListItem_GetIndexPtr)(void *thisPtr, int unused);
		auto vtable = *reinterpret_cast<void***>(item);
		auto GetIndex = reinterpret_cast<UI_ListItem_GetIndexPtr>(vtable[6]);
		return GetIndex(item, 0);
	}

	bool UI_OrderedDataSource_Get(void *data, int index, int key, void *result)
	{
		typedef bool(__fastcall* UI_OrderedDataSource_GetPtr)(void *thisPtr, int unused, int index, int key, void *result);
		auto vtable = *reinterpret_cast<void***>(data);
		auto Get = reinterpret_cast<UI_OrderedDataSource_GetPtr>(vtable[11]);
		return Get(data, 0, index, key, result);
	}

	int __fastcall UI_SetPlayerDesiredTeamHook(void *thisPtr, int unused, int a2, int teamIndex, int a4)
	{
		localDesiredTeam = teamIndex;
		teamChangeTicks = 0;

		typedef int(__thiscall *UI_SetPlayerDesiredTeamFunc)(void* thisPtr, int a2, int a3, int a4);
		UI_SetPlayerDesiredTeamFunc UI_SetPlayerDesiredTeam = reinterpret_cast<UI_SetPlayerDesiredTeamFunc>(0xB25C30);
		return UI_SetPlayerDesiredTeam(thisPtr, a2, teamIndex, a4);
	}

	void OnUiInputUpdated()
	{
		auto isUsingController = *(bool*)0x0244DE98;

		if (isUsingController && Blam::Input::GetActionState(Blam::Input::eGameActionUiB)->Ticks == 1) //reset desired team after cancel
			localDesiredTeam = -1;
		else if (Blam::Input::GetKeyTicks(Blam::Input::KeyCode::eKeyCodeB, Blam::Input::InputType::eInputTypeGame) == 1) //kbm
			localDesiredTeam = -1;
	}

	void __fastcall UI_UpdateRosterColorsHook(void *thisPtr, int unused, void *a0)
	{
		typedef void*(__fastcall* UI_GetOrderedDataSourcePtr)(void *thisPtr, int unused);
		auto UI_GetOrderedDataSource = reinterpret_cast<UI_GetOrderedDataSourcePtr>(0xB14FE0);
		typedef void(__fastcall* UI_BaseUpdateListColorsPtr)(void *thisPtr, int unused, void *a0);
		auto UI_BaseUpdateListColors = reinterpret_cast<UI_BaseUpdateListColorsPtr>(0xB16650);
		typedef void*(__fastcall* UI_Widget_FindFirstChildPtr)(void *thisPtr, int unused, int type);
		auto UI_Widget_FindFirstChild = reinterpret_cast<UI_Widget_FindFirstChildPtr>(0xAB8F80);
		typedef void*(__fastcall* UI_ListItem_NextPtr)(void *thisPtr, int unused, bool a0);
		auto UI_ListItem_Next = reinterpret_cast<UI_ListItem_NextPtr>(0xAB9230);
		typedef void*(__fastcall* UI_Widget_FindChildTextPtr)(void *thisPtr, int unused, int name);
		auto UI_Widget_FindChildText = reinterpret_cast<UI_Widget_FindChildTextPtr>(0xAB8AE0);
		typedef void*(__fastcall* UI_Widget_FindChildBitmapPtr)(void *thisPtr, int unused, int name);
		auto UI_Widget_FindChildBitmap = reinterpret_cast<UI_Widget_FindChildBitmapPtr>(0xAB8A40);
		typedef void(*UI_ColorWidgetWithPlayerColorPtr)(void *widget, int colorIndex, bool isTeam);
		auto UI_ColorWidgetWithPlayerColor = reinterpret_cast<UI_ColorWidgetWithPlayerColorPtr>(0xAA4C80);
		typedef void(__fastcall* UI_Widget_MultiplyColorPtr)(void *thisPtr, int unused, float *argbColor);
		auto UI_Widget_MultiplyColor = reinterpret_cast<UI_Widget_MultiplyColorPtr>(0xAB9D70);
		typedef void(*RgbToFloatColorPtr)(uint32_t rgbColor, float *result);
		auto RgbToFloatColor = reinterpret_cast<RgbToFloatColorPtr>(0x521300);

		const auto str_name = 0x2AA;
		const auto str_base_color = 0x10144;
		const auto str_base_color_hilite = 0x103DD;
		const auto str_team = 0x11D;
		const auto str_player_row_type = 0x103CD;

		// Call the base function in c_gui_list_widget
		UI_BaseUpdateListColors(thisPtr, 0, a0);

		// Get the data source to fetch player information from
		auto data = UI_GetOrderedDataSource(thisPtr, 0);
		if (!data)
			return;

		// Get whether teams are enabled by querying the session parameter
		// TODO: Is there a better way of doing this? The code that H3E uses doesn't seem to work anymore...
		static auto teamsEnabled = false; // This is static so that last value of this can be reused if the session closes
		auto session = Blam::Network::GetActiveSession();
		if (session && session->IsEstablished())
			teamsEnabled = session->HasTeams();

		// Loop through each list item widget
		auto item = UI_Widget_FindFirstChild(thisPtr, 0, 5); // 5 = List item
		while (item)
		{
			// Get child widget pointers
			auto nameWidget = UI_Widget_FindChildText(item, 0, str_name);
			auto baseColorWidget = UI_Widget_FindChildBitmap(item, 0, str_base_color);
			auto baseColorHiliteWidget = UI_Widget_FindChildBitmap(item, 0, str_base_color_hilite);

			// Get the list item index needed for data binding
			auto itemIndex = UI_ListItem_GetIndex(item);

			// Only set the color if player_row_type == 0
			auto playerRowType = 0;
			if (UI_OrderedDataSource_Get(data, itemIndex, str_player_row_type, &playerRowType) && playerRowType == 0)
			{
				// If teams are enabled and the player is assigned to a team, then use the team color,
				// otherwise use the primary color
				auto teamIndex = 0;
				if (teamsEnabled && UI_OrderedDataSource_Get(data, itemIndex, str_team, &teamIndex) && teamIndex >= 0)
				{
					int playerWidgetIndex = *reinterpret_cast<uint32_t*>(reinterpret_cast<int>(data) + itemIndex * 0x1678 + 0x11C);
					if (playerWidgetIndex == session->MembershipInfo.GetPeerPlayer(session->MembershipInfo.LocalPeerIndex))
					{
						if (localDesiredTeam > -1 && localDesiredTeam < 8)
						{
							teamChangeTicks++;
							if (teamChangeTicks > 120)
								localDesiredTeam = -1;
							else
								teamIndex = localDesiredTeam;
						}
					}

					if (nameWidget)
						UI_ColorWidgetWithPlayerColor(nameWidget, teamIndex, true);
					if (baseColorWidget)
						UI_ColorWidgetWithPlayerColor(baseColorWidget, teamIndex, true);
					if (baseColorHiliteWidget)
						UI_ColorWidgetWithPlayerColor(baseColorHiliteWidget, teamIndex, true);
				}
				else
				{
					// Pull the primary color directly out of the data source (it's easier)
					auto primaryColor = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(data) + itemIndex * 0x1678 + 0x7F8);
					float primaryColorF[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
					RgbToFloatColor(primaryColor, &primaryColorF[1]);
					if (baseColorWidget)
						UI_Widget_MultiplyColor(baseColorWidget, 0, primaryColorF);
					if (baseColorHiliteWidget)
						UI_Widget_MultiplyColor(baseColorHiliteWidget, 0, primaryColorF);
				}
			}

			// Move to the next item
			item = UI_ListItem_Next(item, 0, true);
		}
	}

	HWND CreateGameWindowHook()
	{
		typedef HWND(*CreateGameWindowPtr)();
		auto CreateGameWindow = reinterpret_cast<CreateGameWindowPtr>(0xA223F0);
		auto hwnd = CreateGameWindow();
		if (!hwnd)
			return nullptr;

		for (auto &&callback : createWindowCallbacks)
			callback(hwnd);

		return hwnd;
	}

	bool UpdateH3HUDImpl(int playerMappingIndex)
	{
		struct ControlGlobalsAction
		{
			uint16_t Type;
			uint16_t Flags;
			uint32_t ObjectIndex;
			// ...
		};

		static auto GetPlayerControlsAction = (ControlGlobalsAction*(*)(int playerMappingIndex))(0x5D0BD0);
		static auto HUD_DisplayHUDMessage = (void(*)(unsigned int playerMappingIndex, int stringId, int a3, int alignment))(0x00A964E0);

		auto action = GetPlayerControlsAction(playerMappingIndex);

		if (action->Type != 9 && action->Type != 1)
			return false;
		auto actionWeapObject = Blam::Objects::Get(action->ObjectIndex);
		if (!actionWeapObject)
			return false;

		auto actionWeapDef = Blam::Tags::TagInstance(actionWeapObject->TagIndex).GetDefinition<Blam::Tags::Items::Weapon>();

		if (action->Flags & 1)
			HUD_DisplayHUDMessage(playerMappingIndex, actionWeapDef->Item.PickupMessage, -1, 0);
		else if (action->Flags & 2)
			HUD_DisplayHUDMessage(playerMappingIndex, actionWeapDef->Item.SwapMessage, -1, 0);
		else
			HUD_DisplayHUDMessage(playerMappingIndex, -1, -1, 0);

		if (action->Flags & 4)
			HUD_DisplayHUDMessage(playerMappingIndex, actionWeapDef->Item.PickupOrDualWieldMessage, -1, 1);
		else if (action->Flags & 8)
			HUD_DisplayHUDMessage(playerMappingIndex, actionWeapDef->Item.SwapOrDualWieldMessage, -1, 1);
		else
			HUD_DisplayHUDMessage(playerMappingIndex, -1, -1, 1);

		return true;
	}

	void __cdecl UI_UpdateH3HUDHook(int playerMappingIndex)
	{
		static auto UI_UpdateHUD = (void(__cdecl*)(int playerMappingIndex))(0xA95D10);

		if (!UpdateH3HUDImpl(playerMappingIndex))
			UI_UpdateHUD(playerMappingIndex);
	}

	bool __cdecl GetBoundMouseButtonName(int code, wchar_t* buff)
	{
		using namespace Blam::Input;

		static auto GameActionFromUnicodeCode = (GameAction(__cdecl *)(int code))(0xABD240);

		auto bindings = Modules::ModuleInput::Instance().GetBindings();

		auto action = GameActionFromUnicodeCode(code);
		auto mouseButton = bindings->PrimaryMouseButtons[action];
		switch (mouseButton)
		{
		case eMouseButtonWheelDown:
			swprintf(buff, L"<WHEEL DOWN>");
			return true;
		case eMouseButtonWheelUp:
			swprintf(buff, L"<WHEEL UP>");
			return true;
		case eMouseButtonLeft:
			swprintf(buff, L"<LMB>");
			return true;
		case eMouseButtonRight:
			swprintf(buff, L"<RMB>");
			return true;
		case eMouseButtonMiddle:
			swprintf(buff, L"<MMB>");
			return true;
		}

		return false;
	}

	__declspec(naked) void GetActionButtonNameHook()
	{
		__asm
		{
			push[ebp + 0x14]
			push[ebp + 0x10]
			call GetBoundMouseButtonName
			add esp, 8
			test al, al
			jz DEFAULT
			pop ebx
			mov esp, ebp
			pop ebp
			retn
			DEFAULT :
			mov eax, 0xABCA79
				jmp eax
		}
	}

	void __fastcall c_main_menu_screen_widget_item_select_hook(void* thisptr, void* unused, int a2, Blam::Text::StringID screenName, void* a4, void* a5)
	{
		static const auto c_main_menu_screen_widget_item_select = (void(__thiscall*)(void* thisptr, int a2, int a3, void* dataSource, void* a5))0xAE77D0;

		if (screenName == 0x10083) // main_menu
		{
			uint32_t id;
			auto itemIndex = (*(int(__thiscall **)(void*))(*(uint8_t**)a4 + 0x18))(a4);
			if ((*(bool(__thiscall **)(void*, uint32_t, uint32_t, uint32_t*))(*(uint8_t**)a5 + 0x34))(a5, itemIndex, 0x2AA, &id))
			{
				// TODO: Maybe check the menu item's text over the index?

				switch (itemIndex)
				{
				case 0: // Server Browser
					Web::Ui::ScreenLayer::Show("browser", "{}");
					return;

				case 1: // Host Multiplayer
					break;

				case 2: // Host Forge
					break;

				case 3: // Local Games
					ShowLanBrowser();
					return;

				case 4: // Customization
					Web::Ui::ScreenLayer::Show("profile_settings", "{}");
					return;

				case 5: // FileShare
					Web::Ui::ScreenLayer::Show("fileshare", "{}");
					return;

				case 6: // Settings
					Web::Ui::ScreenLayer::Show("settings", "{}");
					return;

				case 7: // Exit
					Web::Ui::ScreenLayer::Show("exit", "{}");
					return;
				}
			}
		}

		c_main_menu_screen_widget_item_select(thisptr, a2, screenName, a4, a5);
	}

	int GetHUDGlobalsIndexForRepresentation(void* playerRepresentation)
	{
		if (!playerRepresentation)
			return 0;

		using Blam::Tags::UI::ChudGlobalsDefinition;
		auto *chgd = Blam::Tags::TagInstance(chgdIndex).GetDefinition<ChudGlobalsDefinition>();

		if (chudIndex > 0 && chudIndex <= chgd->HudGlobals.Count)
			return chudIndex - 1;

		auto nameId = *(uint32_t*)playerRepresentation;
		switch (nameId)
		{
		case 0x111B: // monitor
			return 2;
		case 0x1119: //mp_elite
		case 0xCC: // dervish
			return 1;
		default:
			return 0;
		}
	}

	__declspec(naked) void UI_GetHUDGlobalsIndexHook()
	{
		__asm
		{
			push ecx
			call GetHUDGlobalsIndexForRepresentation
			add esp, 4
			pop esi
			pop ebp
			retn
		}
	}

	void __fastcall c_ui_view_draw_hook(void* thisptr, void* unused)
	{
		if (!Modules::ModuleGame::Instance().VarHideH3UI->ValueInt)
			((void(__thiscall*)(void* thisptr))(0xA290A0))(thisptr);
	}

	__declspec(naked) void CameraModeChangedHook()
	{
		__asm
		{
			//execute custom code
			call UpdateHUDDistortion

			//perform original instruction
			movss xmm0, [ebp + 0xC]

			//return to eldorado code
			ret
		}
	}

	//Flags probably hooks need to be updated to support split screen/theater at some point.
	__declspec(naked) void StateDataFlags2Hook()
	{
		__asm
		{
			call GetBrokenChudStateFlags2Values
			or word ptr[edi + 57Ah], ax

			//Since biped flags are regularly updated,
			//Fix flags additions to flags33 here too, to also be regularly updated.
			call GetBrokenChudStateFlags33Values
			or [edi + 5B0h], eax

			mov eax, 0xA86FC8
			jmp eax
		}
	}

	__declspec(naked) void StateDataFlags3Hook()
	{
		__asm
		{
			call GetBrokenChudStateFlags3Values
			or word ptr[edi + 57Ch], ax
			mov eax, 0xA86EEF
			jmp eax
		}
	}

	__declspec(naked) void StateDataFlags5Hook()
	{
		__asm
		{
			call GetBrokenChudStateFlags5Values
			or word ptr[edi + 580h], ax
			mov eax, 0xA870A6
			jmp eax
		}
	}

	__declspec(naked) void StateDataFlags21Hook()
	{
		__asm
		{
			call GetBrokenChudStateFlags21Values
			or word ptr[edi + 5A0h], ax
			cmp byte ptr[edi + 0x00062C], 00 //perform original instruction
			mov eax, 0xA87BF7
			jmp eax
		}
	}

	//Called if equipment held
	__declspec(naked) void StateDataFlags31Hook()
	{
		__asm
		{
			call GetBrokenChudStateFlags31Values
			or [edi + 5ACh], eax
			mov eax, 0xA85A71
			jmp eax
		}
	}

	//All values in this bitfield are broken.
	int GetBrokenChudStateFlags2Values()
	{
		using Blam::Players::PlayerDatum;

		auto sub_53A6F0 = (void*(*)(uint32_t))(0x53A6F0);

		void* playerRepresentation = sub_53A6F0(Blam::Players::GetLocalPlayer(0).Index);

		auto nameId = *(uint32_t*)playerRepresentation;
		switch (nameId)
		{
		case 0x111B: // monitor
			return 4;
		case 0x1119: //mp_elite
		case 0xCC: // dervish
		default:
			return chudIndex + 1;
		}
	}

	//All voice values in this bitfield are broken.
	int GetBrokenChudStateFlags3Values()
	{
		int flags = 0;

		Modules::ModuleVoIP &voipModule = Modules::ModuleVoIP::Instance();

		if (!voipModule.VarVoipEnabled->ValueInt == 0)
		{
			if (voipModule.voipConnected)
			{
				if (voipModule.VarPTTEnabled->ValueInt == 0)
				{
					if (voipModule.voiceDetected)
						flags |= 0x400; //talking
					else
						flags |= 0x200; //not_talking
				}
				else
				{
					if (Blam::Input::GetActionState(Blam::Input::eGameActionVoiceChat)->Ticks > 0)
						flags |= 0x400; //talking
					else
					{
						flags |= 0x80; //tap_to_talk
						flags |= 0x200; //not_talking
					}
				}
			}
			else
				flags |= 0x40; //talking_disabled
		}

		return flags;
	}

	//All voice values in this bitfield are broken.
	int GetBrokenChudStateFlags5Values()
	{
		int flags = 0;

		if (speakingPlayers.size() > 0 && Modules::ModuleVoIP::Instance().VarSpeakingPlayerOnHUD->ValueInt == 1)
			flags |= 8; //SomeoneIsTalking

		return flags;
	}

	//All voice values in this bitfield are broken.
	int GetBrokenChudStateFlags21Values()
	{
		int flags = 0;

		Modules::ModuleVoIP &voipModule = Modules::ModuleVoIP::Instance();

		if (!voipModule.VarVoipEnabled->ValueInt == 0)
		{
			if (voipModule.voipConnected)
			{
				if (voipModule.VarPTTEnabled->ValueInt == 0)
				{
					if (voipModule.voiceDetected)
						flags |= 0x400; //Bit10
				}
				else
				{
					if (Blam::Input::GetActionState(Blam::Input::eGameActionVoiceChat)->Ticks > 0)
						flags |= 0x400; //Bit10
				}
			}
		}

		return flags;
	}

	int lastEqipIndex;
	int GetBrokenChudStateFlags31Values()
	{
		int flags;

		using ObjectArray = Blam::DataArray<Blam::Objects::ObjectHeader>;
		using Blam::Players::PlayerDatum;

		auto objectsPtr = (ObjectArray**)HaloOnline::GetMainTls(0x448);

		if (!objectsPtr)
			return 0;

		PlayerDatum *playerDatum = Blam::Players::GetPlayers().Get(Blam::Players::GetLocalPlayer(0));

		auto unitObjectDatum = (*objectsPtr)->Get(playerDatum->SlaveUnit);
		auto unitObject = unitObjectDatum->Data;

		if (!unitObjectDatum || !unitObject)
			return 0;

		auto primaryEquipmentObjectIndex = Pointer(unitObject)(0x2F0).Read<uint32_t>();

		if (primaryEquipmentObjectIndex == -1)
			return 0;

		auto itemObjectDatum = (*objectsPtr)->Get(primaryEquipmentObjectIndex);
		if (!itemObjectDatum)
			return 0;

		auto itemTagIndex = Pointer(itemObjectDatum->Data).Read<uint32_t>();

		if (itemTagIndex != lastEqipIndex)
			flags = 0;
		else
			flags = 0x100;

		lastEqipIndex = itemTagIndex;

		return flags;
	}

	int GetBrokenChudStateFlags33Values()
	{
		using Blam::Players::PlayerDatum;
		PlayerDatum *playerDatum = Blam::Players::GetPlayers().Get(Blam::Players::GetLocalPlayer(0));

		int flags = 0;

		//Team and FFA flags that were in Flags 1 in halo 3 are now here.
		auto session = Blam::Network::GetActiveSession();
		if (session && session->IsEstablished())
			if (session->HasTeams())
				flags |= 0x2000; //Bit13, was inactive, now Teams Enabled
			else
				flags |= 0x1000; //Bit12, was inactive, now Teams Disabled

		if (playerDatum->DepletedStamina > 0)
			flags |= 0x4000; //Bit14, was inactive, now StaminaNotFull

		return flags;
	}

	__declspec(naked) void GetGlobalDynamicColorHook()
	{
		_asm
		{
			cmp enableCustomHUDColors, 1
			jne tag_color
			cmp[ebp + 0xC], 0x0
			je secondary_color
			cmp[ebp + 0xC], 0x1
			je secondary_color
			cmp[ebp + 0xC], 0x2
			je primary_color
			cmp[ebp + 0xC], 0x4
			je primary_color
			cmp[ebp + 0xC], 0x8
			je primary_color
			cmp[ebp + 0xC], 0xA
			je primary_color
			cmp[ebp + 0xC], 0xB
			je primary_color
			cmp[ebp + 0xC], 0xF
			je primary_color

			tag_color :
			mov eax, [eax + edi * 4 + 4]
				jmp eldorado_return

				primary_color :
			mov eax, Patches::Ui::customPrimaryHUDColor
				jmp eldorado_return

				secondary_color :
			mov eax, Patches::Ui::customSecondaryHUDColor
				jmp eldorado_return

				eldorado_return :
			pop edi
				pop esi
				pop ebx
				pop ebp
				ret 8
		}
	}

	__declspec(naked) void GetWeaponOutlineColorHook()
	{
		_asm
		{
			cmp enableCustomHUDColors, 1
			jne tag_color
			cmp ecx, 0
			je secondary_color

			tag_color :
			push[eax + ecx * 4 + 0x7C]
				jmp eldorado_return

				secondary_color :
			push customSecondaryHUDColor

				eldorado_return :
			mov ebx, 0xAC9AA0
				call ebx
				add esp, 0xC
				pop ebp
				retn
		}
	}

	bool __fastcall c_gui_map_category_datasource_init(c_gui_map_category_datasource *thisptr, void* unused, int datasource)
	{
		static auto c_gui_data_init = ((bool(__thiscall*)(void*, int))(0x00AD52F0));
		static auto maps_get_multiplayer_map_ids = (void(*)(int* pMapIds, int* pCount))(0x0054BEE0);
		static auto maps_get_multiplayer_level = (bool(*)(int mapId, uint8_t* data))(0x0054CB00);
		static auto sub_54BFD0 = (bool(*)(int mapId))(0x54BFD0);

		if (!c_gui_data_init(thisptr, datasource))
			return false;

		int mapIds[50];
		int numMapIds = 50;
		maps_get_multiplayer_map_ids(mapIds, &numMapIds);

		thisptr->ItemCount = 0;
		for (auto i = 0; i < numMapIds; i++)
		{
			uint8_t levelInfo[0x360];
			if (sub_54BFD0(mapIds[i]) && maps_get_multiplayer_level(mapIds[i], levelInfo))
			{
				auto& item = thisptr->items[thisptr->ItemCount];

				item.Type = 2;
				item.MapId = mapIds[i];
				wcsncpy(item.Name, (wchar_t*)(levelInfo + 0x8), 48);
				wcsncpy(item.Description, (wchar_t*)(levelInfo + 0x48), 256);
				item.field_268 = *(uint32_t*)(levelInfo + 0x34c);
				thisptr->ItemCount++;
			}
		}

		std::qsort(thisptr->items, thisptr->ItemCount, 0x26c, (int(*)(const void*, const void*))0xADA1D0);
		return true;
	}

	bool __fastcall c_gui_game_variant_category_datasource_init(c_gui_game_variant_category_datasource *thisptr, void* unused, int datasource)
	{
		static auto c_gui_data_init = ((bool(__thiscall*)(void*, int))(0x00AD52F0));
		static auto sub_A84200 = (void*(*)(int a1))(0xA84200);
		static auto sub_ADB130 = (void*(__thiscall*)(c_gui_game_variant_category_datasource *thisptr, void* a2, int index))(0xADB130);

		if (!c_gui_data_init(thisptr, datasource))
			return false;

		auto tagsource = sub_A84200(0x201); // game_variant
		if (!tagsource)
			return false;

		thisptr->ItemCount = 0;
		auto index = 0;
		do
		{
			if (index)
			{
				if (index != 5)
					sub_ADB130(thisptr, tagsource, index);
			}
			++index;
		} while (index < 11);

		std::qsort(thisptr->items, thisptr->ItemCount, 0x26c, (int(*)(const void*, const void*))0xADB280);
		return true;
	}

	void __stdcall OnLobbySwitchNetworkMode()
	{
		Web::Ui::ScreenLayer::Show("server_settings", "{}");
	}

	__declspec(naked) void c_gui_screen_pregame_lobby_switch_network_hook()
	{
		__asm
		{
			call OnLobbySwitchNetworkMode

			// jump to cleanup and return
			mov eax, 0xB21F5B
			jmp eax
		}
	}

	void __fastcall c_gui_alert_manager__show(void *thisptr, void *unused, uint32_t name, int a3, wchar_t *customMessage, int profileIndex, bool a6, int a7)
	{
		struct Alert
		{
			uint32_t Name;
			uint8_t Flags;
			uint8_t Unknown05;
			uint8_t Icon;
			uint8_t Unknown07;
			uint32_t Title;
			uint32_t Body;
		};

		const auto ui_get_shared_globals_definition = (uint8_t*(*)())(0x00A84D20);
		const auto ui_resolve_string = (char*(*)(uint32_t unicTagIndex, uint32_t stringId, int lanquageId))(0x0051E290);
		const auto ui_get_prefered_lanquage = (int(*)())(0x0052FC40);

		auto wigl = ui_get_shared_globals_definition();
		if (!wigl)
			return;

		const auto alertStrings = 0x00000989;

		auto &alerts = *(Blam::Tags::TagBlock<Alert>*)(wigl + 0x9C);
		for (auto& alert : alerts)
		{
			if (alert.Name == name)
			{
				auto title = ui_resolve_string(alertStrings, alert.Title, ui_get_prefered_lanquage());
				auto body = ui_resolve_string(alertStrings, alert.Body, ui_get_prefered_lanquage());
				if (customMessage)
				{
					auto customMessageStr = Utils::String::ThinString(customMessage);
					Web::Ui::ScreenLayer::ShowAlert(title, customMessageStr.c_str(), Web::Ui::ScreenLayer::AlertIcon(alert.Icon));
				}
				else
				{
					Web::Ui::ScreenLayer::ShowAlert(title, body, Web::Ui::ScreenLayer::AlertIcon(alert.Icon));
				}

				return;
			}
		}
	}

	void __fastcall c_hud_camera_view__render_outlines_hook(void *thisptr, void *unused, int localProfileIndex, int playerMappingIndex, void *a3)
	{
		const auto c_hud_camera_view__render_outlines_hook = (void(__thiscall*)(void *thisptr, int localProfileIndex, int playerMappingIndex, void *a3))(0xA2E620);

		const auto &moduleTweaks = Modules::ModuleTweaks::Instance();
		const auto playerIndex = Blam::Players::GetLocalPlayer(0); // no need to map until splitscreen is supported
		Blam::Players::PlayerDatum *player;

		if (playerIndex != Blam::DatumHandle::Null && (player = Blam::Players::GetPlayers().Get(playerIndex))
			&& player->SlaveUnit != Blam::DatumHandle::Null && Blam::Objects::Get(player->SlaveUnit))
		{
			// outlines are only rendered if we have a unit regardless
			c_hud_camera_view__render_outlines_hook(thisptr, localProfileIndex, playerMappingIndex, a3);
		}
	}

	__declspec(naked) void chud_talking_player_name_hook()
	{
		_asm
		{
			add eax, 0xC7C
			mov edi, eax
			lea esi, chud_talking_player_name
			mov ecx, 64
			repe movsb
			ret
		}
	}

	bool PlayerMarkerIsDisabled(uint32_t playerIndex)
	{
		auto &players = Blam::Players::GetPlayers();

		if (playerIndex == -1)
			return false;

		auto player = Blam::Players::GetPlayers().Get(playerIndex);
		if (!player)
			return false;

		auto localPlayerDatumIndex = Blam::Players::GetLocalPlayer(0);
		if (localPlayerDatumIndex == Blam::DatumHandle::Null)
			return false;

		auto localPlayer = players.Get(localPlayerDatumIndex);
		if (!localPlayer)
			return false;

		auto waypointTrait = *(uint8_t*)((uint8_t*)player + 0x2DC1);

		switch (waypointTrait)
		{
		case 4:
			return player->Properties.TeamIndex != localPlayer->Properties.TeamIndex; // team only
		case 5:
			return true;
		};

		return false;
	}

	void __fastcall chud_add_player_marker_hook(void *thisptr, void *unused, uint8_t *data_)
	{
		struct s_chud_player_marker_data
		{
			uint32_t player_index;
			uint32_t field_4;
			uint32_t field_8;
			uint32_t color;
			uint32_t flags;
			uint32_t field_14;
			wchar_t text[5];
			uint16_t field_22;
			Blam::Math::RealVector3D position;
		};
		static_assert(sizeof(s_chud_player_marker_data) == 0x30, "s_chud_player_marker_data invalid");

		struct s_chud_player_marker_state
		{
			s_chud_player_marker_data data;
			bool is_valid;
			uint32_t time;
			float scale;
			uint8_t field_3C[0x2C];
			uint32_t state;
			uint32_t field_6C;
			uint32_t field_70;
		};
		static_assert(sizeof(s_chud_player_marker_state) == 0x74, "s_chud_player_marker_state invalid");

		const auto sub_AAF560 = (int(__thiscall *)(void *thisptr))(0xAAF560);

		auto data = (s_chud_player_marker_data*)data_;
		auto index = 0;
		auto state = (s_chud_player_marker_state *)thisptr;
		while (!state->is_valid || state->data.player_index != data->player_index)
		{
			++index;
			++state;
			if (index >= 0x14)
			{
				index = -1;
				break;
			}
		}

		if (PlayerMarkerIsDisabled(data->player_index))
		{
			if (index != -1)
			{
				// existing state found, mark it as invalid
				auto newState = &((s_chud_player_marker_state*)thisptr)[sub_AAF560(thisptr)];
				newState->is_valid = false;
				newState->data.player_index = -1;
			}
		}
		else
		{

			if (index == -1)
			{
				auto newState = &((s_chud_player_marker_state*)thisptr)[sub_AAF560(thisptr)];
				memcpy(&newState->data, data, sizeof(s_chud_player_marker_data));
				newState->is_valid = 1;
				newState->state = -1;
				newState->field_6C = 0;
				newState->time = Blam::Time::GetGameTicks();
				return;
			}

			// player scaling
			if (data->player_index != -1)
			{
				auto player = Blam::Players::GetPlayers().Get(data->player_index);
				if (player && player->SlaveUnit != Blam::DatumHandle::Null)
				{
					auto unitObject = Blam::Objects::Get(player->SlaveUnit);
					if (unitObject)
						data->position.K = data->position.K - 0.05f + std::pow(unitObject->Scale, 1.0f) * 0.05f;
				}
			}

			auto existingState = &((s_chud_player_marker_state*)thisptr)[index];
			memcpy(&existingState->data, data, sizeof(s_chud_player_marker_data));
			existingState->time = Blam::Time::GetGameTicks();
		}
	}

	unsigned int __stdcall IsPlayerSpeaking(int handle)
	{
		Blam::Players::PlayerDatum* player = Blam::Players::GetPlayers().Get(Blam::DatumHandle(handle));
		std::string playerName(Utils::String::ThinString(player->Properties.DisplayName));

		if (std::find(speakingPlayers.begin(), speakingPlayers.end(), playerName) != speakingPlayers.end())
			return 1;

		return 0;
	}

	__declspec(naked) void chud_update_player_marker_state_hook()
	{
		int _ebx, _ecx, _edx, _ebp, _edi, _esp;
		int markerSecondary;

		__asm
		{
			mov markerSecondary, eax

			//preserve registers 
			mov _ebx, ebx
			mov _ecx, ecx
			mov _edx, edx
			mov _ebp, ebp
			mov _edi, edi
			mov _esp, esp

			//check if player is speaking 
			mov eax, [ebp + 0xC]
			push eax
			call IsPlayerSpeaking

			//check result from is player speaking 
			cmp eax, 0
			je check_marker_secondary
			mov eax, 2
			mov markerSecondary, eax //1 = shooting, 2 = speaking, 3 = taking damage 

			check_marker_secondary :

			//restore registers 
			mov ebx, _ebx
				mov edx, _edx
				mov ebp, _ebp
				mov edi, _edi
				mov esp, _esp
				mov eax, markerSecondary

				dec eax
				jz shooting

				dec eax
				jz speaking

				//taking_damage: 
				dec eax
				mov al, byte ptr[ebp + 0xB]
				jnz loc_749469
				or dword ptr[ebx + 0x10], 8
				jmp loc_749469

				shooting :
			mov esi, 0x74947F
				jmp esi

				speaking :
			mov esi, 0x749462
				jmp esi

				loc_749469 :
			mov esi, 0x749469
				jmp esi
		}
	}

	//TODO:  
	//Refactor 
	enum class PlayerMarkerIconIndex : int
	{
		None = -1,
		Speaker,
		DeathCross,
		Tick,
		Juggernaut,
		Unknown,
		Bomb,
		Flag,
		Skull,
		King,
		VIP,
		Padlock,
		One,
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Nine
	};

	enum PlayerMarkerBitmapSpriteIndex : int
	{
		Ally,
		Enemy,
		Objective,
		None
	};

	enum class WaypointTrait : int8_t
	{
		Unchanged,
		NoMarker,
		MarkerVisibleToAllies,
		MarkerVisibleToEveryone,
		NoName, //Added by Unk 
		MarkerVisibleToAlliesNameInvisibleToEnemies //Added by Unk 
	};

	PlayerMarkerBitmapSpriteIndex __stdcall GetPlayerMarkerSpriteIndex(int handle, int markerColorIndex)
	{
		using Blam::GameType;

		auto player = Blam::Players::GetPlayers().Get(handle);
		if (!player)
			return PlayerMarkerBitmapSpriteIndex::None;

		auto session = Blam::Network::GetActiveSession();
		if (!session || !session->IsEstablished())
			return PlayerMarkerBitmapSpriteIndex::None;

		GameType gamemode = (GameType)session->Parameters.GameVariant.Get()->GameType;

		WaypointTrait playerWaypointTrait = *(WaypointTrait*)((uint8_t*)player + 0x2DC1);

		//TODO: add armour colors fix. 
		if (markerColorIndex == 0)
		{
			switch (playerWaypointTrait)
			{
			case WaypointTrait::MarkerVisibleToAllies:
			case WaypointTrait::MarkerVisibleToAlliesNameInvisibleToEnemies:
			case WaypointTrait::MarkerVisibleToEveryone:
			case WaypointTrait::Unchanged:
				return PlayerMarkerBitmapSpriteIndex::Ally;
			default:
				return PlayerMarkerBitmapSpriteIndex::None;
			}
		}
		else
		{
			switch (playerWaypointTrait)
			{
			case WaypointTrait::MarkerVisibleToAllies:
			case WaypointTrait::MarkerVisibleToAlliesNameInvisibleToEnemies:
			case WaypointTrait::NoMarker:
			case WaypointTrait::NoName:
				return PlayerMarkerBitmapSpriteIndex::None;
			case WaypointTrait::MarkerVisibleToEveryone:
				return PlayerMarkerBitmapSpriteIndex::Enemy;
			case WaypointTrait::Unchanged:
			{
				//Any unchanged players holding objectives have icons above their head,  
				//so another hook can check the icon and give them a marker. 
				//The exception to this is infection, which gives the last man standing a marker. 
				//So that's handled here. 
				if (gamemode == GameType::eGameTypeInfection)
				{
					bool thereIsALastManStanding = Pointer(HaloOnline::GetMainTls(0x48 + 0xE6DC)).Read<uint16_t>() == 1;
					bool iAmAlive = Pointer(0x2161808).Read<uint8_t>() == 1;
					bool iAmAZombie = Pointer(HaloOnline::GetMainTls(0x48 + 0xE190)).Read<uint16_t>() == 1;
					if (thereIsALastManStanding && (iAmAlive || iAmAZombie))
						return PlayerMarkerBitmapSpriteIndex::Enemy;
				}

				return PlayerMarkerBitmapSpriteIndex::None;
			}
			}
		}
		return PlayerMarkerBitmapSpriteIndex::None;
	}

	bool __stdcall DoesPlayerMarkerHaveObjective(PlayerMarkerIconIndex markerIconIndex)
	{
		using Blam::GameType;

		auto session = Blam::Network::GetActiveSession();
		if (!session || !session->IsEstablished())
			return false;

		GameType gamemode = (GameType)session->Parameters.GameVariant.Get()->GameType;

		switch (gamemode)
		{
		default:
			break;
		case GameType::eGameTypeAssault:
			if (markerIconIndex == PlayerMarkerIconIndex::Bomb)
				return true;
			break;
		case GameType::eGameTypeCTF:
			if (markerIconIndex == PlayerMarkerIconIndex::Flag)
				return true;
			break;
		case GameType::eGameTypeOddball:
			if (markerIconIndex == PlayerMarkerIconIndex::Skull)
				return true;
			break;
		case GameType::eGameTypeJuggernaut:
			if (markerIconIndex == PlayerMarkerIconIndex::Juggernaut)
				return true;
			break;
		case GameType::eGameTypeVIP:
			if (markerIconIndex == PlayerMarkerIconIndex::VIP)
				return true;
			break;
		}

		return false;
	}


	__declspec(naked) void chud_update_player_marker_sprite_hook()
	{
		int _ebx, _ecx, _edx, _ebp, _edi, _esp;
		__asm
		{
			//preserve registers 
			mov _ebx, ebx
			mov _ecx, ecx
			mov _edx, edx
			mov _ebp, ebp
			mov _edi, edi
			mov _esp, esp

			cmp dword ptr[ebx + 8], 0xFFFFFFFF
			jnz ed_return


			mov _ebp, ebp
			mov ebx, _ebx
			mov eax, [ebx + 0xC] //color 
			push eax
			mov eax, [ebp + 0xC] //handle 
			push eax
			call GetPlayerMarkerSpriteIndex

			//restore registers 
			mov edx, _edx
			mov ebp, _ebp
			mov edi, _edi
			mov esp, _esp

			mov dword ptr[ebx + 4], eax
			mov eax, 1 //Not sure why this is required, but if 0 waypoints hide. 

			ed_return:
			mov esi, 0x749476
				jmp esi
		}
	}

	__declspec(naked) void chud_update_marker_sprite_hook()
	{
		__asm
		{
			push eax

			cmp[esi - 0x2C], 3 //3 = no marker 
			jne fix_armour_colors

			mov eax, [esi - 0x28]
			push eax
			call DoesPlayerMarkerHaveObjective

			cmp eax, 0
			je fix_armour_colors

			cmp[esi - 0x24], 2 //enemy color 
			jne is_ally_with_objective
			mov[esi - 0x2C], 1 //enemy marker 
			jmp fix_armour_colors

			is_ally_with_objective :
			cmp[esi - 0x24], 0 //ally color 
				jne fix_armour_colors
				mov[esi - 0x2C], 0 //ally marker 

				fix_armour_colors :
				cmp playerMarkers, 2
				jne fix_alt_ally_color
				cmp[esi - 0x24], 3
				je eldorado_return
				cmp[esi - 0x2C], 0
				je is_player_marker
				cmp[esi - 0x2C], 1
				je is_player_marker
				cmp[esi - 0x2C], 3
				je is_player_marker
				jmp eldorado_return
				is_player_marker :
			mov[esi - 0x24], 5 //armour colors 

				fix_alt_ally_color : //ally_blue 
				cmp playerMarkers, 1
				jne eldorado_return
				cmp[esi - 0x24], 0
				jne eldorado_return
				mov[esi - 0x24], 4


				eldorado_return :
				pop eax
				//perform original instruction 
				lea ecx, [esi + 0xC]
				push ecx
				mov byte ptr[ecx], 01

				mov eax, 0xACED71
				jmp eax
		}
	}

	__declspec(naked) void chud_update_player_marker_icon_height_hook()
	{
		__asm
		{
			movss xmm0, [ebp - 0xC]

			pop ecx

			push eax
			push ebx
			push ecx
			push edx

			lea     ebx, [esi - 0x18]

			mov     ecx, 5
			xor eax, eax
			test    ecx, ecx
			jz      short loc_4ECBEC
			nop
			loc_4ECBE0 :
			cmp     word ptr[ebx + eax * 2], 0
				jz      short loc_4ECBEC
				inc     eax
				cmp     eax, ecx
				jb      short loc_4ECBE0
				loc_4ECBEC :

			test    eax, eax
				mov     al, [ebp - 1]
				jne		raise_icon
				test    al, al
				je      eldorado_return

				raise_icon :
			mov eax, 0x40300000 //2.75f, may need adjustment 
				movd xmm1, eax
				movss xmm0, [ebp - 0xC]
				mulss xmm0, xmm1

				eldorado_return :

			pop edx
				pop ecx
				pop ebx
				pop eax

				lea ecx, [ebp - 0x1A64]
				push ecx

				mov edx, 0xACF0A5
				jmp edx
		}

	}

	__declspec(naked) void chud_update_player_marker_name_height_hook()
	{
		__asm
		{
			mov[ebp - 0xC], 0xC1C00000

			mov ebx, 0xACF0D3
			jmp ebx
		}
	}

	void __fastcall c_start_menu_pane_screen_widget__handle_spinner_chosen_hook(void *thisptr, void *unused, uint8_t *widget)
	{
		const auto c_gui_list_widget__set_selected = (void(__thiscall *)(void *thisptr, int target, int value, char a5))(0xB16170);
		const auto game_engine_get_map_variant = (uint8_t*(*)())(0x00583230);
		const auto _string_id_value = 0x111;

		switch (*(uint32_t*)(widget + 0x40))
		{
		case 0x10E: //  change_teams
		{
			auto teamIndex = *(int*)((uint8_t*)0x019A03D8 + 0x167C);
			if (teamIndex < 0)
				teamIndex = 0;
			c_gui_list_widget__set_selected(widget, 0x111, teamIndex, 0);
		}
		break;
		case 0x105B9: // change_gametype
			auto selectedGametype = 0;
			auto contentType = *(int*)(game_engine_get_map_variant() + 0x11c);
			switch (contentType)
			{
			case 0:
				selectedGametype = 1;
				break;
			case 1:
				selectedGametype = 2;
				break;
			case 2:
				selectedGametype = 3;
				break;
			case 3:
				selectedGametype = 4;
				break;
			case 4:
				selectedGametype = 7;
				break;
			case 5:
				selectedGametype = 8;
				break;
			case 6:
				selectedGametype = 9;
				break;
			case 7:
				selectedGametype = 6;
				break;
			case 8:
				selectedGametype = 10;
				break;
			case 10:
				selectedGametype = 0;
				break;
			}
			c_gui_list_widget__set_selected(widget, 0x111, selectedGametype, 0);
			break;
		}
	}

	void __fastcall UI_GameVariantSavePromptFix(void *thisptr, void *unused, int a2)
	{
		const auto c_gui_screen_widget__transition_out = (void(__thiscall*)(void *thisptr, int a2))(0x00AB2830);

		auto name = *(uint32_t*)((uint8_t*)thisptr + 0x40);
		if (name != 0x103A9) // game_options
			c_gui_screen_widget__transition_out(thisptr, a2);
	}


	std::string teamNames[8] = { "Red", "Blue", "Green", "Orange", "Purple", "Gold", "Brown", "Pink" };

	const float winnereDisplayTime = 5;
	uint32_t winnerDisplayed;
	std::wstring winnerString;

	const float welcomeDisplayTime = 5;
	uint32_t welcomeDisplayed;
	bool welcome;
	//TODO: Find a better way to determine this. 
	//It defaults to true because it's set to false in game if the game hasn't started, 
	//but remains true if the player joins a game that has already started. 
	bool gameHasStarted = true;

	bool respawning;

	void OnEvent(Blam::DatumHandle player, const Blam::Events::Event *event, const Blam::Events::EventDefinition *definition)
	{
		if (event->NameStringId == 0x4004D) // "general_event_game_over" 
		{
			winnerDisplayed = Blam::Time::GetGameTicks();
			auto session = Blam::Network::GetActiveSession();
			auto get_multiplayer_scoreboard = (Blam::MutiplayerScoreboard*(*)())(0x00550B80);
			auto* scoreboard = get_multiplayer_scoreboard();

			if (!session || !session->IsEstablished() || !scoreboard)
				return;

			std::wstringstream ss;

			if (session->HasTeams())
			{
				bool tied = false;
				int prevHighTeam = 0;
				int16_t prevHighScore = scoreboard->TeamScores[0].TotalScore;
				for (int t = 1; t < 8; t++)
				{
					uint16_t currentScore = scoreboard->TeamScores[t].TotalScore;
					if (prevHighScore == currentScore)
					{
						tied = true;
					}
					else if (currentScore > prevHighScore)
					{
						prevHighTeam = t;
						prevHighScore = currentScore;
						tied = false;
					}
				}

				std::string teamName = teamNames[prevHighTeam];

				if (tied)
					ss << L"Tie game!";
				else
					ss << Utils::String::WidenString(teamName) << L" Team wins!";
			}
			else
			{
				int playerIdx = session->MembershipInfo.FindFirstPlayer();
				std::wstring prevHighPlayer;
				std::uint16_t prevHighScore;
				bool tied = false;

				if (playerIdx == -1)
					return;

				auto player = session->MembershipInfo.PlayerSessions[playerIdx];
				prevHighPlayer = player.Properties.DisplayName;
				prevHighScore = scoreboard->PlayerScores[playerIdx].TotalScore;

				playerIdx = session->MembershipInfo.FindNextPlayer(playerIdx);

				while (playerIdx != -1)
				{
					uint16_t currentScore = scoreboard->PlayerScores[playerIdx].TotalScore;
					if (currentScore == prevHighScore)
					{
						tied = true;
					}
					else if (currentScore > prevHighScore)
					{
						player = session->MembershipInfo.PlayerSessions[playerIdx];
						prevHighScore = currentScore;
						prevHighPlayer = player.Properties.DisplayName;
					}
					playerIdx = session->MembershipInfo.FindNextPlayer(playerIdx);
				}

				if (tied)
					ss << L"Tie game!";
				else
					ss << prevHighPlayer << L" wins!";
			}

			winnerString = ss.str();
		}
	}

	bool HUDStateDisplayHook(int hudIndex, wchar_t* buff, int len, int a4)
	{
		const auto game_engine_round_in_progress = (bool(*)())(0x00550F90);
		const auto sub_6E4AA0 = (bool(__cdecl *)(int a1, wchar_t *DstBuf, int bufflen, char a4))(0x6E4AA0);
		if (sub_6E4AA0(hudIndex, buff, len, a4))
			return true;
		auto playerIndex = Blam::Players::GetLocalPlayer(0);
		if (playerIndex == Blam::DatumHandle::Null)
			return false;
		auto player = Blam::Players::GetPlayers().Get(playerIndex);
		if (!player)
			return false;

		//TODO:
		//Use strings from tags as templates, rather than hardcoding.

		//mp_respawn_timer 
		auto secondsUntilSpawn = Pointer(player)(0x2CBC).Read<int>();
		auto firstTimeSpawning = Pointer(player)(0x4).Read<uint32_t>() & 8;
		if (player->SlaveUnit == Blam::DatumHandle::Null && secondsUntilSpawn > 0)
		{
			if (!game_engine_round_in_progress())
			{
				return false;
			}

			respawning = true;

			if (firstTimeSpawning)
			{
				swprintf(buff, L"Spawn in %d", secondsUntilSpawn);
				gameHasStarted = false; //Make sure it's reset after games. 
			}
			else
				swprintf(buff, L"Respawn in %d", secondsUntilSpawn);

			return true;
		}

		//state_recently_started 
		if (!firstTimeSpawning && !gameHasStarted)
		{
			welcome = true;
			welcomeDisplayed = Blam::Time::GetGameTicks();
			gameHasStarted = true;
			swprintf(buff, L"Welcome!");
			return true;
		}
		if (welcome)
		{
			swprintf(buff, L"Welcome!");

			if (Blam::Time::TicksToSeconds(Blam::Time::GetGameTicks() - welcomeDisplayed) > welcomeDisplayTime)
				welcome = false;

			return true;
		}

		//state_game_over_lost_* 
		if (winnerString != L"")
		{
			swprintf(buff, winnerString.c_str());

			if (Blam::Time::TicksToSeconds(Blam::Time::GetGameTicks() - winnerDisplayed) > winnereDisplayTime)
				winnerString = L"";

			return true;
		}

		return false;
	}
}
