#include "DxLib.h"
#include "Music.h"
#include "Common.h"
#include "Input.h"

#include "ScreenShake.h"

#include "SceneGame.h"
#include "SceneTitle.h"

#include "Fade.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

namespace TitleImage
{
	static int hTitleBG;


	static int hTitleUI;
	static int hTitlePushUI;

	static int hHuman;
	static int hshooting_star_1;
	static int hshooting_star_2;
	//星
	constexpr int SIZE = 96;
	constexpr int STAR_NUM_X = 2;
	constexpr int STAR_NUM_Y = 2;
	constexpr int STAR_ALL = STAR_NUM_X * STAR_NUM_Y;
	static int hSTAR[STAR_ALL];

	//エクスタ文字
	constexpr int CHAR_SIZE = 128;
	constexpr int CHAR_NUM_X = 2;
	constexpr int CHAR_NUM_Y = 4;
	constexpr int CHAR_ALL = CHAR_NUM_X * CHAR_NUM_Y;
	static int hChar[CHAR_ALL];

	//タイトルUI
	constexpr int TUI_SIZE_X = 640;
	constexpr int TUI_SIZE_Y = 100;
	static int hTUI;


	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( 0 != hTitleBG )
		{
			return;
		}
		// else

		// hTitleBG = LoadGraph( "./Data/Images/BG/Title.png" );


		hTitlePushUI = LoadGraph( "./Data/Images/Title/PressAButton.png" );
		LoadDivGraph
		(
			"./Data/Images/Title/TitleCharacter.png",
			STAR_ALL,
			STAR_NUM_X,
			STAR_NUM_Y,
			SIZE,
			SIZE,
			hSTAR
		);

		LoadDivGraph
		(
			"./Data/Images/Title/TitleName.png",
			CHAR_ALL,
			CHAR_NUM_X,
			CHAR_NUM_Y,
			CHAR_SIZE,
			CHAR_SIZE,
			hChar
		);

		hHuman = LoadGraph( "./Data/Images/Title/TitleHuman.png" );
		hshooting_star_1 = LoadGraph( "./Data/Images/Title/TitleStar1.png" );
		hshooting_star_2 = LoadGraph( "./Data/Images/Title/TitleStar2.png" );


	}
	void Release()
	{
		DeleteGraph( hTitleBG	);
		hTitleBG	= 0;


		DeleteGraph( hTitleUI );
		DeleteGraph( hTitlePushUI );
		hTitleUI = 0;
		hTitlePushUI = 0;

		DeleteGraph( hHuman );
		DeleteGraph( hshooting_star_1 );
		DeleteGraph( hshooting_star_2 );

		hHuman = 0;
		hshooting_star_1 = 0;
		hshooting_star_2 = 0;


	}
}

void Title::Init()
{
	TitleImage::Load();

	ShakeInit();

	PlayBGM( M_Title_Select );
}

void Title::MainInit()
{

}

void Title::Uninit()
{
	TitleImage::Release();

	ShakeUninit();
}

void Title::MainUninit()
{

}

void Title::Update()
{
#if DEBUG_MODE

	if ( TRG( KEY_INPUT_C ) )
	{
		char debugstoper = 0;
	}

#endif // DEBUG_MODE

	if ( Fade::GetInstance()->IsDoneFade() && nextState == State::GotoGame )
	{
		PrepareChangeSceneToGame();

		delete this;
		return;
	}
	// else

	ShakeUpdate();

	switch ( state )
	{
	case State::Main:
		MainUpdate(); break;
	default:
		assert( !"Error:SceneTitle state error." );
		break;
	}

	FadeCheck();

#if DEBUG_MODE

	if	(
		( TRG( KEY_INPUT_Z ) || TRG( KEY_INPUT_RETURN ) || TRG_J( 0, Input::A ) )
		&& isOpenFade && nextState != State::GotoGame
		)
	{
		nextState = State::GotoGame;
		FadeBegin();

		PlaySE( M_E_NEXT );
	}

#endif // DEBUG_MODE
}

