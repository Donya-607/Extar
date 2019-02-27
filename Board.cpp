#include "DxLib.h"
#include "Common.h"
#include "Music.h"

#include "Board.h"

namespace BoardImage
{
	static int hBoard;

	void Load()
	{
		// Ç∑Ç≈Ç…ílÇ™ì¸Ç¡ÇƒÇ¢ÇΩÇÁÅCì«Ç›çûÇÒÇæÇ‡ÇÃÇ∆Ç›Ç»ÇµÇƒîÚÇŒÇ∑
		if ( 0 != hBoard )
		{
			return;
		}
		// else

		hBoard		= LoadGraph( "./Data/Images/Result/Board.png" );
	}
	void Release()
	{
		DeleteGraph( hBoard );
		hBoard = 0;
	}

	int  GetBoardHandle()
	{
		return hBoard;
	}
}

void Picture::Init()
{

}

void Picture::Uninit()
{

}

void Picture::Update()
{
	pos = basePos;
}

void Picture::Draw( int handle, Vector2 shake ) const
{
	DrawRotaGraph
	(
		scast<int>( pos.x - shake.x ),
		scast<int>( pos.y - shake.y ),
		0.5,
		ToRadian( angle ),
		handle,
		TRUE
	);
}

void Board::Init( Vector2 centerPos )
{
	pos.x = centerPos.x + ( BoardImage::SIZE_X >> 1 );
	pos.y = centerPos.y + ( BoardImage::SIZE_Y >> 1 );

	const Vector2 INIT_VELO{ 0, 9.0f };
	velo = INIT_VELO;

	pPic.reset( new Picture() );
	pPic->Init();
	pPic->SetPos( pos );
}

void Board::Uninit()
{
	pPic.reset();
}

void Board::Update()
{
	Move();

	if ( pPic )
	{
		pPic->SetPos( pos );
		pPic->Update();
	}
}

void Board::Draw( int hPicture, Vector2 shake ) const
{
	DrawRotaGraph
	(
		scast<int>( pos.x - shake.x ),
		scast<int>( pos.y - shake.y ),
		1.0,
		ToRadian( angle ),
		BoardImage::GetBoardHandle(),
		TRUE
	);

	if ( pPic )
	{
		pPic->Draw( hPicture, shake );
	}
}

void Board::Move()
{
	constexpr float GRAVITY = 1.8f;
	velo.y += GRAVITY;

	pos += velo;

	constexpr float DESTINATION_Y = 352.0f;
	if ( DESTINATION_Y < pos.y )
	{
		pos.y  = DESTINATION_Y;
		velo.y = 0;
	}
}