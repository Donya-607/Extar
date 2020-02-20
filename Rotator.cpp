#include "Rotator.h"

#include <cfloat>	// FLT_MAX

#include "DxLib.h"

#include "Common.h"	// SCREEN_WIDTH, SCREEN_HEIGHT

Rotator::Rotator( int destroyFrame, float genPosX, float halfWidth, float moveSpeed ) :
	timer( destroyFrame ), pos( genPosX ), width( halfWidth ), moveSpeed( moveSpeed )
{}

void Rotator::Update()
{
	timer--;
	Move();
}

void Rotator::DrawHitBox( int R, int G, int B )
{
	const Box body = GetHitBox();
	const unsigned int color = GetColor( R, G, B );

	auto result = DrawBoxAA
	(
		body.cx - body.w,
		0.0f,
		body.cx + body.w,
		scast<float>( SCREEN_HEIGHT ),
		color, TRUE
	);
	result += 1;
}

bool Rotator::ShouldRemove() const
{
	return ( timer <= 0 ) ? true : false;
}

Box  Rotator::GetHitBox() const
{
	Box tmp{};
	tmp.cx		= pos;
	tmp.cy		= scast<float>( SCREEN_HEIGHT >> 1 );
	tmp.w		= width;
	tmp.h		= FLT_MAX * 0.5f;
	tmp.exist	= true;
	return tmp;
}

void Rotator::Move()
{
	pos += moveSpeed;
}
