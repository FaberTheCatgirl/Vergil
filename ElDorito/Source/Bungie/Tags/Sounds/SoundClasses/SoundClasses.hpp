#pragma once
#include "Bungie\Tags\Tags.hpp"
#include "Bungie\Tags\Sounds\SoundClasses\ClassProperty.hpp"
#include "Bungie\Tags\Sounds\SoundClasses\SoundClassEnum.hpp"

namespace Bungie::Tags::Sounds
{
	struct Classes : TagGroup<'sncl'>
	{
		TagBlock<ClassProperty> ClassProperties;
		uint32_t Unknown;
	};
	//TAG_STRUCT_SIZE_ASSERT(Classes, 0x10);

	ClassProperty* GetClassProperty(Classes* classes, SoundClass soundClass);
}
