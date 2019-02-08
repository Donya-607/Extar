#include "DxLib.h"
#include "Music.h"
#include "Common.h"
#include "Input.h"

#include "ScreenShake.h"

#include "SceneGame.h"
#include "SceneTitle.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

namespace TitleImage
{
	static int hTitleBG;

	void Load()
	{
		// Ç∑Ç≈Ç…ílÇ™ì¸Ç¡ÇƒÇ¢ÇΩÇÁÅCì«Ç›çûÇÒÇæÇ‡ÇÃÇ∆Ç›Ç»ÇµÇƒîÚÇŒÇ∑
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
void Title::Uninit()
{
	TitleImage::Release();

	ShakeUninit();
}

void Title::Update()
{
#if DEBUG_MODE

	if ( TRG( KEY_INPUT_C ) || TRG_J_X( XB_LEFT ) )
	{
		char debugstoper = 0;
	}

#endif // DEBUG_MODE

	ShakeUpdate();

	switch ( state )
	{
	case State::Main:
		MainUpdate(); break;
	default:
		assert( !"Error:SceneTitle state error." );
		break;
	}

#if DEBUG_MODE

	if ( TRG( KEY_INPUT_RETURN ) )
	{
		PrepareChangeSceneToGame();
		delete this;
		return;
	}

#endif // DEBUG_MODE
}
void Title::MainUpdate()
{
#if USE_IMGUI

	ImGui::Begin( "test_window" );

	static float foo = 0;
	ImGui::SliderFloat( "foo", &foo, 1.0f, 10.0f );

	ImGui::End();

#endif // USE_IMGUI
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
	
	// îwåi
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