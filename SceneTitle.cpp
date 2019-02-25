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

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( 0 != hTitleBG )
		{
			return;
		}
		// else

		hTitleBG = LoadGraph( "./Data/Images/BG/Title.png" );
	}
	void Release()
	{
		DeleteGraph( hTitleBG	);
		hTitleBG	= 0;
	}
}

void Title::Init()
{
	TitleImage::Load();

	ShakeInit();

	PlayBGM( M_Title );
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

	if ( TRG( KEY_INPUT_C ) || TRG_J_X( XB_LEFT ) )
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

	if ( TRG( KEY_INPUT_RETURN ) && state != State::GotoGame )
	{
		nextState = State::GotoGame;
		FadeBegin();

		PlaySE( M_E_NEXT );
	}

#endif // DEBUG_MODE
}

void Title::MainUpdate()
{

#if DEBUG_MODE

	if ( IS_TRG_EXPOSURE && state != State::GotoGame )
	{
		nextState = State::GotoGame;
		FadeBegin();

		PlaySE( M_E_NEXT );
	}

#endif // DEBUG_MODE



#if USE_IMGUI

	ImGui::Begin( "test_window" );

	static float foo = 0;
	ImGui::SliderFloat( "foo", &foo, 1.0f, 10.0f );

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
}

void Title::FadeEnd()
{
	Fade::GetInstance()->Uninit();
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
	
	// 背景
	{
		DrawExtendGraph
		(
			scast<int>( 0 - shake.x ),
			scast<int>( 0 - shake.y ),
			scast<int>( SCREEN_WIDTH  - shake.x ),
			scast<int>( SCREEN_HEIGHT - shake.y ),
			TitleImage::hTitleBG,
			TRUE
		);
	}
}