#include <algorithm>	// std::max()

#include "DxLib.h"
#include "Common.h"
#include "Music.h"
#include "Input.h"		// アンドゥにて使用

#include "Star.h"

#include "Easing.h"
#include "FileIO.h"
#include "Grid.h"

#undef max
#undef min

namespace StarImage
{
	constexpr int NUM_ROW		= 2;	// X
	constexpr int NUM_COLUMN	= 6;	// Y
	constexpr int NUM_ALL		= NUM_ROW * NUM_COLUMN;

	constexpr int SPD			= 6;

	static int hStars[NUM_ALL];
	//static int hGrade;

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

		//hGrade = LoadGraph( "./Data/Images/UI/StarGrade.png" );
	}
	void Release()
	{
		for ( int i = 0; i < NUM_ALL; i++ )
		{
			DeleteGraph( hStars[i] );
			hStars[i] = 0;
		}

		/*DeleteGraph( hGrade );
		hGrade = 0;*/
	}

	int  GetHandle( int level, int animIndex )
	{
		// level は１始まり，animIndexは０始まりを想定

		assert( level <= Star::MAX_LEVEL );
		assert( animIndex < NUM_ROW );

		return hStars[( ( level - 1 ) * NUM_ROW ) + animIndex];
	}
	int  GetGradeHandle()
	{
		return NULL;
	}
}

void Star::Init( int Row, int Column, int Width, int Height, int Level, bool flagSetAnimeOnly )
{
	if ( flagSetAnimeOnly )
	{
		// アニメ構造体の，再生機能だけ利用する
		anim.Set
		(
			0, StarImage::NUM_ROW,
			0, StarImage::SPD,
			nullptr
		);

		CalcRotate();

		return;
	}
	// else

	row		= Row;
	column	= Column;

	width	= Width;
	height	= Height;

	level	= Level;

	// アニメ構造体の，再生機能だけ利用する
	anim.Set
	(
		0, StarImage::NUM_ROW,
		0, StarImage::SPD,
		nullptr
	);

	CalcRotate();
}
void Star::Uninit()
{

}

void Star::Update()
{
	anim.Update();

	RotateUpdate();

	// さしあたり長方形はナシになったので，とりあえず止めておく。使う可能性も残っているので，削除まではしない
	assert( width == height );
}

void Star::EffectRelatedUpdate()
{
	if ( val >= 255 ) { val_state = 1; }
	if ( val <= 100 ) { val_state = 0; }
	if ( val_state == 0 )
	{

		if ( level == 1 )val += 5;
		if ( level == 2 )val += 5;
		if ( level == 3 )val += 4;
		if ( level == 4 )val += 3;
		if ( level == 5 )val += 2;
		if ( level == 6 )val += 1;
	}
	if ( val_state == 1 )
	{

		if ( level == 1 )val -= 5;
		if ( level == 2 )val -= 5;
		if ( level == 3 )val -= 4;
		if ( level == 4 )val -= 3;
		if ( level == 5 )val -= 2;
		if ( level == 6 )val -= 1;
	}
}

void Star::CalcRotate()
{
	angle = ( level % 2 ) ? 0 : 45.0f;
}

void Star::Draw( Vector2 shake ) const
{
	Vector2 halfSize;
	halfSize.x = ( ( width  * Grid::GetSize().x ) * 0.5f );
	halfSize.y = ( ( height * Grid::GetSize().y ) * 0.5f );


	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );

	DrawRotaGraph3
	(
		scast<int>( FRAME_POS_X + ( row		* Grid::GetSize().x ) + halfSize.x ),	// 画面上の中心座標
		scast<int>( FRAME_POS_Y + ( column	* Grid::GetSize().y ) + halfSize.y ),	// 画面上の中心座標
		StarImage::SIZE >> 1,	// 画像上の中心座標
		StarImage::SIZE >> 1,	// 画像上の中心座標
		scast<double>( width ),
		scast<double>( height ),
		scast<double>( ToRadian( angle ) + radian ),
		StarImage::GetHandle( level, anim.index ),
		TRUE
	);

	SetDrawBlendMode( DX_BLENDMODE_ADD, val );
	DrawRotaGraph3
	(
		scast<int>( FRAME_POS_X + ( row		* Grid::GetSize().x ) + halfSize.x ),	// 画面上の中心座標
		scast<int>( FRAME_POS_Y + ( column	* Grid::GetSize().y ) + halfSize.y ),	// 画面上の中心座標
		StarImage::SIZE >> 1,	// 画像上の中心座標
		StarImage::SIZE >> 1,	// 画像上の中心座標
		scast<double>( width ),
		scast<double>( height ),
		scast<double>( ToRadian( angle ) + radian ),
		StarImage::GetHandle( level, anim.index ),
		TRUE
	);


}

void Star::BeExposed()
{
	level -= 1;

	CalcRotate();
}

