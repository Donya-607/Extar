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
	Vector2 GetMargin();	// Stage��Stage�Ƃ̊Ԃ̌���

	int  GetMaxRow();
	int  CalcMaxColumn();

	void Draw();
}

class Cursor
{
private:
	int  oldStageNumber;	// 1�n�܂�
	int  nowStageNumber;	// 1�n�܂�

	Vector2 pos;	// ����
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
	bool Interpolate();	// ����������TRUE��Ԃ�

#if USE_IMGUI

#endif // USE_IMGUI
};

#endif //INCLUDED_CURSOR_H_
