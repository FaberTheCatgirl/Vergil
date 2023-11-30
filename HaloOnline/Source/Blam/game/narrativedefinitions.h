#pragma once

namespace blofeld
{

	constexpr unsigned long NARRATIVEGLOBALS_TAG = 'narg';

	extern s_tag_group NarrativeGlobals_group;

	extern s_tag_struct_definition NarrativeFlagDefinitionBlock_block_struct;
	extern s_tag_block_definition NarrativeFlagDefinitionBlock_block;
	extern s_tag_block_definition NarrativeGlobals_block_block;

	extern s_tag_struct_definition NarrativeGlobals_struct_definition_struct_definition; // tag group

	extern c_versioned_string_list NarrativeFlagTypeEnum_strings;
	extern s_string_list_definition NarrativeFlagTypeEnum;

	extern s_tag_reference_definition NarrativeGlobalsReference;

} // namespace blofeld

