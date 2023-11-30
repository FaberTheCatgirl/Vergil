#pragma once

namespace blofeld
{

	constexpr unsigned long SCENERY_TAG = 'scen';

	extern s_tag_group scenery_group;

	extern s_tag_block_definition scenery_block_block;

	extern s_tag_struct_definition scenery_group_struct_definition; // tag group

	extern c_versioned_string_list pathfinding_policy_enum$2_strings;
	extern s_string_list_definition pathfinding_policy_enum$2;
	extern c_versioned_string_list lightmapping_policy_enum_strings;
	extern s_string_list_definition lightmapping_policy_enum;
	extern c_versioned_string_list scenery_flags_strings;
	extern s_string_list_definition scenery_flags;

} // namespace blofeld

