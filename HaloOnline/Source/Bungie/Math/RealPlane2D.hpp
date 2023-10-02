#pragma once
#include "Bungie\Math\RealVector2D.hpp"

namespace Bungie::Math
{
	struct RealPlane2D
	{
		RealVector2D Normal;
		float Distance;

		RealPlane2D();
		RealPlane2D(const RealVector2D &normal, const float distance);
		RealPlane2D(const float i, const float j, const float d);

		bool operator==(const RealPlane2D &other) const;
		bool operator!=(const RealPlane2D &other) const;
	};
}

