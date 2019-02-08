#ifndef INCLUDED_VECTOR2_H_
#define INCLUDED_VECTOR2_H_

//--------------------
//
//		Vector2.h
//
//--------------------

/// <summary>
/// float x, y
/// </summary>
class Vector2
{
public:
	float x, y;
public:
	Vector2() : x( 0 ), y( 0 ) {}
	Vector2( float xx, float yy ) : x( xx ), y( yy ) {}
	Vector2( const Vector2 &v ) : x( v.x ), y( v.y ) {}

	Vector2 &operator = ( const Vector2 &R );

	Vector2 operator + ( const Vector2 &R );
	Vector2 operator - ( const Vector2 &R );

	Vector2 &operator += ( const Vector2 &R );
	Vector2 &operator -= ( const Vector2 &R );

	Vector2 operator + ( const float &R );
	Vector2 operator - ( const float &R );
	Vector2 operator * ( const float &R );
	Vector2 operator / ( const float &R );

	Vector2 &operator += ( const float &R );
	Vector2 &operator -= ( const float &R );
	Vector2 &operator *= ( const float &R );
	Vector2 &operator /= ( const float &R );

	// �O�u
	Vector2 &operator ++();
	Vector2 &operator --();
	// ��u
	Vector2 operator ++( int );
	Vector2 operator --( int );

	Vector2 &Normalize();

	Vector2 Ortho() const;

	void Set( float xx, float yy );
	void Rotate( float deg );

	void Truncate( float length );

	float Distance( const Vector2 &R ) const;
	float Length() const;

	// -180�� ~ 180����Ԃ�
	float Degree() const;
public:
	// ����
	static float Dot( Vector2 L, Vector2 R )
	{
		return ( L.x * R.x ) + ( L.y * R.y );
	}
	// �O��
	static float Cross( Vector2 L, Vector2 R )
	{
		return ( L.x * R.y ) - ( L.y * R.x );
	}
};

Vector2 operator + ( const Vector2	&L, const Vector2	&R );
Vector2 operator + ( const Vector2	&L, const float		&R );
Vector2 operator + ( const float	&L, const Vector2	&R );

Vector2 operator - ( const Vector2	&L, const Vector2	&R );
Vector2 operator - ( const Vector2	&L, const float		&R );
Vector2 operator - ( const float	&L, const Vector2	&R );

//Vector2 operator * ( const Vector2	&L, const Vector2	&R );
Vector2 operator * ( const Vector2	&L, const float		&R );
Vector2 operator * ( const float	&L, const Vector2	&R );

//Vector2 operator / ( const Vector2	&L, const Vector2	&R );
Vector2 operator / ( const Vector2	&L, const float		&R );
Vector2 operator / ( const float	&L, const Vector2	&R );

#endif //INCLUDED_VECTOR2_H_