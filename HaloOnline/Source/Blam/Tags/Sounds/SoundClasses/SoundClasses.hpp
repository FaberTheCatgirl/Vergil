#pragma once
#include "Blamags\Tags.hpp"
#include "Blamags\Sounds\SoundClasses\ClassProperty.hpp"
#include "Blamags\Sounds\SoundClasses\SoundClassEnum.hpp"

namespace BlamTags::Sounds
{
	struct Classes : TagGroup<'sncl'>
	{
		TagBlock<ClassProperty> ClassProperties;
		uint32_t Unknown;
	};
	//TAG_STRUCT_SIZE_ASSERT(Classes, 0x10);

	ClassProperty* GetClassProperty(Classes* classes, SoundClass soundClass);
}
