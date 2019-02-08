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

// �O�u
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
// ��u
Vector2 Vector2::operator ++( int )		// int�^�łȂ���΂Ȃ�Ȃ��C�u��u'operator++'�̗]���ȃp�����[�^�[��'int'�^�ł���K�v������܂��v�Əo��
{
	const Vector2 tmp = *this;
	++( *this );				//��L�̑O�u�ł��g���Ď������邱�ƂŁC�������e�̐H���Ⴂ������C�ێ琫�����コ����
	return tmp;					//�ԋp�p�̃C���X�^���X�𐶐����C�����Ԃ����ƂɂȂ�̂ŁC�߂�l�͎��̂ƂȂ�
								//�G���[���b�Z�[�W�F�u�^ "Vector2 &" �̎Q�Ƃ��^ "const Vector2" �̏������q�Ƀo�C���h����Ƃ��ɏC���q���h���b�v����܂����v
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

	// 0 ��������C���̂܂ܕԂ�
	if ( -EPSILON < len && len < EPSILON )
	{
		return *this;
	}

	// �����̋t����������
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

// -180�� ~ 180����Ԃ�
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
