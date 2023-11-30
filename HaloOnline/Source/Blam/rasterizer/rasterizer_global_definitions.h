#pragma once

namespace blofeld
{

	constexpr unsigned long RASTERIZER_GLOBALS_TAG = 'rasg';

	extern s_tag_group rasterizer_globals_group;

	extern s_tag_struct_definition default_textures_refs_block_block_struct;
	extern s_tag_block_definition default_textures_refs_block_block;
	extern s_tag_struct_definition material_textures_refs_block_block_struct;
	extern s_tag_block_definition material_textures_refs_block_block;
	extern s_tag_struct_definition explicit_shader_refs_block_block_struct;
	extern s_tag_block_definition explicit_shader_refs_block_block;
	extern s_tag_block_definition rasterizer_globals_block_block;

	extern s_tag_struct_definition rasterizer_globals_struct_definition_struct_definition; // tag group

	extern c_versioned_string_list global_bitmap_flags_strings;
	extern s_string_list_definition global_bitmap_flags;

	extern s_tag_reference_definition global_rasterizer_globals_reference;

} // namespace blofeld

