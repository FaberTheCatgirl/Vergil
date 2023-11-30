#pragma once

namespace blofeld
{

	constexpr unsigned long SOUND_LOOPING_TAG = 'lsnd';

	extern s_tag_group sound_looping_group;

	extern s_tag_block_definition sound_looping_block_block;

	extern s_tag_struct_definition sound_looping_struct_definition_struct_definition; // tag group

	extern c_versioned_string_list looping_sound_flags_strings;
	extern s_string_list_definition looping_sound_flags;

} // namespace blofeld

