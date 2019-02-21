#include <array>

#include "DxLib.h"
#include "Music.h"
#include "Common.h"
#include "Input.h"

#include "SceneGame.h"
#include "SceneTitle.h"

#include "Collision.h"

#include "ScreenShake.h"

#include "Camera.h"
#include "Grid.h"

namespace GameImage
{
	static int hGameBG;
	static int hFrame;

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( 0 != hGameBG )
		{
			return;
		}
		// else

		hGameBG = LoadGraph( "./Data/Images/BG/Game.png"  );
		hFrame	= LoadGraph( "./Data/Images/BG/Frame.png" );
	}
	void Release()
	{
		DeleteGraph( hGameBG );
		DeleteGraph( hFrame  );
		hGameBG = 0;
		hFrame  = 0;
	}
}

void Game::Init()
{
	PlayBGM( M_Game );

	GameImage::Load();
	CameraImage::Load();

	Vector2 gridSize{ 64.0f, 64.0f };
	Grid::SetSize( gridSize );

	pCamera.reset( new Camera() );
	pCamera->Init( /* sizeX = */3, /* sizeY = */2, /* moveAmount = */1 );

	ShakeInit();
}
void Game::Uninit()
{
	GameImage::Release();
	CameraImage::Release();

	Grid::SetSize( { 0, 0 } );

	pCamera->Uninit();

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

#if DEBUG_MODE	// TODO:ここをデバッグビルドかどうか判定するやつに変えたい

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
	if ( pCamera )
	{
		pCamera->Update();
	}

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

	// 背景
	{
		// 仮置きなので，ExtendGraph
		DrawExtendGraph
		(
			scast<int>( 0 - shake.x ),
			scast<int>( 0 - shake.y ),
			scast<int>( SCREEN_WIDTH  - shake.x ),
			scast<int>( SCREEN_HEIGHT - shake.y ),
			GameImage::hGameBG,
			TRUE
		);

		// 枠
		DrawGraph
		(
			FRAME_POS_X,
			FRAME_POS_Y,
			GameImage::hFrame,
			TRUE
		);
	}

	Grid::Draw( shake );

	if ( pCamera )
	{
		pCamera->Draw( shake );
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