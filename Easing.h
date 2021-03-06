#ifndef INCLUDED_STAR_H_
#define INCLUDED_STAR_H_

#include "Vector2.h"

namespace Easing
{
	float   Interpolate( float   start, float   end, float resistance );	// 距離に抵抗をかけた数値を返す
	Vector2 Interpolate( Vector2 start, Vector2 end, float resistance );	// 距離に抵抗をかけた数値を返す
}

#endif //INCLUDED_STAR_H_