void Title::MainUpdate()
{

	if	( TRG( KEY_INPUT_Z ) && isOpenFade && nextState != State::GotoGame )
	{
		nextState = State::GotoGame;
		FadeBegin();

		PlaySE( M_DECISION );
	}


#if USE_IMGUI

	ImGui::Begin( "title_window" );

	ImGui::SliderFloat( "push_x", &push_pos.x, -1000.0f, 1000.0f );
	ImGui::SliderFloat( "push_y", &push_pos.y, -1000.0f, 1000.0f );

	ImGui::End();

#endif // USE_IMGUI
}

void Title::FadeBegin()
{
	// SceneGameものと同じものを使用している

	constexpr int MOVE_INTERVAL = 1;
	Vector2 pos = FadeImage::GetSize();
	pos *= -1;
	pos.x += scast<float>( SCREEN_WIDTH ) * 0.2f/* 位置の調整 */;
	pos.y -= scast<float>( SCREEN_HEIGHT ) * 1.0f/* 位置の調整 */;

	Fade::GetInstance()->Init( MOVE_INTERVAL, pos );

	isOpenFade = false;
}

void Title::FadeCheck()
{
	if ( nextState != State::Null && Fade::GetInstance()->IsDoneFade() )
	{
		FadeDone();
	}

	if ( Fade::GetInstance()->IsLeave() )
	{
		FadeEnd();
	}
}

void Title::FadeDone()
{
	// シーン遷移チェックは先にしているので，これに引っかかるはずはない想定
	assert( nextState != State::GotoGame );

	switch ( state )
	{
	case State::Main:
		MainUninit();		break;
	default:
		assert( !"Error:SceneGame state error." );
		exit( EXIT_FAILURE );
		return;
	}

	state = nextState;
	nextState = State::Null;

	switch ( state )
	{
	case State::Main:
		MainInit();		break;
	default:
		assert( !"Error:SceneGame state error." );
		exit( EXIT_FAILURE );
		return;
	}

	isOpenFade = false;
}

void Title::FadeEnd()
{
	Fade::GetInstance()->Uninit();
	isOpenFade = true;
}

void Title::PrepareChangeSceneToGame()
{
	Uninit();

	mpMng->mpScene = new Game( mpMng );
	mpMng->mpScene->Init();
}

