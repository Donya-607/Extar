#include "Collision.h"

#define scast static_cast

void Box::Set			( float centerX, float centerY, float halfWidth, float halfHeight, bool isExist )
{
	cx		= centerX;
	cy		= centerY;
	w		= halfWidth;
	h		= halfHeight;
	exist	= isExist;
}
bool Box::IsHitPoint	( const Box &L, const float &RX, const float &RY, bool ignoreExistFlag )
{
	if ( ignoreExistFlag && !L.exist )
	{
		return false;
	}
	// else

	if (
			L.cx - L.w	<= RX			&&	// X1 - W1	<= X2
			RX			<= L.cx + L.w	&&	// X2		<= X1 + W1
			L.cy - L.h	<= RY			&&	// Y1 - H1	<= Y2
			RY			<= L.cy + L.h		// Y2		<= Y1 + H2
		)
	{
		return true;
	}
	//else
	return false;
}
bool Box::IsHitPoint	( const Box &L, const float &LBoxScreenPosX, const float &LBoxScreenPosY, const float &RX, const float &RY, bool ignoreExistFlag )
{
	if ( ignoreExistFlag && !L.exist )
	{
		return false;
	}
	// else

	Box tmp =
	{
		L.cx + LBoxScreenPosX,
		L.cy + LBoxScreenPosY,
		L.w,
		L.h,
		true
	};
	return Box::IsHitPoint( tmp, RX, RY );
}
bool Box::IsHitBox		( const Box &L, const Box &R, bool ignoreExistFlag )
{
	if ( ignoreExistFlag && ( !L.exist || !R.exist ) )
	{
		return false;
	}
	// else

	if	(
			L.cx - L.w <= R.cx + R.w	&&	// X1 - W1 < X2 + W2
			R.cx - R.w <= L.cx + L.w	&&	// X2 - W2 < X1 + W1
			L.cy - L.h <= R.cy + R.h	&&	// Y1 - H1 < Y2 + H2
			R.cy - R.h <= L.cy + L.h		// Y2 - H2 < Y1 + H1
		)
	{
		return true;
	}
	//else
	return false;
}
bool Box::IsHitBox		( const Box &L, const float &LBoxScreenPosX, const float &LBoxScreenPosY, const Box &R, const float &RBoxScreenPosX, const float &RBoxScreenPosY, bool ignoreExistFlag )
{
	if ( ignoreExistFlag && ( !L.exist || !R.exist ) )
	{
		return false;
	}
	// else

	Box tmpL =
	{
		L.cx + LBoxScreenPosX,
		L.cy + LBoxScreenPosY,
		L.w,
		L.h,
		true
	};
	Box tmpR =
	{
		R.cx + RBoxScreenPosX,
		R.cy + RBoxScreenPosY,
		R.w,
		R.h,
		true
	};
	return Box::IsHitBox( tmpL, tmpR );
}
bool Box::IsHitCircle	( const Box &L, const Circle &R, bool ignoreExistFlag )
{
	if ( !ignoreExistFlag && ( !L.exist || !R.exist ) )
	{
		return false;
	}
	// else

	// 処理の内容
	/*
		元の長方形を
		円の半径ぶん大きくし，
		角を丸くした長方形	を作り，
		これと円の中心点との接触を見る
	*/
	// 縦長にした長方形との接触
	{
		Box tmp =
		{
			L.cx, 
			L.cy, 
			L.w, 
			L.h + R.radius, 
			true
		};
		if ( Box::IsHitPoint( tmp, R.cx, R.cy ) )
		{
			return true;
		}
	}
	// 横長にした長方形との接触
	{
		Box tmp =
		{
			L.cx,
			L.cy,
			L.w + R.radius,
			L.h,
			true
		};
		if ( Box::IsHitPoint( tmp, R.cx, R.cy ) )
		{
			return true;
		}
	}
	// 四隅からの距離を見る
	{
		if	(
				Circle::IsHitPoint( R, L.cx - L.w,	L.cy - L.h )	||	// 左上
				Circle::IsHitPoint( R, L.cx - L.w,	L.cy + L.h )	||	// 左下
				Circle::IsHitPoint( R, L.cx + L.w,	L.cy - L.h )	||	// 右上
				Circle::IsHitPoint( R, L.cx + L.w,	L.cy + L.h )		// 右下
			)
		{
			return true;
		}
	}
	return false;
}
bool Box::IsHitCircle	( const Box &L, const float &LBoxScreenPosX, const float &LBoxScreenPosY, const Circle &R, const float &RCircleScreenPosX, const float &RCircleScreenPosY, bool ignoreExistFlag )
{
	if ( ignoreExistFlag && ( !L.exist || !R.exist ) )
	{
		return false;
	}
	// else

	Box tmpL =
	{
		L.cx + LBoxScreenPosX,
		L.cy + LBoxScreenPosY,
		L.w,
		L.h,
		true
	};
	Circle tmpR =
	{
		R.cx + RCircleScreenPosX,
		R.cy + RCircleScreenPosY,
		R.radius,
		true
	};
	return Box::IsHitCircle( tmpL, tmpR );
}

