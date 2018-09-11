#pragma once

#include <cseries\cseries.hpp>
#include <memory\data.hpp>
#include <tag_files\tag_definitions.hpp>

#include "object_data.hpp"

namespace blam
{
	enum
	{
		_object_type_none = -1,

		_object_type_biped,
		_object_type_vehicle,
		_object_type_weapon,
		_object_type_equipment,
		_object_type_arg_device,
		_object_type_terminal,
		_object_type_projectile,
		_object_type_scenery,
		_object_type_machine,
		_object_type_control,
		_object_type_sound_scenery,
		_object_type_crate,
		_object_type_creature,
		_object_type_giant,
		_object_type_effect_scenery,

		_object_type_mask_device =
			(1 << _object_type_arg_device) |
			(1 << _object_type_terminal) |
			(1 << _object_type_machine) |
			(1 << _object_type_control),

		_object_type_mask_item =
			(1 << _object_type_weapon) |
			(1 << _object_type_equipment),

		_object_type_mask_motor =
			(1 << _object_type_biped) |
			(1 << _object_type_vehicle) |
			(1 << _object_type_creature) |
			(1 << _object_type_giant),

		_object_type_mask_unit =
			(1 << _object_type_biped) |
			(1 << _object_type_vehicle) |
			(1 << _object_type_giant),

		_object_type_mask_all =
			(1 << _object_type_projectile) |
			(1 << _object_type_scenery) |
			(1 << _object_type_sound_scenery) |
			(1 << _object_type_crate) |
			(1 << _object_type_effect_scenery) |
			_object_type_mask_device |
			_object_type_mask_item |
			_object_type_mask_motor |
			_object_type_mask_unit
	};

	struct object_type_definition
	{
		const char *name;
		tag group_tag;
		short datum_size;
		short placement_tag_block_offset;
		short palette_tag_block_offset;
		short placement_tag_block_size;
		long_flags placement_flags;
		long unknown14;
		void *mover_definition_data;
		void(__cdecl *initialize)(object_type_definition *type, long);
		void *dispose;
		void *unknown24;
		void *unknown28;
		void *unknown2C;
		void(__cdecl *unknown30)(datum_index object_index);
		void *(__cdecl *adjust_placement)(object_placement_data *placement);
		bool(*_new)(datum_index index, word *, byte *);
		void *(*place)(long, long);
		void *unknown40;
		void(__cdecl *unknown44)(datum_index object_index);
		void(__cdecl *unknown48)(datum_index object_index);
		void *unknown4C;
		void *unknown50;
		void *unknown54;
		long(__cdecl *unknown58)(long, long, long);
		long(__cdecl *unknown5C)(long, long, long, dword *, byte *, long);
		void(__cdecl *unknown60)(datum_index object_index);
		void(__cdecl *unknown64)(datum_index object_index);
		long(__cdecl *unknown68)(object_type_definition **);
		long(__cdecl *unknown6C)(long);
		long(__cdecl *unknown70)(datum_index object_index);
		void *unknown74;
		word unknown78;
		word unknown7A;
		void *unknown7C;
		void *unknown80;
		void *unknown84;
		void(__cdecl *unknown88)(long, long, long, long);
		void(__cdecl *unknown8C)(datum_index, datum_index);
		void(__cdecl *unknown90)(datum_index, datum_index);
		void *unknown94;
		void *unknown98;
		void *unknown9C;
		void *unknownA0;
		void *unknownA4;
		void *unknownA8;
		object_type_definition *type_definitions[16];
		object_type_definition *next;
		long unknown108;
		short unknown10C;
		short unknown10E;
	};
	static_assert(sizeof(object_type_definition) == 0xF8);
}