#define USE_IMGUI_FOR_ROTATE ( true && DEBUG_MODE && USE_IMGUI )
#if USE_IMGUI_FOR_ROTATE
namespace ROTATE
{
	static int		easeKind	= 3;
	static int		easeType	= 2;	// In or Out or InOut
	static int		easeFrame	= 60;	// イージングにかける全体時間
	static float	lastDegree	= 720.0f;
}
#endif // USE_IMGUI_FOR_ROTATE
void Star::Rotate()
{
	if ( nowRotate ) { return; }
	// else

	radian		= 0.0f;
	easeFactor	= 0.0f;
	nowRotate	= true;
}
void Star::RotateUpdate()
{
#if USE_IMGUI_FOR_ROTATE
	if ( FileIO::IsShowImGuiWindow() )
	{
		ImGui::Begin( "Star's Rotate" );

		ImGui::SliderInt( "Easing Kind",	&ROTATE::easeKind,	0, Donya::Easing::GetKindCount() - 1 );
		ImGui::SliderInt( "Easing Type",	&ROTATE::easeType,	0, Donya::Easing::GetTypeCount() - 1 );
		const std::string desc =
		std::string{ "Ease:" } + Donya::Easing::KindName( ROTATE::easeKind ) +
		std::string{ "Type:" } + Donya::Easing::TypeName( ROTATE::easeType );
		ImGui::Text( desc.c_str() );

		ImGui::DragInt  ( "Taking Frame",	&ROTATE::easeFrame  );
		ImGui::DragFloat( "Last Degree",	&ROTATE::lastDegree );
		ImGui::Text( "End\n" );

		ImGui::End();
	}
#endif // USE_IMGUI_FOR_ROTATE

	if ( !nowRotate ) { return; }
	// else

	Donya::Easing::Kind kind{};
	Donya::Easing::Type type{};
	float takeFrame{};
	float lastRadian{};
#if USE_IMGUI_FOR_ROTATE
	kind			= scast<Donya::Easing::Kind>( ROTATE::easeKind );
	type			= scast<Donya::Easing::Type>( ROTATE::easeType );
	takeFrame		= scast<float>( ROTATE::easeFrame );
	lastRadian		= ToRadian( ROTATE::lastDegree );
#else
	// ImGuiを使って動的に調整した値である
	kind			= Donya::Easing::Kind::Circular;
	type			= Donya::Easing::Type::InOut;
	takeFrame		= 60.0f;
	lastRadian		= ToRadian( 720.0f );
#endif // USE_IMGUI_FOR_ROTATE

	const float onceIncrement = 1.0f / takeFrame;

	easeFactor += onceIncrement;
	if ( 1.0f  <= easeFactor )
	{
		radian		= 0.0f;
		easeFactor	= 0.0f;
		nowRotate	= false;
		return;
	}
	// else

	float ease = Donya::Easing::Ease( kind, type, easeFactor );
	radian = lastRadian * ease;
}
#undef USE_IMGUI_FOR_ROTATE

void StarMng::Init( int stageNumber )
{
	stars = FileIO::FetchStarsInfo( stageNumber );
	
	for ( Star &it : stars )
	{
		it.Init(NULL, NULL, NULL, NULL, NULL, /* flagSetAnimeOnly = */true );
	}

#if USE_IMGUI

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
		// assert( !"Star is not exists." );
		return;
	}
	// else

	for ( Star &it : stars )
	{
		it.Update();
		it.EffectRelatedUpdate();
	}

#if USE_IMGUI

	ChangeParametersByImGui();

#endif // USE_IMGUI
}

void StarMng::ClearUpdate()
{
	if ( scast<int>( stars.size() ) < 1 )
	{
		return;
	}
	// else

	for ( Star &it : stars )
	{
		it.Update();
	}
}

void StarMng::Draw( Vector2 shake ) const
{
	// SetDrawBlendMode( DX_BLENDMODE_ADD, 各Starが変更している );

	for ( const Star &it : stars )
	{
		it.Draw( shake );
	}

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );

#if USE_IMGUI

	DrawUI();

#endif // USE_IMGUI
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
bool StarMng::Undo()
{
	if ( !CanUndo() )
	{
		return false;
	}
	// else

	std::vector<int> &storage = levelStorage.back();

	std::vector<int>::iterator it = storage.begin();
	for ( int i = 0 ; it != storage.end(); i++, it++ )
	{
		stars.at( i ).SetData( NULL, NULL, NULL, NULL, *it );
		stars.at( i ).CalcRotate();
	}

	levelStorage.pop_back();

	return true;
}

bool StarMng::IsEqualLevels() const
{
	int criterion = 0;	// 1 ~ 6 が正常

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
	if ( !FileIO::IsShowImGuiWindow() )
	{
		return;
	}
	// else

	ImGui::Begin( "Star_Parameters" );

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

	ImGui::Text( "" ); // Space

	if ( FileIO::GetNowStageNumber() <= FileIO::GetMaxStageNumber() )
	{
		if ( ImGui::Button( "Save" ) )
		{
			SaveData();

			// ファイルに保存するだけで適用しないため，ついでにまとめて読み込みなおす
			FileIO::ReadAllCamera();
			FileIO::ReadAllStars();
			FileIO::ReadAllNumMoves();

			PlaySE( M_E_NEXT );
		}
		if ( ImGui::Button( "Load" ) )
		{
			stars = FileIO::FetchStarsInfo( FileIO::GetNowStageNumber() );
			for ( Star &it : stars )
			{
				it.Init( NULL, NULL, NULL, NULL, NULL, /* flagSetAnimeOnly = */true );
			}

			PlaySE( M_E_BACK );
		}
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
	if ( !FileIO::IsShowImGuiWindow() )
	{
		return;
	}
	// else

	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 128 );
	{
		unsigned int red	= GetColor( 200, 32, 32 );
		unsigned int green	= GetColor( 32, 200, 32 );
		unsigned int blue	= GetColor( 32, 32, 200 );

		// セットせんとする星の画像
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

void StarMng::SaveData()
{
	FileIO::WriteStars( FileIO::GetNowStageNumber(), &stars );
}

#endif // USE_IMGUI