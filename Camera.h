#ifndef INCLUDED_CAMERA_H_
#define INCLUDED_CAMERA_H_

#include <vector>

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

	int  GetHandle( int index );
}

class Camera
{
private:
	int		row;		// ������s, 0�n�܂�
	int		column;		// �������, 0�n�܂�

	int		width;		// �����}�X�̃T�C�Y, 1�n�܂�
	int		height;		// �����}�X�̃T�C�Y, 1�n�܂�

	int		moveAmount;	// ��x�ɓ����}�X�̐�, 1�n�܂�

	int		glowTimer;

	Vector2 pos;		// LeftTop Position
	Vector2 denialShake;
	Vector2 size;		// �S�̃T�C�Y

	std::vector<int> rowStorage;
	std::vector<int> clumStorage;

	bool	isGlow;
	bool	isExposure;	// �I�������u�Ԃ̂�TRUE

	bool	isShake;
public:
	Camera() : row( 0 ), column( 0 ), width( 1 ), height( 1 ),
		moveAmount( 1 ),
		glowTimer( 0 ),
		pos(), denialShake(), size(),
		rowStorage(), clumStorage(),
		isGlow( false ),
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
	void Interpolate();
	void ClampPos();
	void ClampMatrix();

	void Shake();

	void Exposure();
public:
	void SetGlow();
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
public:
	void SaveLog();
	bool Undo();
private:
	bool CanSaveLog() const
	{
		// �Z�[�u�ł��Ȃ��󋵂�����Ǝv���Ă������C�����ł��Ȃ������H
		
		return true;
	}
	bool CanUndo() const
	{
		if	(
			scast<int>( rowStorage.size() ) &&
			scast<int>( clumStorage.size() )
			)
		{
			return true;
		}
		// else
		return false;
	}
#if USE_IMGUI
private:
	void ChangeParametersByImGui();
public:
	void SaveData();
#endif // USE_IMGUI
};

#endif //INCLUDED_CAMERA_H_
