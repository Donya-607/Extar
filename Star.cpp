#include "DxLib.h"
#include "Common.h"
#include "Music.h"
#include "Input.h"	// �A���h�D�ɂĎg�p

#include "Star.h"

#include "FileIO.h"
#include "Grid.h"

namespace
{
	constexpr int ROTATE_INTERVAL = 4;
}

namespace StarImage
{
	constexpr int NUM_ROW		= 3;	// X
	constexpr int NUM_COLUMN	= 6;	// Y
	constexpr int NUM_ALL		= NUM_ROW * NUM_COLUMN;

	constexpr int SPD			= 6;

	static int hStars[NUM_ALL];

	void Load()
	{
		// ���łɒl�������Ă�����C�ǂݍ��񂾂��̂Ƃ݂Ȃ��Ĕ�΂�
		if ( 0 != hStars[0] )
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

void Star::Init( int Row, int Column, int Width, int Height, int Level, bool flagSetAnimeOnly )
{
	// �A�j���\���̂́C�Đ��@�\�������p����
	anim.Set
	(
		0, StarImage::NUM_ROW,
		0, StarImage::SPD,
		nullptr
	);

	rotateInterval = ROTATE_INTERVAL;
	angle = ( rand() % 8 ) * 45.0f;

	if ( flagSetAnimeOnly )
	{
		return;
	}
	// else

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
	Rotate();

	anim.Update();

	// ���������蒷���`�̓i�V�ɂȂ����̂ŁC�Ƃ肠�����~�߂Ă����B�g���\�����c���Ă���̂ŁC�폜�܂ł͂��Ȃ�
	assert( width == height );
}

void Star::Draw( Vector2 shake ) const
{
	DrawRotaGraph3
	(
		scast<int>( FRAME_POS_X + ( row		* Grid::GetSize().x ) + ( Grid::GetSize().x * 0.5f ) ),	// ��ʏ�̒��S���W
		scast<int>( FRAME_POS_Y + ( column	* Grid::GetSize().y ) + ( Grid::GetSize().y * 0.5f ) ),	// ��ʏ�̒��S���W
		StarImage::SIZE >> 1,	// �摜��̒��S���W
		StarImage::SIZE >> 1,	// �摜��̒��S���W
		scast<double>( width  ),
		scast<double>( height ),
		scast<double>( ToRadian( angle ) ),
		StarImage::GetHandle( level, anim.index ),
		TRUE
	);
}

void Star::BeExposed()
{
	level -= 1;
}

void Star::Rotate()
{
	if ( 0 < rotateInterval-- )
	{
		return;
	}
	// else

	rotateInterval = ROTATE_INTERVAL;

	// �����`�̂Ƃ��̂݁C45�x����]
	angle += ( width == height ) ? 45.0f : 90.0f;

	if ( 359 < angle )
	{
		angle = 0;
	}
}


void StarMng::Init( int stageNumber )
{
	stars = FileIO::FetchStarsInfo( stageNumber );
	
	for ( Star &it : stars )
	{
		it.Init(NULL, NULL, NULL, NULL, NULL, /* flagSetAnimeOnly = */true );
	}

#if USE_IMGUI

	this->stageNumber	= stageNumber;;
	choiseRow			= 0;
	choiseColumn		= 0;
	width				= 1;
	height				= 1;
	level				= 6;

#endif // USE_IMGUI

}
void StarMng::Uninit()
{
	for ( Star &it : stars )
	{
		it.Uninit();
	}
	stars.clear();
	
	levelStorage.clear();
}

void StarMng::Update()
{
	if ( scast<int>( stars.size() ) < 1 )
	{
		assert( !"Star is not exists." );
		return;
	}
	// else

	for ( Star &it : stars )
	{
		it.Update();
	}

	if ( IS_TRG_UNDO )
	{
		Undo();
	}

#if USE_IMGUI

	ChangeParametersByImGui();

#endif // USE_IMGUI
}
void StarMng::ClearUpdate()
{
	if ( scast<int>( stars.size() ) < 1 )
	{
		assert( !"Star is not exists." );
		return;
	}
	// else

	for ( Star &it : stars )
	{
		it.Update();
	}

#if USE_IMGUI

	ChangeParametersByImGui();

#endif // USE_IMGUI
}

void StarMng::Draw( Vector2 shake ) const
{
	for ( const Star &it : stars )
	{
		it.Draw( shake );
	}

#if USE_IMGUI

	DrawUI();

#endif // USE_IMGUI
}

bool StarMng::CanSaveLog() const
{
	// �Z�[�u�ł��Ȃ��󋵂�����Ǝv���Ă������C�����ł��Ȃ������H
	if ( scast<int>( stars.size() ) == scast<int>( levelStorage.size() ) )
	{
		// return false;
	}
	// else
	return true;
}
void StarMng::SaveLog()
{
	if ( !CanSaveLog() )
	{
		return;
	}
	// else

	std::vector<int> data;
	for ( const Star &it : stars )
	{
		int level = 0;
		it.AcquireData( nullptr, nullptr, nullptr, nullptr, &level );

		assert( 1 <= level && level <= 6 );

		data.push_back( level );
	}

	levelStorage.push_back( data );
}
void StarMng::Undo()
{
	if ( !CanUndo() )
	{
		return;
	}
	// else

	std::vector<int> &storage = levelStorage.back();

	std::vector<int>::iterator it = storage.begin();
	for ( int i = 0 ; it != storage.end(); i++, it++ )
	{
		stars.at( i ).SetData( NULL, NULL, NULL, NULL, *it );
	}

	levelStorage.pop_back();
}

bool StarMng::IsEqualLevels() const
{
	int criterion = 0;	// 1 ~ 6 ������

	for ( const Star &it : stars )
	{
		if ( 0 == criterion )
		{
			it.AcquireData( nullptr, nullptr, nullptr, nullptr, &criterion );

			assert( 1 <= criterion  && criterion <= 6 );

			continue;
		}
		// else

		int level = 0;
		it.AcquireData( nullptr, nullptr, nullptr, nullptr, &level );

		if ( level != criterion )
		{
			assert( 1 <= criterion  && criterion <= 6 );
			return false;
		}
	}

	return true;
}

#if USE_IMGUI

void StarMng::ChangeParametersByImGui()
{
	ImGui::Begin( "Star_Parameters", nullptr, ImGuiWindowFlags_MenuBar );

	if ( ImGui::BeginMenuBar() )
	{
		if ( ImGui::BeginMenu( "File" ) )
		{
			if ( ImGui::MenuItem( "Save" ) )
			{
				FileIO::WriteStars( stageNumber, &stars );

				// �t�@�C���ɕۑ����邾���œK�p���Ȃ����߁C���łɂ܂Ƃ߂ēǂݍ��݂Ȃ���
				FileIO::ReadAllCamera();
				FileIO::ReadAllStars();

				PlaySE( M_E_NEXT );
			}
			if ( ImGui::MenuItem( "Load" ) )
			{
				stars = FileIO::FetchStarsInfo( stageNumber );
				for ( Star &it : stars )
				{
					it.Init( NULL, NULL, NULL, NULL, NULL, /* flagSetAnimeOnly = */true );
				}

				PlaySE( M_E_BACK );
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::SliderInt( "IO_StageNumber", &stageNumber,	1, 30 );

	ImGui::SliderInt( "Choice_X",		&choiseRow,		0, Grid::GetRowMax() - 1 );
	ImGui::SliderInt( "Choice_Y",		&choiseColumn,	0, Grid::GetColumnMax() - 1 );

	// ImGui::SliderInt( "Set_Width",	&width,			1, Grid::GetRowMax() - 1 );
	// ImGui::SliderInt( "Set_Height",	&height,		1, Grid::GetColumnMax() - 1 );
	ImGui::SliderInt( "Set_Size",		&width,			1, Grid::GetRowMax() - 1 );
	height = width;

	ImGui::SliderInt( "Set_Level",		&level,			1, Star::MAX_LEVEL );

	if ( ImGui::Button( "Set_Star" ) )
	{
		SetStar();

		PlaySE( M_E_NEXT );
	}

	if ( ImGui::Button( "Remove_Star_from_ChoosePos" ) )
	{
		RemoveStar();

		PlaySE( M_E_BACK );
	}

	ImGui::End();
}

void StarMng::SetStar()
{
	RemoveStar();

	stars.push_back( Star() );
	stars.back().Init( choiseRow, choiseColumn, width, height, level );
}
void StarMng::RemoveStar()
{
	Box choose =
	{
		scast<float>( FRAME_POS_X + ( choiseRow		* Grid::GetSize().x ) ) + ( Grid::GetSize().x * 0.5f ),
		scast<float>( FRAME_POS_Y + ( choiseColumn	* Grid::GetSize().y ) ) + ( Grid::GetSize().y * 0.5f ),
		Grid::GetSize().x * 0.5f,
		Grid::GetSize().y * 0.5f,
		true
	};

	std::vector<Star>::iterator it = stars.begin();
	for ( ; it != stars.end(); )
	{
		Box star = it->FetchColWorldPos();

		if ( Box::IsHitBox( choose, star ) )
		{
			it = stars.erase( it );
			continue;
		}
		// else

		it++;
	}
}

void StarMng::DrawUI() const
{
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 128 );
	{
		unsigned int red	= GetColor( 200, 32, 32 );
		unsigned int green	= GetColor( 32, 200, 32 );
		unsigned int blue	= GetColor( 32, 32, 200 );

		// �Z�b�g����Ƃ���G�̉摜
		DrawExtendGraph
		(
			scast<int>( FRAME_POS_X + ( choiseRow		* Grid::GetSize().x ) ),
			scast<int>( FRAME_POS_Y + ( choiseColumn	* Grid::GetSize().y ) ),
			scast<int>( FRAME_POS_X + ( choiseRow		* Grid::GetSize().x ) + ( width  * Grid::GetSize().x ) ),
			scast<int>( FRAME_POS_Y + ( choiseColumn	* Grid::GetSize().y ) + ( height * Grid::GetSize().y ) ),
			StarImage::GetHandle( level, 0 ),
			TRUE
		);

	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
}

#endif // USE_IMGUI