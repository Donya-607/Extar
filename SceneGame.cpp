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

#include "Number.h"

#include "Camera.h"
#include "Grid.h"

#include "FileIO.h"

#include "Fade.h"

namespace SelectImage
{
	static int hSelectBG;

	void Load()
	{
		// ���łɒl�������Ă�����C�ǂݍ��񂾂��̂Ƃ݂Ȃ��Ĕ�΂�
		if ( 0 != hSelectBG )
		{
			return;
		}
		// else

		hSelectBG = LoadGraph( "./Data/Images/BG/Select.png" );
	}
	void Release()
	{
		DeleteGraph( hSelectBG );
		hSelectBG = 0;
	}
}
namespace GameImage
{
	static int hGameBG;
	static int hFrameBG;
	static int hFrameUI;

	void Load()
	{
		// ���łɒl�������Ă�����C�ǂݍ��񂾂��̂Ƃ݂Ȃ��Ĕ�΂�
		if ( 0 != hGameBG )
		{
			return;
		}
		// else

		hGameBG		= LoadGraph( "./Data/Images/BG/Game.png"  );
		hFrameBG	= LoadGraph( "./Data/Images/BG/Frame.png" );
		hFrameUI	= LoadGraph( "./Data/Images/UI/FrameUI.png" );
	}
	void Release()
	{
		DeleteGraph( hGameBG	);
		DeleteGraph( hFrameBG	);
		DeleteGraph( hFrameUI	);
		hGameBG		= 0;
		hFrameBG	= 0;
		hFrameUI	= 0;
	}
}
namespace PauseImage
{
	constexpr int SIZE_X = 736;
	constexpr int SIZE_Y = 576;

	constexpr int NUM_ROW = 5;

	// 0:All, 1:ToGame, 2:Retry, 3:ToSelect, 4:ToTitle
	static int hPauseStatements[NUM_ROW];

	void Load()
	{
		// ���łɒl�������Ă�����C�ǂݍ��񂾂��̂Ƃ݂Ȃ��Ĕ�΂�
		if ( 0 != hPauseStatements[0] )
		{
			return;
		}
		// else

		LoadDivGraph
		(
			"./Data/Images/Pause/Statement.png",
			NUM_ROW,
			NUM_ROW, 1,
			SIZE_X, SIZE_Y,
			hPauseStatements
		);
	}
	void Release()
	{
		for ( int i = 0; i < NUM_ROW; i++ )
		{
			DeleteGraph( hPauseStatements[i] );
			hPauseStatements[i] = 0;
		}
	}

	int  GetHandle( int index )
	{
		assert( 0 <= index && index <= NUM_ROW );

		return hPauseStatements[index];
	}
}

void Game::Init()
{
	PlayBGM( M_Game );

	FileIO::ReadAllCamera();
	FileIO::ReadAllStars();
	FileIO::ReadAllNumMoves();

	Number::Load();
	SelectImage::Load();
	GameImage::Load();
	PauseImage::Load();
	CameraImage::Load();
	StarImage::Load();
	StageImage::Load();
	CursorImage::Load();

	hFont = CreateFontToHandle
	(
		"JF�h�b�gK12",
		-1,				// �T�C�Y�i-1�F�f�t�H���g�j
		-1,				// �����i-1�F�f�t�H���g�j
		DX_FONTTYPE_NORMAL
	);

	switch ( state )
	{
	case State::Select:
		SelectInit();		break;
	case State::Game:
		GameInit();			break;
	case State::Clear:
		ClearInit();		break;
	default:
		assert( !"Error:SceneGame state error." );
		exit( EXIT_FAILURE );
		return;
	}

	ShakeInit();
}
void Game::SelectInit()
{
	pCursor.reset( new Cursor() );
	pCursor->Init();
}
void Game::GameInit()
{
	const float GRID_SIZE = scast<float>( StarImage::SIZE );
	Vector2 gridSize{ GRID_SIZE, GRID_SIZE };
	Grid::SetSize( gridSize );

	pCamera.reset( new Camera() );
	pCamera->Init( stageNumber );

	pStarMng.reset( new StarMng() );
	pStarMng->Init( stageNumber );

	pNumMoves.reset( new NumMoves() );
	pNumMoves->Init( stageNumber );

	numMoves = 0;
	choice = 0;
}
void Game::ClearInit()
{
	choice = 0;
}

