#include "DxLib.h"

#include "Fade.h"

namespace FadeImage
{
	static int hFade;

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( 0 != hFade )
		{
			return;
		}
		// else

		hFade = LoadGraph( "./Data/Images/BG/Fade.png" );
	}
	void Release()
	{
		DeleteGraph( hFade );
		hFade = 0;
	}

	int  GetHandle()
	{
		return hFade;
	}

	const Vector2 SIZE{ 4352.0f, 2300.0f };
	Vector2 GetSize()
	{
		return SIZE;
	}
}

std::unique_ptr<Fade> Fade::instance = nullptr;

void Fade::Init( int moveIntervalFrame, Vector2 posLeftTop )
{
	moveInterval = moveIntervalFrame;
	frame = moveInterval;

	pos = posLeftTop;

	isDoneFade = false;
	isLeave = false;

	isDraw = true;
}
void Fade::Uninit()
{
	isDoneFade	= false;
	isLeave		= false;
	isDraw		= false;
}

void Fade::Update()
{
	if ( !isDraw )
	{
		return;
	}
	// else

	Move();
}

void Fade::Move()
{
	if ( 0 < frame-- )
	{
		return;
	}
	// else

	frame = moveInterval;

	constexpr float MOVE_AMOUNT = 128.0f;
	pos += MOVE_AMOUNT;

	if	(
			SCREEN_WIDTH  <= pos.x + FadeImage::GetSize().x &&
			SCREEN_HEIGHT <= pos.y + FadeImage::GetSize().y
		)
	{
		isDoneFade = true;

		if	(
				SCREEN_WIDTH  <= pos.x &&
				SCREEN_HEIGHT <= pos.y
			)
		{
			isLeave = true;
		}
	}
}

void Fade::Draw() const
{
	if ( !isDraw )
	{
		return;
	}
	// else

	Vector2 halfSize;
	halfSize.x = ( FadeImage::GetSize().x * 0.5f );
	halfSize.y = ( FadeImage::GetSize().y * 0.5f );

	DrawRotaGraph
	(
		scast<int>( pos.x + halfSize.x ),			// 画面上の中心座標
		scast<int>( pos.y + halfSize.y ),			// 画面上の中心座標
		1.0,
		ToRadian( 45.0 ),
		FadeImage::GetHandle(),
		TRUE
	);
}