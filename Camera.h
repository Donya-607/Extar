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
	int		row;		// 今いる行, 0始まり
	int		column;		// 今いる列, 0始まり

	int		width;		// 覆うマスのサイズ, 1始まり
	int		height;		// 覆うマスのサイズ, 1始まり

	int		moveAmount;	// 一度に動くマスの数, 1始まり

	int		glowTimer;

	Vector2 pos;		// LeftTop Position
	Vector2 denialShake;
	Vector2 size;		// 全体サイズ

	std::vector<int> rowStorage;
	std::vector<int> clumStorage;

	bool	isGlow;
	bool	isExposure;	// 露光した瞬間のみTRUE

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
		// セーブできない状況があると思っていたが，そうでもなさそう？
		
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