void Circle::Set		( float centerX, float centerY, float rad, bool isExist )
{
	cx		= centerX;
	cy		= centerY;
	radius	= rad;
	exist	= isExist;
}
bool Circle::IsHitPoint	( const Circle &L, const float &RX, const float &RY, bool ignoreExistFlag )
{
	if ( ignoreExistFlag && !L.exist )
	{
		return false;
	}
	// else

	return ( ( ( RX - L.cx ) * ( RX - L.cx ) ) + ( ( RY - L.cy ) * ( RY - L.cy ) ) < ( L.radius * L.radius ) );
}
bool Circle::IsHitPoint	( const Circle &L, const float &LCircleScreenPosX, const float &LCircleScreenPosY, const float &RX, const float &RY, bool ignoreExistFlag )
{
	if ( ignoreExistFlag && !L.exist )
	{
		return false;
	}
	// else

	Circle tmp =
	{
		L.cx + LCircleScreenPosX,
		L.cy + LCircleScreenPosY,
		L.radius,
		true
	};
	return Circle::IsHitPoint( tmp, RX, RY );
}
bool Circle::IsHitCircle( const Circle &L, const Circle &R, bool ignoreExistFlag )
{
	if ( ignoreExistFlag && ( !L.exist || !R.exist ) )
	{
		return false;
	}
	// else

	float dx = R.cx - L.cx;
	float dy = R.cy - L.cy;
	float rad = R.radius + L.radius;

	return ( ( dx * dx ) + ( dy * dy ) < rad * rad );
}
bool Circle::IsHitCircle( const Circle &L, const float &LCircleScreenPosX, const float &LCircleScreenPosY, const Circle &R, const float &RCircleScreenPosX, const float &RCircleScreenPosY, bool ignoreExistFlag )
{
	if ( ignoreExistFlag && ( !L.exist || !R.exist ) )
	{
		return false;
	}
	// else

	Circle tmpL =
	{
		L.cx + LCircleScreenPosX,
		L.cy + LCircleScreenPosY,
		L.radius,
		true
	};
	Circle tmpR =
	{
		R.cx + RCircleScreenPosX,
		R.cy + RCircleScreenPosY,
		R.radius,
		true
	};
	return Circle::IsHitCircle( tmpL, tmpR );
}
bool Circle::IsHitBox	( const Circle &L, const Box &R, bool ignoreExistFlag )
{
	if ( ignoreExistFlag && ( !L.exist || !R.exist ) )
	{
		return false;
	}
	// else

	return Box::IsHitCircle( R, L );
}
bool Circle::IsHitBox	( const Circle &L, const float &LCircleScreenPosX, const float &LCircleScreenPosY, const Box &R, const float &RBoxScreenPosX, const float &RBoxScreenPosY, bool ignoreExistFlag )
{
	if ( ignoreExistFlag && ( !L.exist || !R.exist ) )
	{
		return false;
	}
	// else

	return Box::IsHitCircle( R, RBoxScreenPosX, RBoxScreenPosY, L, LCircleScreenPosX, LCircleScreenPosY );
}
