#include <array>
#include <string>

#include "DxLib.h"
#include "Common.h"
#include "Music.h"
#include "Input.h"

#include "SceneGame.h"
#include "SceneTitle.h"

#include "Collision.h"

#include "ScreenShake.h"

#include "Camera.h"
#include "Grid.h"

#include "FileIO.h"

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

	FileIO::ReadAllCamera();
	FileIO::ReadAllStars();
	FileIO::ReadAllNumMoves();

	GameImage::Load();
	CameraImage::Load();
	StarImage::Load();

	const float GRID_SIZE = scast<float>( StarImage::SIZE );
	Vector2 gridSize{ GRID_SIZE, GRID_SIZE };
	Grid::SetSize( gridSize );

	pCamera.reset( new Camera() );
	pCamera->Init( stageNumber );

	pStarMng.reset( new StarMng() );
	pStarMng->Init( stageNumber );

	pNumMoves.reset( new NumMoves() );
	pNumMoves->Init( stageNumber );

	ShakeInit();
}
void Game::Uninit()
{
	FileIO::ReleaseCameraData();
	FileIO::ReleaseStarsData();
	FileIO::ReleaseNumMovesData();

	GameImage::Release();
	CameraImage::Release();
	StarImage::Release();

	Grid::SetSize( { 0, 0 } );

	pCamera->Uninit();
	pStarMng->Uninit();
	pNumMoves->Uninit();

	ShakeUninit();
}

void Game::Update()
{
#if DEBUG_MODE

	if ( TRG( KEY_INPUT_C ) )
	{
		char debugstoper = 0;
	}

	if ( TRG( KEY_INPUT_V ) )
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
	case State::Select:
		SelectUpdate();			break;
	case State::Game:
		GameUpdate();			break;
	case State::Clear:
		ClearUpdate();			break;
	default:
		assert( !"Error:SceneGame state error." );
		break;
	}

	CollisionCheck();

#if DEBUG_MODE	// TODO:ここをデバッグビルドかどうか判定するやつに変えたい

	constexpr int DEBUG_CHANGE_SCENE_TIME = 45;
	static int debugTimerForChangeScene = 0;
	if ( state == State::Clear )
	{
		debugTimerForChangeScene++;
	}

	if	(
			TRG( KEY_INPUT_RETURN )
			|| ( state == State::Clear && DEBUG_CHANGE_SCENE_TIME <= debugTimerForChangeScene )
		)
	{
		PrepareChangeSceneToTitle();

		debugTimerForChangeScene = 0;

		delete this;
		return;
	}

#endif // DEBUG_MODE
}

void Game::SelectUpdate()
{

}

void Game::GameUpdate()
{
	if ( pCamera )
	{
		pCamera->Update();

		if ( pCamera->IsExposure() )
		{
			if ( Exposure() )
			{
				numMoves++;
			}
		}
	}

	if ( pStarMng )
	{
		pStarMng->Update();

		if ( IS_TRG_UNDO )
		{
			if ( pStarMng->Undo() )
			{
				numMoves--;
			}
		}
	}

	if ( pNumMoves )
	{
		pNumMoves->Update();
	}

#if USE_IMGUI

	FileIO::UpdateNowStageNumberByImGui();
	if ( FileIO::IsCreateNewStage() )
	{
		if ( pCamera )
		{
			pCamera->SaveData();
		}
		if ( pStarMng )
		{
			pStarMng->SaveData();
		}
		if ( pNumMoves )
		{
			pNumMoves->SaveData();
		}

		FileIO::ReadAllCamera();
		FileIO::ReadAllStars();
		FileIO::ReadAllNumMoves();
	}

#endif // USE_IMGUI


	ShakeUpdate();
}

void Game::ClearUpdate()
{
	if ( pStarMng )
	{
		pStarMng->ClearUpdate();
	}

	ShakeUpdate();
}

bool Game::Exposure()
{
	assert( pCamera );

	if ( !pStarMng )
	{
		return false;
	}
	// else

	Box camera = pCamera->FetchColWorldPos();
	std::vector<int> targets;

	// 適用番号の検査
	int end = pStarMng->GetArraySize();
	for ( int i = 0; i < end; i++ )
	{
		Box star = pStarMng->FetchColWorldPos( i );
		if ( Box::IsHitBox( camera, star ) )
		{
			// １が入っている場合，使えない
			if ( pStarMng->FetchLevel( i ) <= 1 )
			{
				PlaySE( M_FAILURE );
				return false;
			}
			// else

			targets.push_back( i );
		}
	}

	// 適用対象がいなかったら終わる
	if ( !scast<int>( targets.size() ) || !end )
	{
		return false;
	}
	// else

	// TODO:Exposureの音を鳴らすタイミングは，成功が確定してからかどうか。
	PlaySE( M_EXPOSURE );

	// アンドゥ用
	pStarMng->SaveLog();

	// 適用
	for ( int i = 0; i < scast<int>( targets.size() ); i++ )
	{
		pStarMng->Expose( targets.at( i ) );
	}

	// 仮でこの場でクリア確認を行っているが，もし変化演出後とかに変えるのであれば，
	// カメラまたは星に IsDoneExposureEffect() のようなのを作るとよさそう
	if ( pStarMng->IsEqualLevels() )
	{
		state = State::Clear;
	}

	return true;
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
			0, 0,
			GameImage::hFrame,
			TRUE
		);
	}

	Grid::Draw( shake );

	if ( pStarMng )
	{
		pStarMng->Draw( shake );
	}

	if ( pCamera )
	{
		pCamera->Draw( shake );
	}

	DrawUI();

#if	DEBUG_MODE

	if ( isDrawCollision )
	{
		ShowCollisionArea();
	}

#endif	// DEBUG_MODE
}

void Game::DrawUI()
{
	DrawExtendFormatString
	(
		10, 10,
		2.0, 2.0,
		GetColor( 200, 200, 200 ),
		"今の手数：%d", numMoves
	);

	if ( state != State::Clear )
	{
		return;
	}
	// else

#if DEBUG_MODE

	DrawExtendFormatString
	(
		360, 300,
		6.0, 6.0,
		GetColor( 200, 200, 200 ),
		"Stage Clear!"
	);

	if ( pNumMoves )
	{
		int nowRank = pNumMoves->CalcRank( numMoves );

		std::array<unsigned int, 4> colours =
		{
			GetColor( 200, 200, 32 ),
			GetColor( 200, 64, 32 ),
			GetColor( 64, 200, 32 ),
			GetColor( 32, 64, 200 )
		};
		std::array<char, 4> ranks =
		{
			'S', 'A', 'B', 'C'
		};
		std::string result = "Rank : ";
		result.push_back( ranks[nowRank] );

		DrawExtendString
		(
			360, 360,
			6.0, 6.0,
			result.c_str(),
			colours[nowRank]
		);
	}

#endif // DEBUG_MODE

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

		if ( pCamera )
		{
			Box tmp = pCamera->FetchColWorldPos();

			DrawBoxAA
			(
				tmp.cx - tmp.w,
				tmp.cy - tmp.h,
				tmp.cx + tmp.w,
				tmp.cy + tmp.h,
				blue,
				TRUE
			);
		}

		if ( pStarMng )
		{
			int end = pStarMng->GetArraySize();
			for ( int i = 0; i < end; i++ )
			{
				Box star = pStarMng->FetchColWorldPos( i );

				DrawBoxAA
				(
					star.cx - star.w,
					star.cy - star.h,
					star.cx + star.w,
					star.cy + star.h,
					green,
					TRUE
				);
			}
		}
	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
}