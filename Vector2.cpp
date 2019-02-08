#include <math.h>
#include "Common.h"
#include "Vector2.h"

Vector2 &Vector2::operator = ( const Vector2 &R )
{
	x = R.x;
	y = R.y;
	return *this;
}

Vector2 Vector2::operator + ( const Vector2 &R )
{
	return Vector2( x + R.x, y + R.y );
}
Vector2 Vector2::operator - ( const Vector2 &R )
{
	return Vector2( x - R.x, y - R.y );
}

Vector2 &Vector2::operator += ( const Vector2 &R )
{
	x += R.x;
	y += R.y;
	return *this;
}
Vector2 &Vector2::operator -= ( const Vector2 &R )
{
	x -= R.x;
	y -= R.y;
	return *this;
}

Vector2 Vector2::operator + ( const float &R )
{
	return Vector2( x + R, y + R );
}
Vector2 Vector2::operator - ( const float &R )
{
	return Vector2( x - R, y - R );
}
Vector2 Vector2::operator * ( const float &R )
{
	return Vector2( x * R, y * R );
}
Vector2 Vector2::operator / ( const float &R )
{
	return Vector2( x / R, y / R );
}

Vector2 &Vector2::operator += ( const float &R )
{
	x += R;
	y += R;
	return *this;
}
Vector2 &Vector2::operator -= ( const float &R )
{
	x -= R;
	y -= R;
	return *this;
}
Vector2 &Vector2::operator *= ( const float &R )
{
	x *= R;
	y *= R;
	return *this;
}
Vector2 &Vector2::operator /= ( const float &R )
{
	x /= R;
	y /= R;
	return *this;
}

// 前置
Vector2 &Vector2::operator ++()
{
	++x;
	++y;
	return *this;
}
Vector2 &Vector2::operator --()
{
	--x;
	--y;
	return *this;
}
// 後置
Vector2 Vector2::operator ++( int )		// int型でなければならない，「後置'operator++'の余分なパラメーターは'int'型である必要があります」と出る
{
	const Vector2 tmp = *this;
	++( *this );				//上記の前置版を使って実装することで，処理内容の食い違いを避け，保守性を向上させる
	return tmp;					//返却用のインスタンスを生成し，これを返すことになるので，戻り値は実体となる
								//エラーメッセージ：「型 "Vector2 &" の参照を型 "const Vector2" の初期化子にバインドするときに修飾子がドロップされました」
}
Vector2 Vector2::operator --( int )
{
	const Vector2 tmp = *this;
	--( *this );
	return tmp;
}

Vector2 &Vector2::Normalize()
{
	float len = Length();

	// 0 だったら，そのまま返す
	if ( -EPSILON < len && len < EPSILON )
	{
		return *this;
	}

	// 長さの逆数をかける
	*this *= ( 1.0f / len );
	return *this;
}

Vector2 Vector2::Ortho() const
{
	return Vector2( y, -x );
}

void Vector2::Set( float xx, float yy )
{
	x = xx;
	y = yy;
}
void Vector2::Rotate( float deg )
{
	float theta = deg / 180 * PI;
	float c = cosf( theta );
	float s = sinf( theta );

	float tx = ( x * c ) - ( y * s );
	float ty = ( x * s ) + ( y * c );

	x = tx;
	y = ty;
}

void Vector2::Truncate( float length )
{
	float angle = atan2f( y, x );

	x = length * cosf( angle );
	y = length * sinf( angle );
}

float Vector2::Distance( const Vector2 &R ) const
{
	Vector2 d( R.x - x, R.y - y );
	return d.Length();
}
float Vector2::Length() const
{
	return sqrtf( ( x * x ) + ( y * y ) );
}

// -180° ~ 180°を返す
float Vector2::Degree() const
{
	float  deg = ToDegree( atan2f( y, x ) );
	return deg;
}

Vector2 operator + ( const Vector2 &L, const Vector2 &R )
{
	return Vector2( L.x + R.x, L.y + R.y );
}
Vector2 operator + ( const Vector2 &L, const float &R )
{
	return Vector2( L.x + R, L.y + R );
}
Vector2 operator + ( const float &L, const Vector2 &R )
{
	return Vector2( L + R.x, L + R.y );
}
Vector2 operator - ( const Vector2 &L, const Vector2 &R )
{
	return Vector2( L.x - R.x, L.y - R.y );
}
Vector2 operator - ( const Vector2 &L, const float &R )
{
	return Vector2( L.x - R, L.y - R );
}
Vector2 operator - ( const float &L, const Vector2 &R )
{
	return Vector2( L - R.x, L - R.y );
}
Vector2 operator * ( const Vector2 &L, const float &R )
{
	return Vector2( L.x * R, L.y * R );
}
Vector2 operator * ( const float &L, const Vector2 &R )
{
	return Vector2( L * R.x, L * R.y );
}
Vector2 operator / ( const Vector2 &L, const float &R )
{
	return Vector2( L.x / R, L.y / R );
}
Vector2 operator / ( const float &L, const Vector2 &R )
{
	return Vector2( L / R.x, L / R.y );
}
