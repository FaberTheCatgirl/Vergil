#pragma once
#include "Bungie\Math\RealPoint3D.hpp"
#include "Bungie\Math\RealQuaternion.hpp"

namespace Bungie::Math
{
	struct RealOrientation3D
	{
		RealQuaternion Rotation;
		RealPoint3D Translation;
		float Scale;

		RealOrientation3D();
		RealOrientation3D(const RealQuaternion &rotation, const RealPoint3D &translation, const float scale);

		bool operator==(const RealOrientation3D &other) const;
		bool operator!=(const RealOrientation3D &other) const;
	};
}

