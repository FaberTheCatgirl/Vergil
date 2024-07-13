#pragma once
#include "Blam\Tags\Tags.hpp"
#include "Blam\Tags\Sounds\SoundClasses\ClassProperty.hpp"
#include "Blam\Tags\Sounds\SoundClasses\SoundClassEnum.hpp"

namespace Blam::Tags::Sounds
{
	struct Classes : TagGroup<'sncl'>
	{
		TagBlock<ClassProperty> ClassProperties;
		uint32_t Unknown;
	};
	TAG_STRUCT_SIZE_ASSERT(Classes, 0x10);

	ClassProperty* GetClassProperty(Classes* classes, SoundClass soundClass);
}
