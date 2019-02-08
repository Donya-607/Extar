#include "Common.h"
#include "ScreenShake.h"

static Vector2 shakeAmount;
static Vector2 interval;		// ‰æ–Ê—h‚ê‚Ì–§“x
static Vector2 decrease;		// –ˆ“x, —h‚ê•‚ğŒ¸‚ç‚µ‚Ä‚¢‚­—Ê
static int counterX	= 0;
static int counterY	= 0;

void ShakeInit()
{
	shakeAmount.Set( 0, 0 );
	interval.Set( 0, 0 );
	decrease.Set( 0, 0 );
	counterX = 0;
	counterY = 0;
}

void ShakeUpdate()
{
	// X
	if ( shakeAmount.x < -EPSILON || EPSILON < shakeAmount.x )
	{
		if ( interval.x < ++counterX )
		{
			counterX = 0;

			shakeAmount.x *= -1;
			shakeAmount.x += ( 0 < shakeAmount.x ) ? -( decrease.x ) : decrease.x;	//0‚É‹ß‚Ã‚¯‚é
		}
	}
	// Y
	if ( shakeAmount.y < -EPSILON || EPSILON < shakeAmount.y )
	{
		if ( interval.y < ++counterY )
		{
			counterY = 0;

			shakeAmount.y *= -1;
			shakeAmount.y += ( 0 < shakeAmount.y ) ? -( decrease.y ) : decrease.y;	//0‚É‹ß‚Ã‚¯‚é
		}
	}
}

void ShakeUninit()
{
	ShakeInit();
}

void ScreenShake( Vector2 shakeAmounts, Vector2 shakeIntervals )
{
	shakeAmount.x	= shakeAmounts.x;
	shakeAmount.y	= shakeAmounts.y;
	interval.x		= shakeIntervals.x;
	interval.y		= shakeIntervals.y;
	decrease.x		= interval.x * 0.5f;
	decrease.y		= interval.y * 0.5f;
	counterX		= 0;
	counterY		= 0;
}
void ScreenShakeX( float shakeWidth, float shakeInterval )
{
	shakeAmount.x	= shakeWidth;
	interval.x		= shakeInterval;
	decrease.x		= interval.x * 0.5f;
	counterX		= 0;
}
void ScreenShakeY( float shakeHeight, float shakeInterval )
{
	shakeAmount.y	= shakeHeight;
	interval.y		= shakeInterval;
	decrease.y		= interval.y * 0.5f;
	counterY		= 0;
}

Vector2 GetShakeAmount()
{
	return shakeAmount;
}
float GetShakeAmountX()
{
	return shakeAmount.x;
}
float GetShakeAmountY()
{
	return shakeAmount.y;
}