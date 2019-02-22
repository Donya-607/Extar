#include "DxLib.h"
#include "Common.h"
#include "Music.h"

#include "Star.h"

#include "FileIO.h"
#include "Grid.h"

namespace StarImage
{
	constexpr int NUM_ROW		= 3;	// X
	constexpr int NUM_COLUMN	= 6;	// Y
	constexpr int NUM_ALL		= NUM_ROW * NUM_COLUMN;

	constexpr int SPD			= 4;

	static int hStars[NUM_ALL];

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
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
		// level は１始まり，animIndexは０始まりを想定

		assert( level <= Star::MAX_LEVEL );
		assert( animIndex < NUM_ROW );

		return hStars[( ( level - 1 ) * NUM_ROW ) + animIndex];
	}
}

void Star::Init( int Row, int Column, int Width, int Height, int Level, bool flagSetAnimeOnly )
{
	// アニメ構造体の，再生機能だけ利用する
	anim.Set
	(
		0, StarImage::NUM_ROW,
		0, StarImage::SPD,
		nullptr
	);

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
	anim.Update();

	// さしあたり長方形はナシになったので，とりあえず止めておく。使う可能性も残っているので，削除まではしない
	assert( width == height );
}

void Star::Draw( Vector2 shake ) const
{
	DrawExtendGraph
	(
		scast<int>( FRAME_POS_X + ( row		* Grid::GetSize().x ) ),
		scast<int>( FRAME_POS_Y + ( column	* Grid::GetSize().y ) ),
		scast<int>( FRAME_POS_X + ( row		* Grid::GetSize().x ) + ( width  * Grid::GetSize().x ) ),
		scast<int>( FRAME_POS_Y + ( column	* Grid::GetSize().y ) + ( height * Grid::GetSize().y ) ),
		StarImage::GetHandle( level, anim.index ),
		TRUE
	);
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
	level				= 1;

#endif // USE_IMGUI

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

				// ファイルに保存するだけで適用しないため，ついでにまとめて読み込みなおす
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

		// セットせんとする敵の画像
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