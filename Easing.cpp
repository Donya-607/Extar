#include "Easing.h"

namespace Easing
{
	float   Interpolate( float   start, float   end, float resistance )
	{
		float  dist = end - start;
		return dist * resistance;
	}
	Vector2 Interpolate( Vector2 start, Vector2 end, float resistance )
	{
		Vector2 dist = end - start;
		return  dist * resistance;
	}
}