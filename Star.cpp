#include "DxLib.h"
#include "Common.h"
#include "Music.h"

#include "Star.h"

namespace StarImage
{
	constexpr int NUM_ROW		= 3;	// X
	constexpr int NUM_COLUMN	= 6;	// Y
	constexpr int NUM_ALL		= NUM_ROW * NUM_COLUMN;

	constexpr int SPD			= 2;

	static int hStars[NUM_ALL];

	void Load()
	{
		// ���łɒl�������Ă�����C�ǂݍ��񂾂��̂Ƃ݂Ȃ��Ĕ�΂�
		if ( 0 != hStars )
		{
			return;
		}
		// else

		LoadDivGraph
		(
			"./Data/Images/Star/Star.png",
			NUM_ALL,
			NUM_ROW, NUM_COLUMN,
			SIZE, SIZE,
			hStars
		);
	}
	void Release()
	{
		for ( int i = 0; i < NUM_ALL; i++ )
		{
			DeleteGraph( hStars[i] );
			hStars[i] = 0;
		}
	}

	int  GetHandle( int level, int animIndex )
	{
		// level �͂P�n�܂�CanimIndex�͂O�n�܂��z��

		assert( level <= Star::MAX_LEVEL );
		assert( animIndex < NUM_ROW );

		return hStars[( ( level - 1 ) * NUM_ROW ) + animIndex];
	}
}

void Star::Init( int Row, int Column, int Width, int Height, int Level )
{
	row		= Row;
	column	= Column;

	width	= Width;
	height	= Height;

	level	= Level;

	// �A�j���\���̂́C�Đ��@�\�������p����
	anim.Set
	(
		0, StarImage::NUM_ROW,
		0, StarImage::SPD,
		nullptr
	);
}
void Star::Uninit()
{

}

void Star::Update()
{
	anim.Update();
}

void Star::Draw( Vector2 shake ) const
{
	DrawGraph
	(
		FRAME_POS_X + ( row		* width  ),
		FRAME_POS_Y + ( column	* height ),
		StarImage::GetHandle( level, anim.index ),
		TRUE
	);
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

	// std::vector<Star>().swap( stars );	// �f�X�g���N�^�ɋL�q
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