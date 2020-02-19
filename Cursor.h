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

	int  GetHandle( bool isChoice );
	Vector2 GetSize();
}
namespace StageImage
{
	void Load();
	void Release();

	int  GetHandle( int stageNumber );
	int  GetBackHandle( bool isGlow );
	int  GetLRHandle( bool isL, bool isGlow );
}
namespace StageSelect
{
	Vector2 GetPosLeftTop();
	Vector2 GetSize();
	Vector2 GetMargin();	// StageÇ∆StageÇ∆ÇÃä‘ÇÃåÑä‘

	Vector2 GetBackPosCenter();
	Vector2 GetBackSize( bool isGlow );

	int  GetMaxRow();
	int  GetMaxColumn();
	int  GetMaxDisplayNumber();

	void SetBackGlow( bool isGlow );

	void SetLRGlow( bool isL, bool isGlow );

	void Draw( int nowStageNumber );
}

class Cursor
{
private:
	int  nowStageNumber;	// 1énÇ‹ÇË

	int	 glowTimer[2];		// 0:L, 1:R, LRÉ{É^ÉìÇåıÇÁÇπÇÈéûä‘

	Vector2 pos;	// ç∂è„
	Vector2 velo;	// velocity

	bool isDoneMove;
	bool isChooseBack;

	bool isDecision;
#if USE_IMGUI

#endif // USE_IMGUI

public:
	Cursor() : nowStageNumber( 1 ),
		glowTimer(),
		pos(), velo(),
		isDoneMove( false ),
		isChooseBack( false ),
		isDecision( false )
	{}
	~Cursor() {}

	void Init();
	void Uninit();

	void Update( bool isAcceptInput, bool canDecision );

	void Draw( Vector2 shake ) const;
public:
	void SetNowStagenumber( int number )
	{
		nowStageNumber = number;
	}
	int  GetNowStageNumber() const
	{
		return nowStageNumber;
	}
	bool IsChoiceBack() const
	{
		return isChooseBack;
	}
	bool IsDecision() const
	{
		return isDecision;
	}
private:
	void Move();
	void Interpolate();
	void GlowUpdate();

#if USE_IMGUI

#endif // USE_IMGUI
};

#endif //INCLUDED_CURSOR_H_