void Game::Uninit()
{
	SelectUninit();
	GameUninit();
	ClearUninit();

	FileIO::ReleaseCameraData();
	FileIO::ReleaseStarsData();
	FileIO::ReleaseNumMovesData();

	Number::Release();
	SelectImage::Release();
	GameImage::Release();
	PauseImage::Release();
	CameraImage::Release();
	StarImage::Release();
	StageImage::Release();
	CursorImage::Release();

	DeleteFontToHandle( hFont );

	Grid::SetSize( { 0, 0 } );

	ShakeUninit();
}
void Game::SelectUninit()
{
	if ( pCursor )   { pCursor->Uninit();   }
}
void Game::GameUninit()
{
	if ( pCamera   ) { pCamera->Uninit();   }
}
void Game::ClearUninit()
{
	DeleteGraph( hScreenShot );
	hScreenShot = 0;

	if ( pStarMng  ) { pStarMng->Uninit();  }
	if ( pNumMoves ) { pNumMoves->Uninit(); }
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
		// ���͂ⓖ���蔻��\���͂���Ȃ�
		// isDrawCollision = !isDrawCollision;
	}

#endif // DEBUG_MODE

	if ( Fade::GetInstance()->IsDoneFade() )
	{
		if ( nextState == State::GotoTitle )
		{
			PrepareChangeSceneToTitle();

			delete this;
			return;
		}
		// else
	}
	// else

	FadeCheck();

	if ( IsInputPauseButton() )
	{
		isPause = !isPause;
		PlaySE( M_E_NEXT );
	}
	if ( isPause )
	{
		PauseUpdate();

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
		exit( EXIT_FAILURE );
		return;
	}

	CollisionCheck();

#if DEBUG_MODE	// TODO:�������f�o�b�O�r���h���ǂ������肷���ɕς�����

	if ( TRG( KEY_INPUT_RETURN ) )
	{
		nextState = State::GotoTitle;
		FadeBegin();

		PlaySE( M_E_NEXT );
	}

#endif // DEBUG_MODE
}

void Game::SelectUpdate()
{
	if ( pCursor )
	{
		pCursor->Update();

		stageNumber = pCursor->GetNowStageNumber();

		if ( nextState == State::Null && pCursor->IsDecision() )
		{
			PlaySE( M_E_NEXT );

			nextState =
				( pCursor->IsChoiceBack() )
				? State::GotoTitle
				: State::Game;

			FadeBegin();
		}
	}
}

