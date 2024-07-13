#include "Blam\Tags\Sounds\SoundClasses\SoundClasses.hpp"

namespace Blam::Tags::Sounds {
	ClassProperty* GetClassProperty(Classes* classes, SoundClass soundClass) {
		return &classes->ClassProperties[(int)soundClass];
	}
}