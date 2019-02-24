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
	constexpr int SIZE = 96;

	void Load();
	void Release();

	int  GetHandle();
}
namespace SelectStage
{
	Vector2 GetPosLeftTop();
	Vector2 GetSize();
	Vector2 GetMargin();	// StageÇ∆StageÇ∆ÇÃä‘ÇÃåÑä‘

	int  GetMaxRow();
	int  CalcMaxColumn();

	void Draw();
}

class Cursor
{
private:
	int  oldStageNumber;	// 1énÇ‹ÇË
	int  nowStageNumber;	// 1énÇ‹ÇË

	Vector2 pos;	// ç∂è„
	Vector2 velo;	// velocity

	bool isDoneMove;
#if USE_IMGUI

#endif // USE_IMGUI

public:
	Cursor() : oldStageNumber( 1 ), nowStageNumber( 1 ),
		pos(), velo(),
		isDoneMove( false )
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
	bool Interpolate();	// äÆóπÇµÇΩÇÁTRUEÇï‘Ç∑

#if USE_IMGUI

#endif // USE_IMGUI
};

#endif //INCLUDED_CURSOR_H_
