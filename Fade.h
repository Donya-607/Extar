#ifndef INCLUDED_FADE_H_
#define INCLUDED_FADE_H_

#include "Common.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

#include "Vector2.h"

namespace FadeImage
{
	void Load();
	void Release();

	int  GetHandle();

	Vector2 GetSize();	// Whole Size
}

class Fade
{
private:
	int moveInterval;	// Frame
	int frame;

	Vector2 pos;	// ç∂è„

	bool isDoneFade;
	bool isLeave;

	bool isDraw;
#if USE_IMGUI

#endif // USE_IMGUI

public:
	Fade() : moveInterval( 0 ), frame( 0 ),
		pos(),
		isDoneFade( false ), isLeave( false ),
		isDraw( false )
	{}
	~Fade() {}

	void Init( int moveIntervalFrame, Vector2 posLeftTop );
	void Uninit();

	void Update();

	void Draw( Vector2 shake ) const;
public:
	bool IsDoneFade() const
	{
		return isDoneFade;
	}
	bool IsLeave() const
	{
		return isLeave;
	}
private:
	void Move();

#if USE_IMGUI

#endif // USE_IMGUI
};

#endif //INCLUDED_FADE_H_
