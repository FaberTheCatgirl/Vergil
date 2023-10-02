#include "Bungie\Tags\Sounds\SoundClasses\SoundClasses.hpp"

namespace Bungie::Tags::Sounds {
	ClassProperty* GetClassProperty(Classes* classes, SoundClass soundClass) {
		return &classes->ClassProperties[(int)soundClass];
	}
}