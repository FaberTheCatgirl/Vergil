#include <tagdefinitions-private-pch.h>
#include <blofeld_field_type_override.h>

namespace blofeld
{

	TAG_BLOCK(decorator_palette, MAXIMUM_DECORATOR_PALETTES_PER_SCENARIO)
	{
		{ _field_string_id, "name^" },
		{ _field_short_block_index, "decorator set 0" },
		{ _field_word_integer, "decorator weight 0" },
		{ _field_short_block_index, "decorator set 1" },
		{ _field_word_integer, "decorator weight 1" },
		{ _field_short_block_index, "decorator set 2" },
		{ _field_word_integer, "decorator weight 2" },
		{ _field_short_block_index, "decorator set 3" },
		{ _field_word_integer, "decorator weight 3" },
		{ _field_short_block_index, "decorator set 4" },
		{ _field_word_integer, "decorator weight 4" },
		{ _field_short_block_index, "decorator set 5" },
		{ _field_word_integer, "decorator weight 5" },
		{ _field_short_block_index, "decorator set 6" },
		{ _field_word_integer, "decorator weight 6" },
		{ _field_short_block_index, "decorator set 7" },
		{ _field_word_integer, "decorator weight 7" },
		{ _field_terminator }
	};

	TAG_BLOCK(global_decorator_placement_block, MAXIMUM_DECORATOR_PLACEMENTS_PER_SET)
	{
		{ _field_real_point_3d, "position" },
		{ _field_byte_integer, "type index" },
		{ _field_byte_integer, "motion scale" },
		{ _field_byte_integer, "ground tint" },
		{ _field_byte_flags, "flags", &decorator_placement_flags_definition },
		{ _field_real_quaternion, "rotation" },
		{ _field_real, "scale" },
		{ _field_real_point_3d, "tint color" },
		{ _field_real_point_3d, "original point" },
		{ _field_real_point_3d, "original normal" },
		{ _field_long_integer, "bsp index!" },
		{ _field_short_integer, "cluster index!" },
		{ _field_short_integer, "cluster decorator set index!" },
		{ _field_terminator }
	};

	TAG_BLOCK(decorator_scenario_set_block, MAXIMUM_DECORATOR_SETS_PER_SCENARIO)
	{
		{ _field_tag_reference, "decorator set^", &global_decorator_set_reference },
		{ _field_block, "placements*", &global_decorator_placement_block_block },
		{ _field_terminator }
	};

	TAG_BLOCK(scenario_decorator_block, 1)
	{
		{ _field_struct, "brush", &decorator_brush_struct_struct_definition },
		{ _field_long_integer, "decorator count*" },
		{ _field_long_integer, "current bsp count*" },
		{ _field_real_vector_3d, "global offset!" },
		{ _field_real_vector_3d, "global x!" },
		{ _field_real_vector_3d, "global y!" },
		{ _field_real_vector_3d, "global z!" },
		{ _field_block, "palette", &decorator_palette_block },
		{ _field_block, "sets", &decorator_scenario_set_block_block },
		{ _field_terminator }
	};

	TAG_STRUCT(decorator_brush_struct)
	{
		{ _field_long_enum, "left button brush", &decorator_left_brush_type_enum_definition },
		{ _field_long_enum, "middle button brush", &decorator_right_brush_type_enum_definition },
		{ _field_long_enum, "control+left button brush", &decorator_left_brush_type_enum_definition },
		{ _field_long_enum, "control+middle button brush", &decorator_right_brush_type_enum_definition },
		{ _field_long_enum, "alt+left button brush", &decorator_left_brush_type_enum_definition },
		{ _field_long_enum, "alt+middle button brush", &decorator_right_brush_type_enum_definition },
		{ _field_real, "outer radius!" },
		{ _field_real, "feather percent!" },
		{ _field_byte_flags, "reapply flags", &decorator_brush_reapply_flags_definition },
		{ _field_byte_flags, "render flags", &decorator_brush_render_flags_definition },
		{ _field_byte_flags, "action flags", &decorator_brush_action_flags_definition },
		{ _field_char_enum, "brush shape", &decorator_brush_shape_enum_definition },
		{ _field_long_block_index, "current palette" },
		{ _field_long_block_index, "current set" },
		{ _field_long_integer, "current type" },
		{ _field_real, "paint rate:[0 - 1]" },
		{ _field_real_rgb_color, "paint color" },
		{ _field_real, "move distance#drop height for drop to ground" },
		{ _field_real, "angle snap interval#rotate brushes will snap to intervals of this" },
		{ _field_real, "Editor cull distance#decorators will not draw beyond this distance from the camera" },
		{ _field_explanation, "KEYS", "[] \t brush size\n-= \t brush feathering\n0-9 \t brush opacity\nSHIFT+0-9\t select palette\nSHIFT\t while painting will reverse the operation\n" },
		{ _field_terminator }
	};

