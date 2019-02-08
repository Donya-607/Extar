#ifndef INCLUDED_COLLISION_H_
#define INCLUDED_COLLISION_H_

//--------------------
//
//		Collision.h
//
//--------------------

class Circle;

/// <summary>
/// �����蔻��p<para></para>
/// cx, cy	: ���S�_�C�I�u�W�F�N�g�ɐݒ肷��ꍇ�̓I�t�Z�b�g�l���w��<para></para>
/// w, h	: ���E�����̔������w��<para></para>
/// exist	: ����̗L���Ɏg�p
/// </summary>
class Box
{
public:
	float	cx;		// ���S�_�C�I�u�W�F�N�g�ɐݒ肷��ꍇ�̓I�t�Z�b�g�l���w��
	float	cy;		// ���S�_�C�I�u�W�F�N�g�ɐݒ肷��ꍇ�̓I�t�Z�b�g�l���w��
	float	w;		// ���̔���
	float	h;		// �����̔���
	bool	exist;	// ����̗L��
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
/// �����蔻��p<para></para>
/// cx, cy	: ���S�_�C�I�u�W�F�N�g�ɐݒ肷��ꍇ�̓I�t�Z�b�g�l���w��<para></para>
/// radius	: ���a���w��<para></para>
/// exist	: ����̗L���Ɏg�p
/// </summary>
class Circle
{
public:
	float	cx;		// ���S�_
	float	cy;		// ���S�_
	float	radius;	// ���a
	bool	exist;	// ����̗L��
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
