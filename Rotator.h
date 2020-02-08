#pragma once

#include "Collision.h"
#include "Vector2.h"

/// <summary>
/// ������]�����铖���蔻���񋟂���B
/// ���g�͉��z�I�ȏc���ƂȂ�C�����ʒu���牡�ɓ����B���ۂ̓����蔻��͍s��Ȃ��B
/// </summary>
class Rotator
{
private:
	float	pos;				// �w���W
	float	width{ 1.0f };		// �����̕�
	float	moveSpeed{ 1.0f };	// ������
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
