#include <tagdefinitions-private-pch.h>
#include <blofeld_field_type_override.h>

namespace blofeld
{

	TAG_GROUP_FROM_BLOCK(variant_globals, VARIANT_GLOBALS_TAG, variant_globals_block_block );

	TAG_BLOCK(single_variant_block, s_variant_globals_definition::k_variant_globals_per_block_count_maximum)
	{
		{ _field_data, "metadata" },
		{ _field_data, "full_data" },
		{ _field_long_string, "filename^" },
		{ _field_terminator }
	};

	TAG_BLOCK(FallbackHopperFileBlock, s_variant_globals_definition::k_hopper_files_maximum_count)
	{
		{ _field_data, "file_data" },
		{ _field_long_string, "filename^" },
		{ _field_terminator }
	};

	TAG_BLOCK_FROM_STRUCT(variant_globals_block, 1, variant_globals_struct_definition_struct_definition );

	TAG_STRUCT(variant_globals_struct_definition)
	{
		{ _field_block, "map_variants", &single_variant_block_block },
		{ _field_block, "game_variants", &single_variant_block_block },
		{ _field_block, "fallback_hoppers", &FallbackHopperFileBlock_block },
		{ _field_terminator }
	};

} // namespace blofeld