void Game::GameUpdate()
{
	if ( isTakeScreenShot )
	{
		isTakeScreenShot = false;
		FadeBegin();
	}

	if ( pCamera )
	{
		pCamera->Update();

		if ( pCamera->IsExposure() )
		{
			if ( Exposure() )
			{
				numMoves++;
			}

			// ���ł��̏�ŃN���A�m�F���s���Ă��邪�C�����ω����o��Ƃ��ɕς���̂ł���΁C
			// �J�����܂��͐��� IsDoneExposureEffect() �̂悤�Ȃ̂����Ƃ悳����
			if ( pStarMng->IsEqualLevels() )
			{
				PlaySE( M_E_NEXT );

				nextState = State::Clear;
				isClear = true;

				FadeEnd();	// �t���O�̏���������������
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

#if DEBUG_MODE

	if ( TRG( KEY_INPUT_E ) && nextState == State::Null/* �A�Ŗh�~ */ )
	{
		PlaySE( M_E_NEXT );

		nextState = State::Clear;
		isClear = true;

		FadeEnd();	// �t���O�̏���������������
	}

#endif // DEBUG_MODE

#if USE_IMGUI

	FileIO::UpdateShowWIndowState();
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

#if DEBUG_MODE

	if ( TRG( KEY_INPUT_E ) && pCursor->IsDecision() && nextState == State::Null/* �A�Ŗh�~ */ )
	{
		PlaySE( M_E_NEXT );

		nextState = State::Select;
		FadeBegin();
	}

#endif // DEBUG_MODE

	ShakeUpdate();
}

void Game::FadeBegin()
{
	constexpr int MOVE_INTERVAL = 1;
	Vector2 pos = FadeImage::GetSize();
	pos *= -1;
	pos.x += scast<float>( SCREEN_WIDTH  ) * 0.2f/* �ʒu�̒��� */;
	pos.y -= scast<float>( SCREEN_HEIGHT ) * 1.0f/* �ʒu�̒��� */;

	Fade::GetInstance()->Init( MOVE_INTERVAL, pos );
}

void Game::FadeCheck()
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

void Game::FadeDone()
{
	// �V�[���J�ڃ`�F�b�N�͐�ɂ��Ă���̂ŁC����Ɉ���������͂��͂Ȃ��z��
	assert( nextState != State::GotoTitle );

	switch ( state )
	{
	case State::Select:
		SelectUninit();		break;
	case State::Game:
		GameUninit();		break;
	case State::Clear:
		ClearUninit();		break;
	default:
		assert( !"Error:SceneGame state error." );
		exit( EXIT_FAILURE );
		return;
	}

	state = nextState;
	nextState = State::Null;

	switch ( state )
	{
	case State::Select:
		SelectInit();		break;
	case State::Game:
		GameInit();			break;
	case State::Clear:
		ClearInit();		break;
	default:
		assert( !"Error:SceneGame state error." );
		exit( EXIT_FAILURE );
		return;
	}

	numMoves = 0;
	isPause = false;
}

void Game::FadeEnd()
{
	Fade::GetInstance()->Uninit();
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

	// �K�p�ԍ��̌���
	int end = pStarMng->GetArraySize();
	for ( int i = 0; i < end; i++ )
	{
		Box star = pStarMng->FetchColWorldPos( i );
		if ( Box::IsHitBox( camera, star ) )
		{
			// �P�������Ă���ꍇ�C�g���Ȃ�
			if ( pStarMng->FetchLevel( i ) <= 1 )
			{
				PlaySE( M_FAILURE );
				return false;
			}
			// else

			targets.push_back( i );
		}
	}

	// �K�p�Ώۂ����Ȃ�������I���
	if ( !scast<int>( targets.size() ) || !end )
	{
		return false;
	}
	// else

	// TODO:Exposure�̉���炷�^�C�~���O�́C�������m�肵�Ă��炩�ǂ����B
	PlaySE( M_EXPOSURE );

	// �A���h�D�p
	pStarMng->SaveLog();

	// �K�p
	for ( int i = 0; i < scast<int>( targets.size() ); i++ )
	{
		pStarMng->Expose( targets.at( i ) );
	}

	return true;
}

bool Game::IsInputPauseButton()
{
	if ( state != State::Game )
	{
		return false;
	}
	// else
	if ( !IsTriggerPauseButton() )
	{
		return false;
	}
	// else
	if ( nextState != State::Null )
	{
		return false;
	}
	// else

	if ( !isPause )
	{
		choice = 0;
	}

	return true;
}
void Game::PauseUpdate()
{
	constexpr int MAX_MENU = 4;

	bool isUp = false, isDown = false;

	if ( IS_TRG_UP	 ) { isUp	= true; }
	if ( IS_TRG_DOWN ) { isDown	= true; }

	if ( ( 0		< choice		)	&& isUp		&& !isDown	) { choice -= 1; PlaySE( M_E_NEXT ); }
	if ( ( choice	< MAX_MENU - 1	)	&& isDown	&& !isUp	) { choice += 1; PlaySE( M_E_NEXT ); }

	assert( 0 <= choice  && choice < MAX_MENU );

	if ( IS_TRG_EXPOSURE )
	{
		PlaySE( M_E_NEXT );

		switch ( choice )
		{
		case 0:
			{
				if ( state == State::Game )
				{
					isPause = false;
					return;
				}
				// else
				if ( state == State::Clear )
				{
					nextState = State::Game;
					if ( stageNumber <= FileIO::GetMaxStageNumber() )
					{
						stageNumber++;
					}
					else
					{
						assert( !"Error : Next_Stage is Not Exists." );
						exit( EXIT_FAILURE );
						return;
					}

					FadeBegin();
				}
			}
			break;
		case 1:
			{
				nextState = State::Game;
				FadeBegin();
			}
			break;
		case 2:
			{
				nextState = State::Select;
				FadeBegin();
			}
			break;
		case 3:
			{
				nextState = State::GotoTitle;
				FadeBegin();
			}
			break;
		}
	}
}

void Game::PrepareChangeSceneToTitle()
{
	Uninit();

	mpMng->mpScene = new Title( mpMng );
	mpMng->mpScene->Init();
}

void Game::TakeScreenShot()
{
	int result = SaveDrawScreenToPNG
	(
		FRAME_POS_X,
		FRAME_POS_Y,
		FRAME_POS_X + FRAME_WIDTH,
		FRAME_POS_Y + FRAME_HEIGHT,
		"./Data/Images/ScreenShot/ScreenShot.png"
	);
	if ( result != 0 )
	{
		assert( !"ScreenShot Error!" );
		exit( EXIT_FAILURE );
		return;
	}
	// else

	hScreenShot = LoadGraph( "./Data/Images/ScreenShot/ScreenShot.png" );

	isClear = false;
	isTakeScreenShot = true;
}

void Game::Draw()
{
	constexpr int PAUSE_BRIGHTNESS = 64;

	Vector2 shake = GetShakeAmount();

	if ( isPause ) { SetDrawBright( PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS ); }

	switch ( state )
	{
	case State::Select:
		SelectDraw();
		if ( isPause ) { SetDrawBright( PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS ); }
		SelectDrawUI();		break;
	case State::Game:
		GameDraw();
		if ( isPause ) { SetDrawBright( PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS ); }
		GameDrawUI();		break;
	case State::Clear:
		ClearDraw();
		if ( isPause ) { SetDrawBright( PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS ); }
		ClearDrawUI();		break;
	default:
		assert( !"Error:SceneGame state error." );
		exit( EXIT_FAILURE );
		return;
	}

	if ( isPause )
	{
		SetDrawBright( 255, 255, 255 );

		PauseDraw();
	}

#if	DEBUG_MODE

	if ( isDrawCollision )
	{
		ShowCollisionArea();
	}

#endif	// DEBUG_MODE
}

void Game::SelectDraw()
{
	Vector2 shake = GetShakeAmount();

	// �w�i
	{
		// ���u���Ȃ̂ŁCExtendGraph
		DrawExtendGraph
		(
			scast<int>( 0 - shake.x ),
			scast<int>( 0 - shake.y ),
			scast<int>( SCREEN_WIDTH - shake.x ),
			scast<int>( SCREEN_HEIGHT - shake.y ),
			SelectImage::hSelectBG,
			TRUE
		);
	}

	StageSelect::Draw( stageNumber );

	if ( pCursor )
	{
		pCursor->Draw( shake );
	}
}

void Game::GameDraw()
{
	Vector2 shake = GetShakeAmount();

	// �w�i
	{
		DrawGraph
		(
			0, 0,
			GameImage::hGameBG,
			TRUE
		);

		SetDrawBlendMode( DX_BLENDMODE_ADD, 255 );
		// �g
		DrawGraph
		(
			0, 0,
			GameImage::hFrameBG,
			TRUE
		);
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );

		// �g�̉摜
		DrawGraph
		(
			0, 0,
			GameImage::hFrameUI,
			TRUE
		);
	}

	Grid::Draw( shake );

	if ( pStarMng )
	{
		pStarMng->Draw( shake );
	}

	if ( isClear )
	{
		TakeScreenShot();
		return;
	}
	// else

	if ( pCamera )
	{
		pCamera->Draw( shake );
	}
}

void Game::ClearDraw()
{
	Vector2 shake = GetShakeAmount();

	// �w�i
	{
		DrawGraph
		(
			0, 0,
			GameImage::hGameBG,
			TRUE
		);

		SetDrawBlendMode( DX_BLENDMODE_ADD, 255 );
		// �g
		DrawGraph
		(
			0, 0,
			GameImage::hFrameBG,
			TRUE
		);
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );

		// �g�̉摜
		DrawGraph
		(
			0, 0,
			GameImage::hFrameUI,
			TRUE
		);
	}

	if ( pStarMng )
	{
		pStarMng->Draw( shake );
	}

	if ( hScreenShot != 0 )
	{
		int x = 0;
		int y = 0;

		GetMousePoint( &x, &y );

		DrawGraph
		(
			x, y,
			hScreenShot,
			TRUE
		);
	}
}

void Game::PauseDraw()
{
	/*
	int x = 0;
	int y = 0;

	GetMousePoint( &x, &y );

	DrawCircle
	(
		x, y,
		16,
		GetColor( 200, 200, 200 )
	);
	*/

	// ����
	DrawGraph
	(
		608,
		288,
		PauseImage::GetHandle( 0 ),
		TRUE
	);
	// ����
	DrawGraph
	(
		608,
		288,
		PauseImage::GetHandle( choice + 1 ),
		TRUE
	);
}

void Game::SelectDrawUI()
{

}

void Game::GameDrawUI()
{
	// �萔
	{
		DrawExtendFormatString
		(
			10, 10,
			2.0, 2.0,
			GetColor( 200, 200, 200 ),
			"���̎萔�F%d", numMoves
		);
	}

	// �����x���̕\��
	{
		const Vector2 BASE{ scast<float>( FRAME_POS_X + FRAME_WIDTH ),scast<float>( FRAME_POS_Y ) };
		const Vector2 TWEAK{ 48.0f, 0 };
		const Vector2 HALF_SIZE{ Grid::GetSize().x * 0.5f , Grid::GetSize().y * 0.5f };

		for ( int i = 1; i <= Star::MAX_LEVEL; i++ )
		{
			double angle = ( i % 2 ) ? 0 : 45.0;

			DrawExtendFormatString
			(
				scast<int>( BASE.x ),
				scast<int>( BASE.y ) + ( ( i - 1 ) * StarImage::SIZE ),
				1.0, 1.0,
				GetColor( 200, 200, 200 ),
				"���x�� %d", i
			);

			DrawRotaGraph
			(
				scast<int>( BASE.x + TWEAK.x + HALF_SIZE.x ),									// ���S���W
				scast<int>( BASE.y + TWEAK.y + HALF_SIZE.y ) + ( ( i - 1 ) * StarImage::SIZE ),	// ���S���W
				1.0, ToRadian( angle ),
				StarImage::GetHandle( i, 0 ),
				TRUE
			);
		}
	}
}

void Game::ClearDrawUI()
{

#if DEBUG_MODE

	DrawExtendFormatStringToHandle
	(
		300, 300,
		6.0, 6.0,
		GetColor( 32, 32, 32 ),
		hFont,
		"Stage Clear!"
	);
	/*
	DrawExtendFormatString
	(
		360, 300,
		6.0, 6.0,
		GetColor( 200, 200, 200 ),
		"Stage Clear!"
	);
	*/

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

		DrawExtendStringToHandle
		(
			360, 400,
			6.0, 6.0,
			result.c_str(),
			colours[nowRank],
			hFont
		);
		/*
		DrawExtendString
		(
			360, 400,
			6.0, 6.0,
			result.c_str(),
			colours[nowRank]
		);
		*/
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