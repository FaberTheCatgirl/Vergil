#include <tagdefinitions-private-pch.h>
#include <blofeld_field_type_override.h>

namespace blofeld
{

	TAG_GROUP_INHERIT_FROM_BLOCK(scenery, SCENERY_TAG, object, OBJECT_TAG, scenery_block_block );

	TAG_BLOCK_FROM_STRUCT(scenery_block, 1, scenery_group_struct_definition );

	TAG_STRUCT(scenery_group)
	{
		{ _field_struct, "object", &object_struct_definition_struct_definition },
		{ _field_explanation, "Pathfinding", "Indicate whether, by default, we should create pathfinding data for this type of scenery" },
		{ _field_enum, "pathfinding policy", &pathfinding_policy_enum },
		{ _field_word_flags, "flags", &scenery_flags },
		{ _field_explanation, "Lightmapping", "Indicate whether, by default, how we should lightmap this type of scenery" },
		{ _field_enum, "lightmapping policy", &lightmapping_policy_enum },
		{ _field_pad, "WGW", 2 },

		{ _field_version_greater, _engine_type_haloreach, 2 },
		{ _field_tag_reference, "Structure Lighting Tag", &Tag::Reference<struct s_structure_lighting_info>::s_defaultDefinition },
		{ _field_useless_pad },

		{ _field_terminator }
	};

	STRINGS(pathfinding_policy_enum$2)
	{
		"Pathfinding CUT-OUT",
		"Pathfinding STATIC",
		"Pathfinding DYNAMIC",
		"Pathfinding NONE"
	};
	STRING_LIST(pathfinding_policy_enum$2, pathfinding_policy_enum$2_strings, _countof(pathfinding_policy_enum$2_strings));

	STRINGS(lightmapping_policy_enum)
	{
		"Per-Vertex",
		"Per-Pixel (not implemented)",
		"Dynamic"
	};
	STRING_LIST(lightmapping_policy_enum, lightmapping_policy_enum_strings, _countof(lightmapping_policy_enum_strings));

	STRINGS(scenery_flags)
	{
		"not physical#has no havok representation; will not build physics from collision",
		"use complex activation#tests all clusters for activation instead of just the origin"
	};
	STRING_LIST(scenery_flags, scenery_flags_strings, _countof(scenery_flags_strings));

} // namespace blofeld

