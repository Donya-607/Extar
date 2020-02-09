#pragma once

#include "Collision.h"
#include "Vector2.h"

/// <summary>
/// 星を回転させる当たり判定を提供する。
/// 自身は仮想的な縦線となり，生成位置から横に動く。実際の当たり判定は行わない。
/// </summary>
class Rotator
{
private:
	float	pos;				// Ｘ座標
	float	width{ 1.0f };		// 半分の幅
	float	moveSpeed{ 1.0f };	// 横方向
public:
	Rotator( float generatePosX, float halfWidth, float moveSpeed );
public:
	void Update();

	void DrawHitBox( int red = 255, int green = 255, int blue = 255 );
public:
	bool ShouldRemove() const;
	Box  GetHitBox() const;
private:
	void Move();
};
