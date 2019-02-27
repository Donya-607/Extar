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
	int		row;		// 今いる行, 0始まり
	int		column;		// 今いる列, 0始まり

	int		width;		// 覆うマスのサイズ, 1始まり
	int		height;		// 覆うマスのサイズ, 1始まり

	int		moveAmount;	// 一度に動くマスの数, 1始まり

	Vector2 pos;		// LeftTop Position
	Vector2 velo;		// Velocity
	Vector2 size;		// 全体サイズ

	bool	isExposure;	// 露光した瞬間のみTRUE

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
	bool ClampMatrix();	// 補正したらTRUE

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
