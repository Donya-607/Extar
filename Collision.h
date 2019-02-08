#ifndef INCLUDED_COLLISION_H_
#define INCLUDED_COLLISION_H_

//--------------------
//
//		Collision.h
//
//--------------------

class Circle;

/// <summary>
/// 当たり判定用<para></para>
/// cx, cy	: 中心点，オブジェクトに設定する場合はオフセット値を指定<para></para>
/// w, h	: 幅・高さの半分を指定<para></para>
/// exist	: 判定の有無に使用
/// </summary>
class Box
{
public:
	float	cx;		// 中心点，オブジェクトに設定する場合はオフセット値を指定
	float	cy;		// 中心点，オブジェクトに設定する場合はオフセット値を指定
	float	w;		// 幅の半分
	float	h;		// 高さの半分
	bool	exist;	// 判定の有無
public:
	Box() : cx( 0 ), cy( 0 ), w( 0 ), h( 0 ), exist( false ) {}
	Box
	(
		float centerX, float centerY,
		float halfWidth, float halfHeight,
		bool  isExist
	) :
		cx( centerX ), cy( centerY ),
		w( halfWidth ), h( halfHeight ),
		exist( isExist ) {}

	void Set				( float centerX, float centerY, float halfWidth, float halfHeight, bool isExist );
public:
	static bool IsHitPoint	( const Box &L, const float &RX, const float &RY, bool ignoreExistFlag = false );
	static bool IsHitPoint	( const Box &L, const float &LBoxScreenPosX, const float &LBoxScreenPosY, const float &RX, const float &RY, bool ignoreExistFlag = false );
	static bool IsHitBox	( const Box &L, const Box &R, bool ignoreExistFlag = false );
	static bool IsHitBox	( const Box &L, const float &LBoxScreenPosX, const float &LBoxScreenPosY, const Box &R, const float &RBoxScreenPosX, const float &RBoxScreenPosY, bool ignoreExistFlag = false );
	static bool IsHitCircle	( const Box &L, const Circle &R, bool ignoreExistFlag = false );
	static bool IsHitCircle	( const Box &L, const float &LBoxScreenPosX, const float &LBoxScreenPosY, const Circle &R, const float &RCircleScreenPosX, const float &RCircleScreenPosY, bool ignoreExistFlag = false );
};


/// <summary>
/// 当たり判定用<para></para>
/// cx, cy	: 中心点，オブジェクトに設定する場合はオフセット値を指定<para></para>
/// radius	: 半径を指定<para></para>
/// exist	: 判定の有無に使用
/// </summary>
class Circle
{
public:
	float	cx;		// 中心点
	float	cy;		// 中心点
	float	radius;	// 半径
	bool	exist;	// 判定の有無
public:
	Circle() : cx( 0 ), cy( 0 ), radius( 0 ), exist( false ) {}
	Circle
	(
		float centerX, float centerY,
		float rad,
		bool  isExist
	) :
		cx( centerX ), cy( centerY ),
		radius( rad ),
		exist( isExist ) {}

	void Set				( float centerX, float centerY, float rad, bool isExist );
public:
	static bool IsHitPoint	( const Circle &L, const float &RX, const float &RY, bool ignoreExistFlag = false );
	static bool IsHitPoint	( const Circle &L, const float &LCircleScreenPosX, const float &LCircleScreenPosY, const float &RX, const float &RY, bool ignoreExistFlag = false );
	static bool IsHitCircle	( const Circle &L, const Circle &R, bool ignoreExistFlag = false );
	static bool IsHitCircle	( const Circle &L, const float &LCircleScreenPosX, const float &LCircleScreenPosY, const Circle &R, const float &RCircleScreenPosX, const float &RCircleScreenPosY, bool ignoreExistFlag = false );
	static bool IsHitBox	( const Circle &L, const Box &R, bool ignoreExistFlag = false );
	static bool IsHitBox	( const Circle &L, const float &LCircleScreenPosX, const float &LCircleScreenPosY, const Box &R, const float &RBoxScreenPosX, const float &RBoxScreenPosY, bool ignoreExistFlag = false );
};


#endif //INCLUDED_COLLISION_H_
