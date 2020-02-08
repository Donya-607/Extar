#include "Rotator.h"

#include <cfloat>	// FLT_MAX

#include "DxLib.h"

#include "Common.h"	// SCREEN_WIDTH, SCREEN_HEIGHT

Rotator::Rotator( const Vector2 genPos, float halfWidth, float moveSpeed ) :
	pos( genPos ), width( halfWidth ), moveSpeed( moveSpeed )
{}

void Rotator::Update()
{
	Move();
}

void Rotator::DrawHitBox( int R, int G, int B )
{
	const Box body = GetHitBox();
	const unsigned int color = GetColor( R, G, B );

	DrawBoxAA
	(
		body.cx - body.w,
		body.cy - body.h,
		body.cx + body.w,
		body.cy + body.h,
		color, TRUE
	);
}

bool Rotator::ShouldRemove() const
{
	const float halfScreenWidth  = scast<float>( SCREEN_WIDTH  >> 1 );
	const float halfScreenHeight = scast<float>( SCREEN_HEIGHT >> 1 );

	Box screen{};
	screen.cx = halfScreenWidth;
	screen.cy = halfScreenHeight;
	screen.w  = halfScreenWidth;
	screen.h  = halfScreenHeight;
	screen.exist = true;

	return ( Box::IsHitBox( GetHitBox(), screen ) ) ? false : true;
}

Box  Rotator::GetHitBox() const
{
	Box tmp{};
	tmp.cx		= pos.x;
	tmp.cy		= pos.y;
	tmp.w		= width;
	tmp.h		= FLT_MAX;
	tmp.exist	= true;
	return tmp;
}

void Rotator::Move()
{
	pos.x += moveSpeed;
}
