#ifndef INCLUDED_CAMERA_H_
#define INCLUDED_CAMERA_H_

#include "Common.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

#include "Collision.h"
#include "Vector2.h"

namespace CameraImage
{
	void Load();
	void Release();

	int  GetHandle();
}

class Camera
{
private:
	int		row;		// ������s, 0�n�܂�
	int		column;		// �������, 0�n�܂�

	int		width;		// �����}�X�̃T�C�Y, 1�n�܂�
	int		height;		// �����}�X�̃T�C�Y, 1�n�܂�

	int		moveAmount;	// ��x�ɓ����}�X�̐�, 1�n�܂�

	Vector2 pos;		// LeftTop Position
	Vector2 velo;		// Velocity
	Vector2 size;		// �S�̃T�C�Y

	bool	isExposure;	// �I�������u�Ԃ̂�TRUE

	bool	isShake;
public:
	Camera() : row( 0 ), column( 0 ), width( 1 ), height( 1 ),
		moveAmount( 1 ),
		pos(), velo(), size(),
		isExposure( false ),
		isShake( false )
	{}
	~Camera() {}

	void Init( int stageNumber );
	void Uninit();

	void Update();

	bool IsExposure() const
	{
		return isExposure;
	}

	void Draw( Vector2 shake ) const;
private:
	void Move();
	void ClampPos();
	bool ClampMatrix();	// �␳������TRUE

	void Shake();

	void Exposure();
public:
	void SetShake();

	Box  FetchColWorldPos() const;

	void AcquireData( int *Width, int *Height, int *MoveAmount ) const
	{
		if( nullptr != Width		) { *Width		= width;		}
		if( nullptr != Height		) { *Height		= height;		}
		if( nullptr != MoveAmount	) { *MoveAmount	= moveAmount;	}
	}
	void SetData( int Width, int Height, int MoveAmount )
	{
		width		= Width;
		height		= Height;
		moveAmount	= MoveAmount;
	}

#if USE_IMGUI
private:
	void ChangeParametersByImGui();
public:
	void SaveData();
#endif // USE_IMGUI
};

#endif //INCLUDED_CAMERA_H_
