#pragma once
#include "new\cseries\cseries.hpp"
#include "new\memory\data.hpp"
#include "new\tag_files\tag_groups.hpp"

namespace blam
{
	struct pageable_resource
	{
		char : 8;
		char : 8;
		byte_flags location_flags;
		char compression_type;
		long index;
		unsigned long compressed_size;
		unsigned long decompressed_size;
		long crc_checksum;
		unsigned long raw_size;
		tag_reference owner;
		unsigned short salt;
		char type;
		char : 8;
		tag_data definition_data;
		data_address definition_address;
		tag_block definition_fixups;
		tag_block d3d_object_fixups;
		long : 32;
	};
	static_assert(sizeof(pageable_resource) == 0x60);
}