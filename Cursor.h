#ifndef INCLUDED_CURSOR_H_
#define INCLUDED_CURSOR_H_

#include "Common.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

#include "Animation.h"
#include "Collision.h"
#include "Vector2.h"

namespace CursorImage
{
	void Load();
	void Release();

	int  GetHandle();
	Vector2 GetSize();
}
namespace SelectImage
{
	void Load();
	void Release();

	int  GetHandle( int stageNumber );
	int  GetBackHandle();
}
namespace SelectStage
{
	Vector2 GetPosLeftTop();
	Vector2 GetSize();
	Vector2 GetMargin();	// StageÇ∆StageÇ∆ÇÃä‘ÇÃåÑä‘

	Vector2 GetBackPosLeftTop();
	Vector2 GetBackSize();

	int  GetMaxRow();
	int  GetMaxColumn();
	int  GetMaxDisplayNumber();

	void Draw( int nowStageNumber );
}

class Cursor
{
private:
	int  nowStageNumber;	// 1énÇ‹ÇË

	Vector2 pos;	// ç∂è„
	Vector2 velo;	// velocity

	bool isDoneMove;
	bool isChooseBack;
#if USE_IMGUI

#endif // USE_IMGUI

public:
	Cursor() : nowStageNumber( 1 ),
		pos(), velo(),
		isDoneMove( false ),
		isChooseBack( false )
	{}
	~Cursor() {}

	void Init();
	void Uninit();

	void Update();

	void Draw( Vector2 shake ) const;
public:
	int  GetNowStageNumber() const
	{
		return nowStageNumber;
	}
private:
	void Move();
	void Interpolate();

#if USE_IMGUI

#endif // USE_IMGUI
};

#endif //INCLUDED_CURSOR_H_