void Title::Draw()
{
	Vector2 shake = GetShakeAmount();
	
	SetDrawBlendMode( DX_BLENDGRAPHTYPE_NORMAL, 0 );

	// 背景
	{
		/*DrawExtendGraph
		(
			scast<int>( 0 - shake.x ),
			scast<int>( 0 - shake.y ),
			scast<int>( SCREEN_WIDTH - shake.x ),
			scast<int>( SCREEN_HEIGHT - shake.y ),
			TitleImage::hTitleBG,
			TRUE
		);*/
	}

	//後ろの星
	{
		DrawGraph
		(
			scast<int>( star_pos.x + 180.0f ),
			scast<int>( star_pos.y + 180.0f + sin( PI * 2.0f / 240.0f * Count ) * 20.0f ),
			TitleImage::hSTAR[0],
			TRUE
		);
		Count++;
		if ( Count >= 240 )Count = 0;
	}
	if ( flash_state == 2 || flash_state == 3 )
	{
		SetDrawBlendMode( DX_BLENDMODE_ADD, flash_val );
		DrawGraph
		(
			scast<int>( star_pos.x + 180.0f ),
			scast<int>( star_pos.y + 180.0f + sin( PI * 2.0f / 240.0f * Count ) * 20.0f ),
			TitleImage::hSTAR[0],
			TRUE
		);

	}
	SetDrawBlendMode( DX_BLENDGRAPHTYPE_NORMAL, 0 );


	timer++;	// 時間差を作るため

	//Human
	{
		DrawGraph
		(
			0,
			0,
			TitleImage::hHuman,
			TRUE
		);
	}

	//流れ星ではないけど、点滅する星
	{
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, shooting_star_1_val );
		{
			if ( shooting_star_1_val >= 255 ) { shooting_star_1_val_state = 1; }
			if ( shooting_star_1_val <= 50 ) { shooting_star_1_val_state = 0; }
			if ( shooting_star_1_val_state == 1 )
			{
				shooting_star_1_val -= 5;
			}
			if ( shooting_star_1_val_state == 0 )
			{
				shooting_star_1_val += 5;
			}
			DrawGraph
			(
				0,
				0,
				TitleImage::hshooting_star_1,
				TRUE
			);
		}

		SetDrawBlendMode( DX_BLENDMODE_ALPHA, shooting_star_2_val );
		{
			if ( shooting_star_2_val >= 255 ) { shooting_star_2_val_state = 1; }
			if ( shooting_star_2_val <= 50 ) { shooting_star_2_val_state = 0; }
			if ( shooting_star_2_val_state == 1 )
			{
				shooting_star_2_val -= 5;
			}
			if ( shooting_star_2_val_state == 0 )
			{
				shooting_star_2_val += 5;
			}
			DrawGraph
			(
				0,
				0,
				TitleImage::hshooting_star_2,
				TRUE
			);
		}
	}

	SetDrawBlendMode( DX_BLENDGRAPHTYPE_NORMAL, 0 );

	//エ
	{
		DrawGraph
		(
			scast<int>( E_pos.x + 200.0f ),
			scast<int>( E_pos.y + 200.0f + sin( PI * 2.0f / 240.0f * Count ) * 20.0f ),
			TitleImage::hChar[0],
			TRUE
		);
	}
	//ク
	{
		DrawGraph
		(
			scast<int>( Ku_pos.x + 330.0f ),
			scast<int>( Ku_pos.y + 200.0f + sin( PI * 2.0f / 240.0f * Count2 ) * 20.0f ),
			TitleImage::hChar[2],
			TRUE
		);
		if ( timer >= 10 )
		{
			Count2++;
			if ( Count2 >= 240 )Count2 = 0;
		}
	}
	//ス
	{
		DrawGraph
		(
			scast<int>( Su_pos.x + 460.0f ),
			scast<int>( Su_pos.y + 200.0f + sin( PI * 2.0f / 240.0f * Count3 ) * 20.0f ),
			TitleImage::hChar[4],
			TRUE
		);
		if ( timer >= 18 )
		{
			Count3++;
			if ( Count3 >= 240 )Count3 = 0;
		}
	}
	//タ
	{
		DrawGraph
		(
			scast<int>( Ta_pos.x + 590.0f ),
			scast<int>( Ta_pos.y + 200.0f + sin( PI * 2.0f / 240.0f * Count4 ) * 20.0f ),
			TitleImage::hChar[6],
			TRUE
		);
		if ( timer >= 27 )
		{
			Count4++;
			if ( Count4 >= 240 )Count4 = 0;
		}

	}

	//前の星
	{
		DrawRotaGraph3
		(
			scast<int>( star_pos.x + 690.0f ),
			scast<int>( star_pos.y + 310.0f + sin( PI * 2.0f / 240.0f * Count4 ) * 20.0f ),
			TitleImage::SIZE >> 1,
			TitleImage::SIZE >> 1,
			0,
			0,
			scast<double>( ToRadian( angle ) ),
			TitleImage::hSTAR[2],
			TRUE,
			TRUE
		);


	}

	SetDrawBlendMode( DX_BLENDGRAPHTYPE_NORMAL, 0 );

	DrawGraph
	(
		scast<int>( push_pos.x - 180 ),
		scast<int>( push_pos.y ),
		TitleImage::hTitlePushUI,
		TRUE
	);

	switch ( flash_state )
	{
	case 0:
		{
			flash_rand = 60/*360 + rand() % 360*/;
			flash_state++;
		}
		break;
	case 1:
		{
			flash_rand--;
			if ( flash_rand <= 0 )flash_state++;
		}
		break;
	case 2:
		{
			SetDrawBlendMode( DX_BLENDMODE_ADD, flash_val );
			DrawRotaGraph3
			(
				scast<int>( star_pos.x + 690.0f ),
				scast<int>( star_pos.y + 310.0f + sin( PI * 2.0f / 240.0f * Count4 ) * 20 ),
				TitleImage::SIZE >> 1,
				TitleImage::SIZE >> 1,
				flash_scale.x + 0.01f,
				flash_scale.y + 0.01f,
				scast<double>( ToRadian( angle ) ),
				TitleImage::hSTAR[2],
				TRUE,
				TRUE
			);

			DrawRotaGraph3
			(
				scast<int>( star_pos.x + 690.0f ),
				scast<int>( star_pos.y + 310.0f + sin( PI * 2.0f / 240.0f * Count4 ) * 20 ),
				TitleImage::SIZE >> 1,
				TitleImage::SIZE >> 1,
				flash_scale.x + 0.02f,
				flash_scale.y + 0.02f,
				scast<double>( ToRadian( angle ) ),
				TitleImage::hSTAR[2],
				TRUE,
				TRUE
			);
			DrawRotaGraph3
			(
				scast<int>( star_pos.x + 690.0f ),
				scast<int>( star_pos.y + 310.0f + sin( PI * 2.0f / 240.0f * Count4 ) * 20 ),
				TitleImage::SIZE >> 1,
				TitleImage::SIZE >> 1,
				flash_scale.x + 0.03f,
				flash_scale.y + 0.03f,
				scast<double>( ToRadian( angle ) ),
				TitleImage::hSTAR[2],
				TRUE,
				TRUE
			);

			flash_scale.x += scale_up_speed.x;
			flash_scale.y += scale_up_speed.y;
			scale_up_speed -= 0.015f;

			if ( scale_up_speed.x <= 0.0f || scale_up_speed.y <= 0.0f )
			{
				scale_up_speed.x = 0.0f;

				scale_up_speed.y = 0.0f;
			}
			if ( flash_scale.x >= 4.0f || flash_scale.y >= 4.0f )
			{
				flash_scale.x = 4.0f;
				flash_scale.y = 4.0f;
			}

			angle += angle_speed;
			if ( angle_speed != 5 )
			{
				angle_speed -= 0.05f;
			}

			flash_val += val_speed;
			if ( val_speed <= 5 )
			{
				val_speed -= 10;
			}
			if ( flash_val >= 255 )flash_state++;
		}
		break;
	case 3:
		{
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, alpha_val );
			DrawRotaGraph3
			(
				scast<int>( star_pos.x + 690.0f ),
				scast<int>( star_pos.y + 310.0f + sin( PI * 2.0f / 240.0f * Count4 ) * 20.0f ),
				TitleImage::SIZE >> 1,
				TitleImage::SIZE >> 1,
				flash_scale.x + 0.01f,
				flash_scale.y + 0.01f,
				scast<double>( ToRadian( angle ) ),
				TitleImage::hSTAR[2],
				TRUE,
				TRUE
			);

			SetDrawBlendMode( DX_BLENDMODE_ADD, flash_val );
			DrawRotaGraph3
			(
				scast<int>( star_pos.x + 690.0f ),
				scast<int>( star_pos.y + 310.0f + sin( PI * 2.0f / 240.0f * Count4 ) * 20.0f ),
				TitleImage::SIZE >> 1,
				TitleImage::SIZE >> 1,
				flash_scale.x + 0.02f,
				flash_scale.y + 0.02f,
				scast<double>( ToRadian( angle ) ),
				TitleImage::hSTAR[2],
				TRUE,
				TRUE
			);
			DrawRotaGraph3
			(
				scast<int>( star_pos.x + 690.0f ),
				scast<int>( star_pos.y + 310.0f + sin( PI * 2.0f / 240.0f * Count4 ) * 20 ),
				TitleImage::SIZE >> 1,
				TitleImage::SIZE >> 1,
				flash_scale.x + 0.03f,
				flash_scale.y + 0.03f,
				scast<double>( ToRadian( angle ) ),
				TitleImage::hSTAR[2],
				TRUE,
				TRUE
			);
			angle += angle_speed;
			if ( angle_speed != 5 )
			{
				angle_speed -= 0.05f;
			}

			alpha_val -= 5;
			flash_val -= 2;

			flash_scale.x -= 0.02f;
			if ( flash_scale.x <= 0.0f )flash_scale.x = 0.0f;
			flash_scale.y -= 0.02f;
			if ( flash_scale.y <= 0.0f )flash_scale.y = 0.0f;

			if ( flash_val <= 0 )
			{
				scale_up_speed.x = 0.3f;
				scale_up_speed.y = 0.3f;
				flash_scale.x = 0.0f;
				flash_scale.x = 0.0f;
				flash_val = 0;
				alpha_val = 255;
				flash_state = 0;
				angle = 0;
				angle_speed = 100;
			}
		}
		break;
	}

	SetDrawBlendMode( DX_BLENDGRAPHTYPE_NORMAL, 255 );


}