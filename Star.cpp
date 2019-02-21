#include "DxLib.h"
#include "Common.h"
#include "Music.h"

#include "Star.h"

void Star::Init( int Row, int Column, int Width, int Height, int Level )
{
	row		= Row;
	column	= Column;

	width	= Width;
	height	= Height;

	level	= Level;
}
void Star::Uninit()
{

}

void Star::Update()
{

}

void Star::Draw( Vector2 shake ) const
{

}



void StarMng::Init( int stageNumber )
{

}
void StarMng::Uninit()
{
	for ( Star &it : stars )
	{
		it.Uninit();
	}

	// std::vector<Star>().swap( stars );	// デストラクタに記述
}

void StarMng::Update()
{
	for ( Star &it : stars )
	{
		it.Update();
	}
}

void StarMng::Draw( Vector2 shake ) const
{
	for ( const Star &it : stars )
	{
		it.Draw( shake );
	}
}