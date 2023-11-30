#pragma once

namespace blofeld
{

	constexpr unsigned long NEW_CINEMATIC_LIGHTING_TAG = 'nclt';

	extern s_tag_group new_cinematic_lighting_group;

	extern s_tag_struct_definition cinematic_dynamic_light_block_block_struct;
	extern s_tag_block_definition cinematic_dynamic_light_block_block;
	extern s_tag_block_definition new_cinematic_lighting_block_block;

	extern s_tag_struct_definition new_cinematic_lighting_struct_definition_struct_definition; // tag group

	extern c_versioned_string_list cinematic_dynamic_light_flags_strings;
	extern s_string_list_definition cinematic_dynamic_light_flags;

	extern s_tag_reference_definition global_new_cinematic_lighting_reference;
	extern s_tag_reference_definition global_light_definition_reference;

} // namespace blofeld

