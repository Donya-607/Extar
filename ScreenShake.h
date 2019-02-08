#ifndef INCLUDED_SHAKE_H_
#define INCLUDED_SHAKE_H_

#include "Vector2.h"

//--------------------
//
//		Shake.h
//
//--------------------

void ShakeInit();
void ShakeUpdate();
void ShakeUninit();

void ScreenShake( Vector2 shakeAmounts, Vector2 shakeIntervals );
void ScreenShakeX( float shakeWidth, float shakeInterval );
void ScreenShakeY( float shakeHeight, float shakeInterval );

Vector2 GetShakeAmount();
float GetShakeAmountX();
float GetShakeAmountY();

#endif //INCLUDED_SHAKE_H_
