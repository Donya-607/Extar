#include <array>

#include "DxLib.h"
#include "Music.h"
#include "Common.h"
#include "Input.h"

#include "SceneGame.h"
#include "SceneTitle.h"

#include "Collision.h"

#include "ScreenShake.h"

namespace GameImage
{
	static int hGameBG;

	void Load()
	{
		// Ç∑Ç≈Ç…ílÇ™ì¸Ç¡ÇƒÇ¢ÇΩÇÁÅCì«Ç›çûÇÒÇæÇ‡ÇÃÇ∆Ç›Ç»ÇµÇƒîÚÇŒÇ∑
		if ( 0 != hGameBG )
		{
			return;
		}
		// else

		hGameBG = LoadGraph( "./Data/Images/BG/Game.png" );
	}
	void Release()
	{
		DeleteGraph( hGameBG );
		hGameBG = 0;
	}
}

void Game::Init()
{
	PlayBGM( M_Game );

	GameImage::Load();

	ShakeInit();
}
void Game::Uninit()
{
	GameImage::Release();

	ShakeUninit();
}

void Game::Update()
{
#if DEBUG_MODE

	if ( TRG( KEY_INPUT_C ) || TRG_J_X( XB_LEFT ) )
	{
		char debugstoper = 0;
	}

	if ( TRG( KEY_INPUT_D ) )
	{
		isDrawCollision = !isDrawCollision;
	}

#endif // DEBUG_MODE

	if ( IsInputPauseButton() )
	{
		isPause = !isPause;
		PlaySE( M_E_NEXT );
	}

	if ( isPause )
	{
		PauseUpdate();

		if ( IsInputPauseButton() )
		{
			PlaySE( M_E_BACK );
			isPause = false;
		}

		return;
	}
	// else

	switch ( state )
	{
	case State::Game:
		GameUpdate();			break;
	default:
		assert( !"Error:SceneGame state error." );
		break;
	}

	CollisionCheck();

#if DEBUG_MODE

	if ( TRG( KEY_INPUT_RETURN ) )
	{
		PrepareChangeSceneToTitle();

		delete this;
		return;
	}

#endif // DEBUG_MODE
}
void Game::GameUpdate()
{
	ShakeUpdate();
}

bool Game::IsInputPauseButton()
{
	if	(
			0
			// state != State::Game
		)
	{
		return false;
	}
	// else
	if ( !IsTriggerPauseButton() )
	{
		return false;
	}
	// else

	return true;
}
void Game::PauseUpdate()
{
	
}

void Game::PrepareChangeSceneToTitle()
{
	Uninit();

	mpMng->mpScene = new Title( mpMng );
	mpMng->mpScene->Init();
}

void Game::Draw()
{
	Vector2 shake  = GetShakeAmount();

	// îwåi
	{
		DrawExtendGraph
		(
			scast<int>( 0 - shake.x ),
			scast<int>( 0 - shake.y ),
			scast<int>( SCREEN_WIDTH  - shake.x ),
			scast<int>( SCREEN_HEIGHT - shake.y ),
			GameImage::hGameBG,
			TRUE
		);
	}

#if	DEBUG_MODE

	if ( isDrawCollision )
	{
		ShowCollisionArea();
	}

#endif	// DEBUG_MODE
}

void Game::CollisionCheck()
{

}

void Game::ShowCollisionArea()
{
	Vector2 shake = GetShakeAmount();

	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 96 );
	{
		unsigned int red   = GetColor( 200, 32, 32 );
		unsigned int green = GetColor( 32, 200, 32 );
		unsigned int blue  = GetColor( 32, 32, 200 );
	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
}