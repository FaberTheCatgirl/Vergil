#pragma once
#include <cstdint>

namespace Blam
{
	struct Preferences
	{
		uint8_t Unknown00;
		uint8_t IsDirty;
		uint8_t Unknown[0x1E];
		uint8_t Unknown20[0x78];
		uint8_t Unknown98[0x78];
		uint32_t Unknown110;
		uint32_t Unknown114;
		uint8_t Unknown118[0x80];
		uint8_t Unknown198[0x198]; // campaign?
		uint32_t Unknown330;
		uint32_t Unknown334;
		uint8_t Unknown338[0xE718]; // game varaint at 0x33c
		uint8_t UnknownEA50[0xE2A8]; // map variant at 0xEA58
		uint8_t Unknown1CCF8[0x24E80];
		uint8_t Unknown41B78[0x4C];
		uint32_t Unknown41BC0;
		uint8_t Unknown41BC5;
		uint8_t Unknown41BC6;
		uint8_t Unknown41BC7;
		uint8_t Unknown41BC8;
		uint8_t Fullscreen;
		uint32_t Unknown41BD0;
		uint32_t Contrast;
		uint32_t Brightness;
		uint32_t Unknown41BDC;
		uint32_t ScreenResolutionWidth;
		uint32_t ScreenResolutionHeight;
		uint32_t Unknown41BE8;
		uint32_t Unknown41BEC;
		uint32_t TextureResolution;
		uint32_t ShadowQuality;
		uint32_t TextureFilteringQuality;
		uint32_t LightingQuality;
		uint32_t EffectsQuality;
		uint32_t DetailsQuality;
		uint32_t PostprocessingQuality;
		uint8_t MotionBlur;
		uint8_t VSync;
		uint8_t Antialiasing;
		uint8_t Unknown41C0F;
		uint8_t HideWatermark;
		uint32_t DisplaySubtitles;
		uint32_t DisplayAdapter;
		uint32_t Unknown41C1C;
		uint32_t Unknown41C20;
		uint32_t Unknown41C24;
		uint32_t Unknown41C28;
		uint32_t Unknown41C2C;
		uint32_t Unknown41C30;
		uint32_t Unknown41C34;
		uint32_t Unknown41C38;
		uint32_t Unknown41C3C;
		uint32_t Unknown41C40;
		uint32_t Unknown41C44;
		uint32_t Unknown41C48;
		uint32_t Unknown41C4C;
		uint32_t MasterVolume;
		uint32_t SfxVolume;
		uint32_t MusicVolume;
		uint32_t VoiceVolume;
		uint32_t VoiceChatControl;
		uint32_t VoiceChatVolume;
		uint8_t ToggleCrouch;
		uint8_t HUDShake;
		uint8_t Unknown41C6A;
		uint8_t Unknown41C6B;
		uint32_t TeamColor;
		float CameraFov;
		uint32_t ControlsMethod;
		uint32_t MouseSensitivityVertical;
		uint32_t MouseSensitivityHorizontal;
		uint32_t MouseSensitivityVehicleVertical;
		uint32_t MouseSensitivityVehicleHorizontal;
		uint32_t MouseAcceleration;
		uint8_t MouseFilter;
		uint8_t InvertMouse;
		uint8_t Unknown41C8E;
		uint8_t Unknown41C8F;
		uint32_t Unknown41C90;
		uint8_t Bindings[0x17c];
		uint8_t Unknown41E10[0x1F0];
	};
	static_assert(sizeof(Preferences) == 0x42000);
}