	STRINGS(decorator_placement_flags_definition)
	{
		"UNUSED!",
		"UNUSED2!"
	};
	STRING_LIST(decorator_placement_flags_definition, decorator_placement_flags_definition_strings, _countof(decorator_placement_flags_definition_strings));

	STRINGS(decorator_left_brush_type_enum_definition)
	{
		"fill add",
		"airbrush add",
		"airbrush color",
		"airbrush erase",
		"density smooth",
		"precision place",
		"precision delete",
		"scale",
		"scale additive",
		"scale subtractive",
		"rotate random",
		"rotate normal",
		"rotate local",
		"eraser",
		"reapply type settings",
		"raise to ground (use reapply flags)",
		"drop to ground (use reapply flags)",
		"comb",
		"thin"
	};
	STRING_LIST(decorator_left_brush_type_enum_definition, decorator_left_brush_type_enum_definition_strings, _countof(decorator_left_brush_type_enum_definition_strings));

	STRINGS(decorator_right_brush_type_enum_definition)
	{
		"fill add",
		"airbrush add",
		"airbrush color",
		"airbrush erase",
		"density smooth",
		"precision place",
		"precision delete",
		"scale",
		"scale additive",
		"scale subtractive",
		"rotate random",
		"rotate normal",
		"rotate local",
		"eraser",
		"reapply type settings",
		"raise to ground (use reapply flags)",
		"drop to ground (use reapply flags)",
		"comb",
		"thin"
	};
	STRING_LIST(decorator_right_brush_type_enum_definition, decorator_right_brush_type_enum_definition_strings, _countof(decorator_right_brush_type_enum_definition_strings));

	STRINGS(decorator_brush_reapply_flags_definition)
	{
		"reapply hover",
		"reapply orientation",
		"reapply scale",
		"reapply motion",
		"reapply color",
		"reapply ground tint",
		"reapply all decorators"
	};
	STRING_LIST(decorator_brush_reapply_flags_definition, decorator_brush_reapply_flags_definition_strings, _countof(decorator_brush_reapply_flags_definition_strings));

	STRINGS(decorator_brush_render_flags_definition)
	{
		"render preview",
		"render in radius only",
		"render selected only",
		"dont render lines"
	};
	STRING_LIST(decorator_brush_render_flags_definition, decorator_brush_render_flags_definition_strings, _countof(decorator_brush_render_flags_definition_strings));

	STRINGS(decorator_brush_action_flags_definition)
	{
		"clamp scale",
		"enforce minimum distance",
		"select all decorator sets",
		"use global up",
		"constrain to a single channel"
	};
	STRING_LIST(decorator_brush_action_flags_definition, decorator_brush_action_flags_definition_strings, _countof(decorator_brush_action_flags_definition_strings));

	STRINGS(decorator_brush_shape_enum_definition)
	{
		"flattened sphere",
		"spherical",
		"tall sphere",
		"floating sphere"
	};
	STRING_LIST(decorator_brush_shape_enum_definition, decorator_brush_shape_enum_definition_strings, _countof(decorator_brush_shape_enum_definition_strings));

} // namespace blofeld

