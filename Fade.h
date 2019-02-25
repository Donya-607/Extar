#ifndef INCLUDED_FADE_H_
#define INCLUDED_FADE_H_

#include <memory>

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
	static std::unique_ptr<Fade> instance;
private:
	int moveInterval;	// Frame
	int frame;

	Vector2 pos;	// ç∂è„

	bool isDoneFade;
	bool isLeave;

	bool isDraw;
#if USE_IMGUI

#endif // USE_IMGUI

private:
	Fade() : moveInterval( 0 ), frame( 0 ),
		pos(),
		isDoneFade( false ), isLeave( false ),
		isDraw( false )
	{}
public:
	~Fade() {}

	static Fade *GetInstance()
	{
		if ( !instance )
		{
			instance.reset( new Fade );
		}

		return instance.get();
	}
public:
	void Init( int moveIntervalFrame, Vector2 posLeftTop );
	void Uninit();

	void Update();

	void Draw() const;
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
