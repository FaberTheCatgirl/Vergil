#pragma once

namespace blofeld
{

	constexpr unsigned long WEATHER_GLOBALS_TAG = 'wxcg';

	extern s_tag_group weather_globals_group;

	extern s_tag_struct_definition global_textures_refs_block_block_struct;
	extern s_tag_block_definition global_textures_refs_block_block;
	extern s_tag_block_definition weather_globals_block_block;

	extern s_tag_struct_definition weather_globals_struct_definition_struct_definition; // tag group
	extern s_tag_struct_definition rain_ripple_setting_block_struct_definition;

	extern s_tag_reference_definition global_weather_globals_reference;

} // namespace blofeld

