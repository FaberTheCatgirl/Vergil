#pragma once

#include "Bungie\Math\RealVector3D.hpp"
#include "Bungie\Math\RealQuaternion.hpp"
#include <cstdint>

namespace Forge::RotationSnap
{
	void Update(uint32_t playerIndex, uint32_t objectIndex);
	void RotateSnapped(const Bungie::Math::RealVector3D& axis);
	void RotateToScripted(uint32_t objectIndex, const Bungie::Math::RealQuaternion& rotation);
}
