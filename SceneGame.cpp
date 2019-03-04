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

namespace TextBehavior
{
	// 文の数とフレームの数は揃えてください

	const std::vector<std::string> TUTORIAL =
	{
		"やっほー、初心者さん？　やり方教えるね♪",
		"水色の枠がカメラの範囲だよ！",
		"範囲内に星をおさめると、ＸかＡで露光が使えるよ",

		"露光は、範囲内にある星を明るくできるよ",
		"すべての星の明るさをそろえよう★",
		"セレクトボタンでもう一度教えるよ！"
	};
	const std::vector<int> TUTORIAL_SHOW_FRAME =
	{
		210,
		180,
		180,

		180,
		180,
		-1
	};
	const std::vector<std::string> EMPHASIS_STR =	// RGB( 87, 101, 255 )
	{
		"水色の枠",
		"露光",
		"すべて",
		"セレクトボタン"
	};

	const std::vector<std::string> RAND_SAY =
	{
		"ファイトォ！",
		"んー悩むねー",
		"難しいなぁ・・・",

		"その調子　その調子！",
		"がんばれー！",
		"んー、どうするんだろう・・・",

		"迷うなぁ・・・",
		"いい感じかも♪",
		"星がきれいだねー",

		"キラッ★"
	};
	const std::vector<int> RAND_SAY_SHOW_FRAME =
	{
		180,
		180,
		180,

		180,
		180,
		180,

		180,
		180,
		180,

		90
	};

	const std::vector<std::string> CLEAR_SAY =
	{
		"良いね！",
		"おめでとう！",
		"ナイス！",
		"やったね！"
	};
	const std::vector<int> CLEAR_SAY_SHOW_FRAME =
	{
		60,
		60,
		60,
		60
	};
}

namespace SelectImage
{
	static int hSelectBG;

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
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
	static int hMovesUI;


	static int hshutter;


	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( 0 != hGameBG )
		{
			return;
		}
		// else

		hGameBG		= LoadGraph( "./Data/Images/BG/Game.png"  );
		hFrameBG	= LoadGraph( "./Data/Images/BG/Frame.png" );
		hFrameUI	= LoadGraph( "./Data/Images/UI/FrameUI.png" );
		hMovesUI	= LoadGraph( "./Data/Images/UI/NumberOfMoves.png" );


		hshutter	= LoadGraph( "./Data/Images/Camera/Shutter.png" );


	}
	void Release()
	{
		DeleteGraph( hGameBG	);
		DeleteGraph( hFrameBG	);
		DeleteGraph( hFrameUI	);
		DeleteGraph( hMovesUI );
		hGameBG		= 0;
		hFrameBG	= 0;
		hFrameUI	= 0;
		hMovesUI	= 0;


		DeleteGraph( hshutter	);
		hshutter		= 0;


	}
}
namespace ClearImage
{
	constexpr int SIZE_STAR_X = 192;
	constexpr int SIZE_STAR_Y = 224;

	static int hClearBG;
	static int hRecordStatement;
	static int hRecordStar[2];	// 0:Dark, 1:Glow

	constexpr int SIZE_GOTO_NEXT_X = 352;
	constexpr int SIZE_GOTO_NEXT_Y = 96;
	static int hGotoNext;

	constexpr int NUM_STATEMENT_ROW = 5;
	constexpr int SIZE_STATEMENT_X = 736;
	constexpr int SIZE_STATEMENT_Y = 576;

	// 0:All, 1:ToGame, 2:Retry, 3:ToSelect, 4:ToTitle
	static int hMenuStatements[NUM_STATEMENT_ROW];
	static int hFinalStageMenuStatements[NUM_STATEMENT_ROW];

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( 0 != hClearBG )
		{
			return;
		}
		// else

		hClearBG = LoadGraph( "./Data/Images/BG/Clear.png" );
		hRecordStatement = LoadGraph( "./Data/Images/Result/RecordStatement.png" );

		LoadDivGraph
		(
			"./Data/Images/Result/RecordStar.png",
			2,
			2, 1,
			SIZE_STAR_X, SIZE_STAR_Y,
			hRecordStar
		);

		hGotoNext = LoadGraph( "./Data/Images/Result/GotoNext.png" );

		LoadDivGraph
		(
			"./Data/Images/Result/Statement.png",
			NUM_STATEMENT_ROW,
			NUM_STATEMENT_ROW, 1,
			SIZE_STATEMENT_X, SIZE_STATEMENT_Y,
			hMenuStatements
		);
		LoadDivGraph
		(
			"./Data/Images/Result/FinalStageStatement.png",
			NUM_STATEMENT_ROW,
			NUM_STATEMENT_ROW, 1,
			SIZE_STATEMENT_X, SIZE_STATEMENT_Y,
			hFinalStageMenuStatements
		);
	}
	void Release()
	{
		DeleteGraph( hClearBG			);
		DeleteGraph( hRecordStatement	);
		hClearBG			= 0;
		hRecordStatement	= 0;

		DeleteGraph( hRecordStar[0] );
		DeleteGraph( hRecordStar[1] );
		hRecordStar[0] = 0;
		hRecordStar[1] = 0;

		DeleteGraph( hGotoNext );
		hGotoNext = 0;

		for ( int i = 0; i < NUM_STATEMENT_ROW; i++ )
		{
			DeleteGraph( hMenuStatements[i]				);
			DeleteGraph( hFinalStageMenuStatements[i]	);
			hMenuStatements[i]				= 0;
			hFinalStageMenuStatements[i]	= 0;
		}
	}

	int  GetStatementHandle( int index, bool isFinalStage )
	{
		assert( 0 <= index && index < NUM_STATEMENT_ROW );

		if ( isFinalStage )
		{
			return hFinalStageMenuStatements[index];
		}
		// else
		return hMenuStatements[index];
	}
}
namespace ClearRelated
{
	constexpr int FADE_WAIT			= 80;
	constexpr int GOTO_NEXT_WAIT	= 120;
}

namespace MilkyWayImage
{
	static int hShootingStar[2];

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( 0 != hShootingStar[0] )
		{
			return;
		}
		// else

		hShootingStar[0] = LoadGraph( "./Data/Images/Title/TitleStar1.png" );
		hShootingStar[1] = LoadGraph( "./Data/Images/Title/TitleStar2.png" );
	}
	void Release()
	{
		DeleteGraph( hShootingStar[0] );
		DeleteGraph( hShootingStar[1] );
		hShootingStar[0] = 0;
		hShootingStar[1] = 0;
	}

	int  GetHandle( int index )
	{
		assert( 0 <= index && index <= 1 );

		return hShootingStar[index];
	}
}

namespace PauseImage
{
	constexpr int SIZE_X = 736;
	constexpr int SIZE_Y = 576;

	constexpr int NUM_ROW = 5;

	// 0:All, 1:ToGame, 2:Retry, 3:ToSelect, 4:ToTitle
	static int hPauseStatements[NUM_ROW];
	static int hPause;

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
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

		hPause = LoadGraph( "./Data/Images/Pause/Pause.png" );
	}
	void Release()
	{
		for ( int i = 0; i < NUM_ROW; i++ )
		{
			DeleteGraph( hPauseStatements[i] );
			hPauseStatements[i] = 0;
		}

		DeleteGraph( hPause );
		hPause = 0;
	}

	int  GetMenuHandle( int index )
	{
		assert( 0 <= index && index <= NUM_ROW );

		return hPauseStatements[index];
	}
	int  GetHandle()
	{
		return hPause;
	}
}

namespace HumanImage
{
	constexpr int NUM_ROW	 = 2;
	constexpr int NUM_COLUMN = 2;
	constexpr int NUM_ALL = NUM_ROW * NUM_COLUMN;

	constexpr int SIZE_X = 256;
	constexpr int SIZE_Y = 352;

	// static int hBody[NUM_ALL];
	// static int hArm[NUM_ALL];
	static int hBody;
	static int hArm;

	constexpr int NUM_MOUTH_ROW = 3;
	constexpr int SIZE_MOUTH_X  = 256;
	constexpr int SIZE_MOUTH_Y  = 352;
	static int hMouth[NUM_MOUTH_ROW];

	constexpr int SIZE_BALLOON_X = 928;
	constexpr int SIZE_BALLOON_Y = 160;

	static int hBalloon;	// フキダシ

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		// if ( 0 != hBody[0] )
		if ( 0 != hBody )
		{
			return;
		}
		// else

		/*
		LoadDivGraph
		(
			"./Data/Images/Human/Body.png",
			NUM_ALL,
			NUM_ROW, NUM_COLUMN,
			SIZE_X, SIZE_Y,
			hBody
		);

		LoadDivGraph
		(
			"./Data/Images/Human/Arm.png",
			NUM_ALL,
			NUM_ROW, NUM_COLUMN,
			SIZE_X, SIZE_Y,
			hArm
		);
		*/
		hBody = LoadGraph( "./Data/Images/Human/Body.png" );
		hArm  = LoadGraph( "./Data/Images/Human/Arm.png" );

		LoadDivGraph
		(
			"./Data/Images/Human/Mouth.png",
			NUM_MOUTH_ROW,
			NUM_MOUTH_ROW, 1,
			SIZE_MOUTH_X, SIZE_MOUTH_Y,
			hMouth
		);

		hBalloon = LoadGraph( "./Data/Images/Human/Balloon.png" );
	}
	void Release()
	{
		// for ( int i = 0; i < NUM_ALL; i++ )
		{
			DeleteGraph( hBody );
			DeleteGraph( hArm );
			hBody = 0;
			hArm = 0;
		}
		for ( int i = 0; i < NUM_MOUTH_ROW; i++ )
		{
			DeleteGraph( hMouth[i] );
			hMouth[i] = 0;
		}

		DeleteGraph( hBalloon );
		hBalloon = 0;
	}

	int  GetBodyHandle( int index )
	{
		// assert( 0 <= index && index < NUM_ALL );

		// return hBody[index];
		return hBody;
	}
	int  GetArmHandle( int index )
	{
		// assert( 0 <= index && index < NUM_ALL );

		// return hArm[index];
		return hArm;
	}
	int  GetMouthHandle( int index )
	{
		assert( 0 <= index && index < NUM_MOUTH_ROW );

		return hMouth[index];
	}
	int  GetBalloonHandle()
	{
		return hBalloon;
	}
}
namespace HumanBehavior
{
	constexpr int BALLOON_POS_X = 256;
	constexpr int BALLOON_POS_Y = 896;

	constexpr float	HAND_LET_DOWN_PLUS_Y = 128.0f;

	constexpr int	RISE_REQUIRED_TIME	 = 40;
	constexpr float	HAND_RISE_SPD_Y = scast<float>( RISE_REQUIRED_TIME ) / 6;
}

void RecordStar::Init( Vector2 centerPos, bool isGlowStar )
{
	pos		= centerPos;
	isGlow	= isGlowStar;

	if ( isGlow )
	{
		rotateSpd	= 24.0f;

		magniSpd	= -0.3f;
		scale		= 2.0f;

		return;
	}
	// else

	rotateSpd	= 19.0f;

	magniSpd	= -0.1f;
}

void RecordStar::Update()
{
	if( angle < 360.0f )
	{
		constexpr float MINUS_SPD	= 0.3f;
		constexpr float LOWER_SPD	= 0.5f;

		rotateSpd -= MINUS_SPD;
		if ( rotateSpd < LOWER_SPD )
		{
			rotateSpd = LOWER_SPD;
		}

		angle += rotateSpd;
	}
	else
	{
		angle = 360.0f;
	}

	if ( 1.0f < scale )
	{
		constexpr float MINUS_SPD	= 0.15f;
		constexpr float LOWER_SPD	= 0.1f;

		magniSpd += MINUS_SPD;
		if ( LOWER_SPD < magniSpd )
		{
			magniSpd = LOWER_SPD;
		}

		scale -= magniSpd;
	}
	else
	{
		scale = 1.0f;
	}
}

void RecordStar::Draw( Vector2 shake ) const
{
	DrawRotaGraph
	(
		scast<int>( pos.x ),
		scast<int>( pos.y ),
		scale,
		ToRadian( angle ),
		ClearImage::hRecordStar[( isGlow ) ? 1 : 0],
		TRUE
	);
}

void Game::Init()
{
	FileIO::ReadAllCamera();
	FileIO::ReadAllStars();
	FileIO::ReadAllNumMoves();

	Number::Load();

	SelectImage::Load();
	GameImage::Load();
	ClearImage::Load();
	PauseImage::Load();
	HumanImage::Load();

	MilkyWayImage::Load();
	ShootingStarImage::Load();

	CameraImage::Load();
	StarImage::Load();
	StageImage::Load();
	CursorImage::Load();
	BoardImage::Load();

	hFont = CreateFontToHandle
	(
		"JFドットK12",
		-1,				// サイズ（-1：デフォルト）
		-1,				// 太さ（-1：デフォルト）
		DX_FONTTYPE_NORMAL
	);

	pSSMng.reset( new ShootingStarMng() );
	pSSMng->Init();

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
	PlayBGM( M_Title_Select );

	numMoves = 0;

	pCursor.reset( new Cursor() );
	pCursor->SetNowStagenumber( stageNumber );
	pCursor->Init();
}
void Game::GameInit()
{
	PlayBGM( M_Game );

	const float GRID_SIZE = scast<float>( StarImage::SIZE );
	Vector2 gridSize{ GRID_SIZE, GRID_SIZE };
	Grid::SetSize( gridSize );

	pCamera.reset( new Camera() );
	pCamera->Init( stageNumber );

	pStarMng.reset( new StarMng() );
	pStarMng->Init( stageNumber );

	pNumMoves.reset( new NumMoves() );
	pNumMoves->Init( stageNumber );

	numMoves	= 0;
	pauseTimer	= 0;
	choice		= 0;

	mouthIndex		= 0;
	balloonLength	= 0;
	textTimer		= 0;
	textLength		= 0;
	textExtendInterval = 0;
	textNumber		= 0;

	armPos		= { 0, scast<float>( ( SCREEN_HEIGHT - HumanImage::SIZE_Y ) + HumanBehavior::HAND_LET_DOWN_PLUS_Y ) };
	pausePos	= { 704.0f, 64.0f };

	isOpenBalloon = true;
	isDoneMoveArm = false;

	// 他のＰＧの作業
	{
		str_up_pos		= { 0, 0 };
		str_down_pos	= { 192.0f, 64.0f };
		str_speed		= { 0.0f, 50.0f };
		shutter_flag	= false;
		shutter_state	= 0;

		DrawStart_X		= 190;
		DrawStart_Y		= 60;
		Src_X			= 14;
		Src_Y			= 150;
		Hgh				= 0;
		Wid				= 1538.0f;

		Src_Down_Y		= -790.0f;
		Hgh_Down		= 780.0f;
	}
}
void Game::ClearInit()
{
	PlayBGM( M_Result );

	choice = ( stageNumber == FileIO::GetMaxStageNumber() ) ? 1 : 0;
	clearTimer = 0;
	gotoNextPosX = SCREEN_WIDTH;

	pBoard.reset( new Board() );
	pBoard->Init( { 960.0f, scast<float>( -BoardImage::SIZE_Y ) } );

	recordStars.clear();

	isShowClearMenu = false;
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
	ClearImage::Release();
	PauseImage::Release();
	HumanImage::Release();

	MilkyWayImage::Release();
	ShootingStarImage::Release();

	CameraImage::Release();
	StarImage::Release();
	StageImage::Release();
	CursorImage::Release();
	BoardImage::Release();

	DeleteFontToHandle( hFont );

	if ( pSSMng ) { pSSMng->Uninit(); }

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
	clearTimer = 0;

	DeleteGraph( hScreenShot );
	hScreenShot = 0;

	if ( pStarMng  ) { pStarMng->Uninit();  }
	if ( pNumMoves ) { pNumMoves->Uninit(); }
	if ( pBoard	   ) { pBoard->Uninit();	}

	recordStars.clear();

	// 隠す
	armPos = { scast<float>( SCREEN_WIDTH ), scast<float>( SCREEN_HEIGHT ) };
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
		// もはや当たり判定表示はいらない
		// isDrawCollision = !isDrawCollision;
	}

	// 流れ星を出す
	if ( pSSMng )
	{
		int x, y;
		GetMousePoint( &x, &y );

		if ( TRG( KEY_INPUT_S ) )
		{
			pSSMng->GenerateStar( { scast<float>( x ), scast<float>( y ) } );
		}
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
		PlaySE( M_PAUSE );
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

	if ( pSSMng )
	{
		pSSMng->Update();
	}

	// シャッター演出
	{
		if ( shutter_flag == true )
		{
			switch ( shutter_state )
			{
			case 0:
				Hgh += str_speed.y;
				Src_Down_Y += str_speed.y;
				if ( Hgh >= ( 768 * 0.5 ) + 50 )shutter_state++;
				break;

			case 1:
				Hgh -= str_speed.y;
				Src_Down_Y -= str_speed.y;
				if ( Hgh < 64 - 768 )
				{
					shutter_flag = false;
					shutter_state = 0;
					Hgh = 0;
					Src_Down_Y = -790.0f;

					nextState = State::Clear;
					FadeDone();
				}
				break;

			}
		}
	}


	CollisionCheck();

#if DEBUG_MODE	// TODO:ここをデバッグビルドかどうか判定するやつに変えたい

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
		pCursor->Update( isOpenFade/* フェードイン中の操作制限のため */ );

		stageNumber = pCursor->GetNowStageNumber();

		if ( nextState == State::Null && pCursor->IsDecision() )
		{
			if ( pCursor->IsChoiceBack() )
			{
				nextState = State::GotoTitle;
				PlaySE( M_GOTO_TITLE );
			}
			else
			{
				nextState = State::Game;
				PlaySE( M_DECISION );
			}

			FadeBegin();
		}
	}
}

void Game::GameUpdate()
{
	if ( isTakeScreenShot )
	{
		isTakeScreenShot = false;

		// FadeBegin();
		shutter_flag = true;
		isOpenFade = false;

		PlaySE( M_SHUTTER );
	}

	// カメラの更新より先に判定し，描画後にスクショが始まるようにする
	if ( !isDoneMoveArm && pStarMng->IsEqualLevels() )
	{
		if ( nextState == State::Null )
		{
			/* テキストボックスを隠す
			{
				mouthIndex = 0;
				textLength = 0;
				textExtendInterval = 0;

				isOpenBalloon = false;
			}
			*/

			// クリア台詞を出す
			{
				if ( !balloonLength )
				{
					constexpr int INIT_LENGTH = 64;
					balloonLength = INIT_LENGTH;
				}

				textTimer = 0;
				textLength = 1;
				textExtendInterval = 0;
				textNumber = rand() % scast<int>( TextBehavior::CLEAR_SAY.size() );

				isOpenBalloon = true;
			}

			nextState = State::Clear;

			isOpenFade = false;	// 操作不能にするため
			isDoneMoveArm = false;
		}	

		armPos.y -= HumanBehavior::HAND_RISE_SPD_Y;

		if ( armPos.y < ( SCREEN_HEIGHT - HumanImage::SIZE_Y ) )
		{
			// armPos.y = scast<float>( SCREEN_HEIGHT - HumanImage::SIZE_Y );
			armPos.y += HumanBehavior::HAND_RISE_SPD_Y;

			isClearMoment	= true;
			isDoneMoveArm	= true;
		}
	}

	if ( pCamera && isOpenFade/* フェードイン中の操作制限のため */ )
	{
		pCamera->Update();

		if ( pCamera->IsExposure() && nextState == State::Null )
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

		if ( IS_TRG_UNDO && isOpenFade && nextState == State::Null )
		{
			if ( pStarMng->Undo(), pCamera->Undo()/* HAC:ちゃんと両方での成功を条件に取るべきである */ )
			{
				numMoves--;
				PlaySE( M_UNDO );
			}
		}
	}

	if ( pNumMoves )
	{
		pNumMoves->Update();
	}

	ShootingStarUpdate();

	BalloonUpdate();

#if DEBUG_MODE

	if ( TRG( KEY_INPUT_E ) && isOpenFade/* 連打防止 */ )
	{
		PlaySE( M_E_NEXT );

		nextState = State::Clear;
		isClearMoment = true;

		FadeEnd();	// フラグの初期化したいため
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
	clearTimer++;

	if ( pStarMng )
	{
		pStarMng->ClearUpdate();
	}

	if ( clearTimer < ClearRelated::FADE_WAIT )
	{
		BalloonUpdate();
	}


	if ( pBoard )
	{
		if ( ClearRelated::FADE_WAIT < clearTimer )
		{
			pBoard->Update();
		}
	}

	// RecordStarの生成管理
	{
		// HACK:星の数が３つじゃないなら，ここも変える必要がある
		const std::array<int, 3> generateFrames =
		{
			ClearRelated::FADE_WAIT + 50/* 基準 */,
			ClearRelated::FADE_WAIT + 50 + 30/* 間隔 */,
			ClearRelated::FADE_WAIT + 50 + ( 30 * 2 )
		};

		int nextGenerate = scast<int>( recordStars.size() );
		if	(
				nextGenerate < scast<int>( generateFrames.size() ) &&
				clearTimer == generateFrames[nextGenerate]	// 短絡評価
			)
		{
			Vector2 base{ 602.0f, 864.0f };
			float interval = scast<float>( ( 160 + ClearImage::SIZE_STAR_X ) * nextGenerate );
			base.x += interval;

			int nowRank = pNumMoves->CalcRank( numMoves );	// 0始まり
			// 達成難易度は，右からの降順で並んでいるとする（左のほうが達成されやすい）
			bool isGlow =
				( nextGenerate <= scast<int>( generateFrames.size() ) - 1 - nowRank )
				? true
				: false;

			recordStars.push_back( RecordStar() );
			recordStars.back().Init( base, isGlow );

			if ( isGlow )
			{
				PlaySE( M_RECORD_STAR );
			}
			else
			{
				PlaySE( M_UNRECORD_STAR );
			}
		}
	}
	for ( RecordStar &it : recordStars )
	{
		it.Update();
	}

	// 項目選択
	if ( isShowClearMenu )
	{
		// PauseUpdateのものと同一
		constexpr int MAX_MENU = 4;

		bool isUp = false, isDown = false;

		if ( IS_TRG_UP ) { isUp = true; }
		if ( IS_TRG_DOWN ) { isDown = true; }

		int lower = ( stageNumber == FileIO::GetMaxStageNumber() ) ? 1 : 0;
		if ( ( lower	< choice		) && isUp && !isDown ) { choice -= 1; PlaySE( M_SELECT ); }
		if ( ( choice	< MAX_MENU - 1	) && isDown && !isUp ) { choice += 1; PlaySE( M_SELECT ); }

		/*	// 上下を繋げる
		{
		if ( choice		< lower		) { choice = MAX_MENU - 1;	}
		if ( MAX_MENU	< choice	) { choice = lower;			}
		}
		*/

		assert( lower <= choice  && choice < MAX_MENU );

		if ( IS_TRG_EXPOSURE && nextState == State::Null )
		{
			PlaySE( M_DECISION );

			switch ( choice )
			{
			case 0:
				{
					if ( state == State::Clear )
					{
						nextState = State::Game;

						if ( FileIO::GetMaxStageNumber() <= stageNumber )
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

	// 「次へ」の移動
	if ( !isShowClearMenu && ClearRelated::FADE_WAIT + ClearRelated::GOTO_NEXT_WAIT < clearTimer )
	{
		const int   DESTINATION = SCREEN_WIDTH - ClearImage::SIZE_GOTO_NEXT_X;
		const float RESISTANCE = 0.3f;

		float dist = fabsf( scast<float>( DESTINATION - gotoNextPosX ) );
		gotoNextPosX -= scast<int>( dist * RESISTANCE );

		if ( gotoNextPosX < DESTINATION )
		{
			gotoNextPosX = DESTINATION;
		}

		if ( IS_TRG_EXPOSURE )
		{
			PlaySE( M_DECISION );

			isShowClearMenu = true;
			gotoNextPosX = SCREEN_WIDTH;
		}
	}

#if DEBUG_MODE

	if ( TRG( KEY_INPUT_E ) && isOpenFade/* 連打防止 */ )
	{
		PlaySE( M_E_NEXT );

		nextState = State::Select;
		FadeBegin();
	}

#endif // DEBUG_MODE

	ShakeUpdate();
}

void Game::ShootingStarUpdate()
{
	constexpr int AMOUNT = 5;
	sStarTimer += ( sStarState == 0 ) ? AMOUNT : -AMOUNT;

	constexpr int FLASH_INTERVAL = 120;
	if ( sStarTimer < 0 || FLASH_INTERVAL <= sStarTimer )
	{
		sStarState = ( sStarState == 0 ) ? 1 : 0;
	}
}

void Game::BalloonUpdate()
{
	textTimer++;

	// フキダシの更新
	if ( 0 != balloonLength )
	{
		constexpr int LOWER = 12;
		constexpr float DIV = 0.3f;

		if ( isOpenBalloon )	// ひらく
		{
			if ( balloonLength < HumanImage::SIZE_BALLOON_X )
			{
				float remaining = scast<float>( HumanImage::SIZE_BALLOON_X - balloonLength );

				if ( remaining <= LOWER )
				{
					balloonLength = HumanImage::SIZE_BALLOON_X;
				}
				else
				{
					balloonLength += scast<int>( remaining * DIV );

					if ( HumanImage::SIZE_BALLOON_X < balloonLength )
					{
						balloonLength = HumanImage::SIZE_BALLOON_X;
					}
				}
			}
		}
		else	// とじる
		{
			float remaining = scast<float>( balloonLength );

			if ( remaining <= LOWER )
			{
				balloonLength = 0;
			}
			else
			{
				balloonLength -= scast<int>( remaining * DIV );

				if ( balloonLength < 0 )
				{
					balloonLength = 0;
				}
			}
		}
	}

	// クリア台詞
	if ( nextState == State::Clear || state == State::Clear )
	{
		// 表示時間経過確認
		if ( 0 != textLength )
		{
			int showFrame = TextBehavior::CLEAR_SAY_SHOW_FRAME[textNumber];

			if ( showFrame <= textTimer )
			{
				textLength = 0;
				textExtendInterval = 0;

				isOpenBalloon = false;
			}
		}

		// 文字数増加確認
		if ( 0 != textLength && ( textLength * 2 ) <= scast<int>( TextBehavior::CLEAR_SAY[textNumber].size() ) )
		{
			constexpr int INTERVAL = 3;
			textExtendInterval++;
			if ( INTERVAL <= textExtendInterval )
			{
				textExtendInterval = 0;
				textLength++;

				// 口変化
				{
					if ( scast<int>( TextBehavior::CLEAR_SAY[textNumber].size() ) <= ( textLength * 2 ) )
					{
						mouthIndex = 0;
					}
					else
					{
						int oldIndex = mouthIndex;
						while ( oldIndex == mouthIndex )
						{
							mouthIndex = rand() % HumanImage::NUM_MOUTH_ROW;
						}
					}
				}

				PlaySE( M_VOICE );
			}
		}

		return;
	}
	// else

	// チュートリアル
	if ( stageNumber == 1 )
	{
		constexpr int TUTORIAL_TEXT_START_FRAME		= 80;
		constexpr int TUTORIAL_BALLOON_START_FRAME	= TUTORIAL_TEXT_START_FRAME - 20;
		if ( state == State::Game )	// クリア後は新しくは出さない
		{
			if ( textTimer == TUTORIAL_TEXT_START_FRAME )
			{
				textLength = 1;
			}
			if ( textTimer == TUTORIAL_BALLOON_START_FRAME )
			{
				constexpr int INIT_LENGTH = 64;
				balloonLength = INIT_LENGTH;
			}
		}

		// 表示時間経過確認
		if ( state == State::Game )
		{
			if ( textNumber < scast<int>( TextBehavior::TUTORIAL.size() ) - 1 )
			{
				int sumFrame = 0;
				for ( int i = 0; i <= textNumber; i++ )
				{
					sumFrame += TextBehavior::TUTORIAL_SHOW_FRAME[i];
				}

				if ( sumFrame <= textTimer - TUTORIAL_TEXT_START_FRAME )
				{
					textLength = 1;
					textExtendInterval = 0;

					textNumber++;
				}
			}
			else // else文にすると，すべて表示した後でないとリセットできないようになる
			if ( IS_TRG_SELECT )
			{
				textTimer = TUTORIAL_TEXT_START_FRAME + TextBehavior::TUTORIAL_SHOW_FRAME[0];

				textLength = 1;
				textExtendInterval = 0;

				textNumber = 1;
			}
		}

		// 文字数増加確認
		if ( 0 != textLength && ( textLength * 2 ) <= scast<int>( TextBehavior::TUTORIAL[textNumber].size() ) )
		{
			constexpr int INTERVAL = 3;
			textExtendInterval++;
			if ( INTERVAL <= textExtendInterval )
			{
				textExtendInterval = 0;
				textLength++;

				// 口変化
				{
					if ( scast<int>( TextBehavior::TUTORIAL[textNumber].size() ) <= ( textLength * 2 ) )
					{
						mouthIndex = 0;
					}
					else
					{
						int oldIndex = mouthIndex;
						while ( oldIndex == mouthIndex )
						{
							mouthIndex = rand() % HumanImage::NUM_MOUTH_ROW;
						}
					}
				}

				PlaySE( M_VOICE );
			}
		}

		return;
	}
	// else

	constexpr int SAY_INTERVAL	= 60 * 20/* 秒数 */;
	int remTimer = textTimer % SAY_INTERVAL;
	{
		constexpr int TEXT_START_FRAME		= 80;
		constexpr int BALLOON_START_FRAME	= TEXT_START_FRAME - 20;
		if ( state == State::Game )	// クリア後は新しくは出さない
		{
			if ( remTimer == TEXT_START_FRAME )
			{
				textLength = 1;
			}
			if ( remTimer == BALLOON_START_FRAME )
			{
				// 発言番号算出
				{
					int size = scast<int>( TextBehavior::RAND_SAY.size() );
					textNumber = rand() % size;
				}

				isOpenBalloon = true;

				constexpr int INIT_LENGTH = 64;
				balloonLength = INIT_LENGTH;
			}
		}

		// 表示時間経過確認
		if ( 0 != textLength )
		{
			int showFrame = TextBehavior::RAND_SAY_SHOW_FRAME[textNumber];

			if ( showFrame <= remTimer - TEXT_START_FRAME )
			{
				textLength = 0;
				textExtendInterval = 0;

				textNumber++;

				isOpenBalloon = false;
			}
		}

		// 文字数増加確認
		if ( 0 != textLength && ( textLength * 2 ) <= scast<int>( TextBehavior::RAND_SAY[textNumber].size() ) )
		{
			constexpr int INTERVAL = 3;
			textExtendInterval++;
			if ( INTERVAL <= textExtendInterval )
			{
				textExtendInterval = 0;
				textLength++;

				// 口変化
				{
					if ( scast<int>( TextBehavior::RAND_SAY[textNumber].size() ) <= ( textLength * 2 ) )
					{
						mouthIndex = 0;
					}
					else
					{
						int oldIndex = mouthIndex;
						while ( oldIndex == mouthIndex )
						{
							mouthIndex = rand() % HumanImage::NUM_MOUTH_ROW;
						}
					}
				}

				PlaySE( M_VOICE );
			}
		}
	}
}

void Game::FadeBegin()
{
	constexpr int MOVE_INTERVAL = 1;
	Vector2 pos = FadeImage::GetSize();
	pos *= -1;
	pos.x += scast<float>( SCREEN_WIDTH  ) * 0.2f/* 位置の調整 */;
	pos.y -= scast<float>( SCREEN_HEIGHT ) * 1.0f/* 位置の調整 */;

	Fade::GetInstance()->Init( MOVE_INTERVAL, pos );

	isOpenFade = false;
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
	// シーン遷移チェックは先にしているので，これに引っかかるはずはない想定
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

	// 「次のステージ」を選んだ場合
	if ( isShowClearMenu && state == State::Clear && nextState == State::Game )
	{
		if ( choice == 0 &&  stageNumber < FileIO::GetMaxStageNumber() )
		{
			stageNumber++;
		}
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

	isPause = false;
	isOpenFade = false;
}

void Game::FadeEnd()
{
	Fade::GetInstance()->Uninit();
	isOpenFade = true;
}

bool IsInsideStarFourCorners( Box camera, Box star )
{
	Vector2 corners[4] =
	{
		{ star.cx - star.w, star.cy - star.h },	// 左上
		{ star.cx + star.w, star.cy - star.h },	// 右上
		{ star.cx - star.w, star.cy + star.h },	// 左下
		{ star.cx + star.w, star.cy + star.h }	// 右下
	};

	for ( int i = 0; i < 4; i++ )
	{
		if ( !Box::IsHitPoint( camera, corners[i].x, corners[i].y ) )
		{
			return false;
		}
	}

	return true;
}
bool Game::Exposure()
{
	assert( pCamera );

	if ( !pStarMng  || !isOpenFade )
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
				pCamera->SetShake();
				PlaySE( M_FAILURE );

				return false;
			}
			// else
			if ( IsInsideStarFourCorners( camera, star ) )
			{
				targets.push_back( i );
			}
			else
			{
				pCamera->SetShake();
				PlaySE( M_FAILURE );

				return false;
			}
		}
	}

	// 適用対象がいなかったら終わる
	if ( !scast<int>( targets.size() ) || !end )
	{
		return false;
	}
	// else

	pCamera->SetGlow();

	// TODO:Exposureの音を鳴らすタイミングは，成功が確定してからかどうか。
	PlaySE( M_EXPOSURE );

	// アンドゥ用
	pStarMng->SaveLog();
	pCamera->SaveLog();

	// 適用
	for ( int i = 0; i < scast<int>( targets.size() ); i++ )
	{
		pStarMng->Expose( targets.at( i ) );
	}

	return true;
}

bool Game::IsInputPauseButton()
{
	if	(
			!IsTriggerPauseButton()	||
			state != State::Game	|| 
			!isOpenFade				||
			nextState != State::Null
		)
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
	// ポーズ文字
	{
		constexpr int INTERVAL	= 120;
		constexpr float AMPL	= 0.6f;
		pauseTimer++;
		pausePos.y += sinf( ( PI / INTERVAL ) * pauseTimer ) * AMPL;
	}

	// choice は 0 始まりである

	constexpr int MAX_MENU = 4;

	bool isUp = false, isDown = false;

	if ( IS_TRG_UP	 ) { isUp	= true; }
	if ( IS_TRG_DOWN ) { isDown	= true; }

	int lower = ( stageNumber == FileIO::GetMaxStageNumber() ) ? 1 : 0;
	if ( ( 0		< choice		)	&& isUp		&& !isDown	) { choice -= 1; PlaySE( M_SELECT ); }
	if ( ( choice	< MAX_MENU - 1	)	&& isDown	&& !isUp	) { choice += 1; PlaySE( M_SELECT ); }
	
	/*	// 上下を繋げる
	{
		if ( choice		< lower		) { choice = MAX_MENU - 1;	}
		if ( MAX_MENU	< choice	) { choice = lower;			}
	}
	*/

	assert( 0 <= choice  && choice < MAX_MENU );

	if ( IS_TRG_EXPOSURE && nextState == State::Null )
	{
		PlaySE( M_DECISION );

		switch ( choice )
		{
		case 0:
			{
				if ( state == State::Game )
				{
					isPause = false;
					return;
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

	isClearMoment = false;
	isTakeScreenShot = true;
}

#if DEBUG_MODE

void Game::DebugTakeStageScreenShot()
{
	std::string filename =
		"./Data/Images/Thumbnails/Stage"
		+ std::to_string( stageNumber ) + ".png";

	SaveDrawScreenToPNG
	(
		FRAME_POS_X,
		FRAME_POS_Y,
		FRAME_POS_X + FRAME_WIDTH,
		FRAME_POS_Y + FRAME_HEIGHT,
		filename.c_str()
	);
}

#endif // DEBUG_MODE

void Game::Draw()
{
	constexpr int PAUSE_BRIGHTNESS = 64;
	constexpr int CLEAR_BRIGHTNESS = 64;

	Vector2 shake = GetShakeAmount();

	if ( isPause ) { SetDrawBright( PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS ); }

	switch ( state )
	{
	case State::Select:
		SelectDraw();
		SelectDrawUI();		break;
	case State::Game:
		GameDraw();
		if ( isPause ) { SetDrawBright( PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS, PAUSE_BRIGHTNESS ); }
		GameDrawUI();		break;
	case State::Clear:
		ClearDraw();
		if ( isShowClearMenu ) { SetDrawBright( CLEAR_BRIGHTNESS, CLEAR_BRIGHTNESS, CLEAR_BRIGHTNESS ); }
		ClearDrawUI();		break;
	default:
		assert( !"Error:SceneGame state error." );
		exit( EXIT_FAILURE );
		return;
	}

	if ( pSSMng )
	{
		pSSMng->Draw( shake );
	}

	if ( isPause )
	{
		SetDrawBright( 255, 255, 255 );

		PauseDraw();
	}

	SetDrawBright( 255, 255, 255 );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );

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

	// 背景
	{
		// 仮置きなので，ExtendGraph
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

	// 背景
	{
		DrawGraph
		(
			0, 0,
			GameImage::hGameBG,
			TRUE
		);

		SetDrawBlendMode( DX_BLENDMODE_ADD, 255 );
		// 枠
		DrawGraph
		(
			0, 0,
			GameImage::hFrameBG,
			TRUE
		);
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );

		// 枠の画像
		DrawGraph
		(
			0, 0,
			GameImage::hFrameUI,
			TRUE
		);
	}

	// 流れ星
	{
		SetDrawArea
		(
			0, 0,
			FRAME_POS_X,
			SCREEN_HEIGHT
		);

		constexpr int POS_X = 0;
		constexpr int POS_Y = 200;

		constexpr int ENHANCE = 50;
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, sStarTimer + ENHANCE );
		DrawGraph
		(
			POS_X, POS_Y,
			MilkyWayImage::GetHandle( 0 ),
			TRUE
		);
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 - ( sStarTimer + ENHANCE ) );
		DrawGraph
		(
			POS_X, POS_Y,
			MilkyWayImage::GetHandle( 1 ),
			TRUE
		);
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );

		SetDrawArea
		(
			0, 0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT
		);
	}

	Grid::Draw( shake );

	if ( pStarMng )
	{
		pStarMng->Draw( shake );
	}

	// 他ＰＧによる作業
	if ( shutter_flag )
	{
		//シャッター(上から下)
		DrawRectGraph
		(
			DrawStart_X,
			DrawStart_Y,
			Src_X,
			Src_Y,
			scast<int>( Wid ),
			scast<int>( Hgh ),

			GameImage::hshutter,
			TRUE
		);

		//シャッター(下から上)
		DrawRectGraph
		(
			DrawStart_X,
			DrawStart_Y,
			Src_X,
			scast<int>( Src_Down_Y ),
			scast<int>( Wid ),
			scast<int>( Hgh_Down ),

			GameImage::hshutter,
			TRUE
		);
	}

#if DEBUG_MODE

	if ( TRG( KEY_INPUT_F9 ) && !isClearMoment )
	{
		DebugTakeStageScreenShot();
	}

#endif // DEBUG_MODE

	if ( isClearMoment )
	{
		TakeScreenShot();
	}

	if ( nextState == State::Null && pCamera )
	{
		pCamera->Draw( shake );
	}
	
	// 人
	{
		int animIndex = 0;

		// 体
		DrawGraph
		(
			0,
			SCREEN_HEIGHT - HumanImage::SIZE_Y,
			HumanImage::GetBodyHandle( animIndex ),
			TRUE
		);

		// 口
		DrawGraph
		(
			0,
			SCREEN_HEIGHT - HumanImage::SIZE_Y,
			HumanImage::GetMouthHandle( mouthIndex ),
			TRUE
		);

		// 腕
		DrawGraph
		(
			scast<int>( armPos.x ),
			scast<int>( armPos.y ),
			HumanImage::GetArmHandle( animIndex ),
			TRUE
		);
	}

	// フキダシ
	if ( 0 != balloonLength )
	{
		DrawExtendGraph
		(
			HumanBehavior::BALLOON_POS_X,
			HumanBehavior::BALLOON_POS_Y,
			HumanBehavior::BALLOON_POS_X + balloonLength,
			HumanBehavior::BALLOON_POS_Y + HumanImage::SIZE_BALLOON_Y,
			HumanImage::GetBalloonHandle(),
			TRUE
		);
	}
	// セリフ
	if ( 0 != textLength )
	{
		if ( nextState == State::Clear )	// クリア
		{
			int index = textNumber % scast<int>( TextBehavior::CLEAR_SAY.size() );
			int length = textLength * 2/* 日本語で２バイト文字なので，倍にして対応 */;
			if ( scast<int>( TextBehavior::CLEAR_SAY[index].size() ) <= textLength )
			{
				length = scast<int>( TextBehavior::CLEAR_SAY[index].size() );
			}

			constexpr int DIST_X = 80;
			constexpr int DIST_Y = 52;

			DrawExtendStringToHandle
			(
				HumanBehavior::BALLOON_POS_X + DIST_X,
				HumanBehavior::BALLOON_POS_Y + DIST_Y,
				2.0, 2.0,
				( TextBehavior::CLEAR_SAY[index].substr( 0, length ) ).c_str(),
				GetColor( 42, 97, 110 ),
				hFont
			);
		}
		else if ( stageNumber == 1 )	// チュートリアル
		{
			int index  = textNumber % scast<int>( TextBehavior::TUTORIAL.size() );
			int length = textLength * 2/* 日本語で２バイト文字なので，倍にして対応 */;
			if ( scast<int>( TextBehavior::TUTORIAL[index].size() ) <= textLength )
			{
				length = scast<int>( TextBehavior::TUTORIAL[index].size() );
			}

			constexpr int DIST_X = 80;
			constexpr int DIST_Y = 52;

			DrawExtendStringToHandle
			(
				HumanBehavior::BALLOON_POS_X + DIST_X,
				HumanBehavior::BALLOON_POS_Y + DIST_Y,
				2.0, 2.0,
				( TextBehavior::TUTORIAL[index].substr( 0, length ) ).c_str(),
				GetColor( 42, 97, 110 ),
				hFont
			);
		}
		else	// ランダム発言
		{
			int index  = textNumber % scast<int>( TextBehavior::RAND_SAY.size() );
			int length = textLength * 2/* 日本語で２バイト文字なので，倍にして対応 */;
			if ( scast<int>( TextBehavior::RAND_SAY[index].size() ) <= textLength )
			{
				length = scast<int>( TextBehavior::RAND_SAY[index].size() );
			}

			constexpr int DIST_X = 80;
			constexpr int DIST_Y = 52;

			DrawExtendStringToHandle
			(
				HumanBehavior::BALLOON_POS_X + DIST_X,
				HumanBehavior::BALLOON_POS_Y + DIST_Y,
				2.0, 2.0,
				( TextBehavior::RAND_SAY[index].substr( 0, length ) ).c_str(),
				GetColor( 42, 97, 110 ),
				hFont
			);
		}
	}
}

void Game::ClearDraw()
{
	Vector2 shake = GetShakeAmount();

	// 背景
	if ( !isShowClearMenu )
	{
		DrawGraph
		(
			0, 0,
			GameImage::hGameBG,
			TRUE
		);

		SetDrawBlendMode( DX_BLENDMODE_ADD, 255 );
		// 枠
		DrawGraph
		(
			0, 0,
			GameImage::hFrameBG,
			TRUE
		);
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );

		// 枠の画像
		DrawGraph
		(
			0, 0,
			GameImage::hFrameUI,
			TRUE
		);
	}

	if ( !isShowClearMenu && pStarMng )
	{
		pStarMng->Draw( shake );
	}

	// 人
	if ( !isShowClearMenu )
	{
		int animIndex = 0;

		// 体
		DrawGraph
		(
			0,
			SCREEN_HEIGHT - HumanImage::SIZE_Y,
			HumanImage::GetBodyHandle( animIndex ),
			TRUE
		);

		// 口
		DrawGraph
		(
			0,
			SCREEN_HEIGHT - HumanImage::SIZE_Y,
			HumanImage::GetMouthHandle( mouthIndex ),
			TRUE
		);

		// 腕
		DrawGraph
		(
			scast<int>( armPos.x ),
			scast<int>( armPos.y ),
			HumanImage::GetArmHandle( animIndex ),
			TRUE
		);
	}
	// フキダシ
	if ( 0 != balloonLength )
	{
		DrawExtendGraph
		(
			HumanBehavior::BALLOON_POS_X,
			HumanBehavior::BALLOON_POS_Y,
			HumanBehavior::BALLOON_POS_X + balloonLength,
			HumanBehavior::BALLOON_POS_Y + HumanImage::SIZE_BALLOON_Y,
			HumanImage::GetBalloonHandle(),
			TRUE
		);
	}
	// セリフ
	if ( 0 != textLength )
	{
		int index = textNumber % scast<int>( TextBehavior::CLEAR_SAY.size() );
		int length = textLength * 2/* 日本語で２バイト文字なので，倍にして対応 */;
		if ( scast<int>( TextBehavior::CLEAR_SAY[index].size() ) <= textLength )
		{
			length = scast<int>( TextBehavior::CLEAR_SAY[index].size() );
		}

		constexpr int DIST_X = 80;
		constexpr int DIST_Y = 52;

		DrawExtendStringToHandle
		(
			HumanBehavior::BALLOON_POS_X + DIST_X,
			HumanBehavior::BALLOON_POS_Y + DIST_Y,
			2.0, 2.0,
			( TextBehavior::CLEAR_SAY[index].substr( 0, length ) ).c_str(),
			GetColor( 42, 97, 110 ),
			hFont
		);
	}

	if ( isShowClearMenu )
	{
		SetDrawBright( 64, 64, 64 );
	}
	else if ( clearTimer < 255 )
	{
		int alpha = ( 255 / ClearRelated::FADE_WAIT )/* AMPL */ * clearTimer;

		SetDrawBlendMode( DX_BLENDMODE_ALPHA, alpha );
	}
	else
	{
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 );
	}

	// BG
	DrawGraph
	(
		0, 0,
		ClearImage::hClearBG,
		TRUE
	);
	// Statement
	DrawGraph
	(
		0, 0,
		ClearImage::hRecordStatement,
		TRUE
	);
	// Stage, Moves Number
	{
		const int STAGE_POS_X = 368;
		const int STAGE_POS_Y = 128;

		const int MOVES_POS_X = 288;
		const int MOVES_POS_Y = 508;

		const int STAGE_MAGNI_X = 2;
		const int STAGE_MAGNI_Y = 2;

		const int MOVES_MAGNI_X = 1;
		const int MOVES_MAGNI_Y = 1;

		int stgNum = stageNumber;
		// HACK:ステージ数は２ケタにおさまる想定である
		for ( int digit = 0; digit < 2; digit++ )
		{
			if ( stageNumber < 10 )
			{
				if ( digit != 0 )
				{
					continue;
				}
				// else

				DrawExtendGraph
				(
					STAGE_POS_X - Number::SIZE_X,
					STAGE_POS_Y,
					STAGE_POS_X - Number::SIZE_X + ( Number::SIZE_X * STAGE_MAGNI_X ),
					STAGE_POS_Y + ( Number::SIZE_Y * STAGE_MAGNI_Y ),
					Number::GetHandle( stgNum, true ),
					TRUE
				);

				continue;
			}
			// else

			DrawExtendGraph
			(
				STAGE_POS_X - ( Number::SIZE_X >> 1 ) - ( ( Number::SIZE_X + 8/* 字間 */ ) * digit ),
				STAGE_POS_Y,
				STAGE_POS_X - ( Number::SIZE_X >> 1 ) - ( ( Number::SIZE_X + 8/* 字間 */ ) * digit ) + ( Number::SIZE_X * STAGE_MAGNI_X ),
				STAGE_POS_Y + ( Number::SIZE_Y * STAGE_MAGNI_Y ),
				Number::GetHandle( stgNum % 10, true ),
				TRUE
			);

			stgNum /= 10;
		}
		// NumberOfMoves
		int movNum = numMoves;
		for ( int digit = 0; digit < 2; digit++ )
		{
			if ( numMoves < 10 )
			{
				if ( digit != 0 )
				{
					continue;
				}
				// else

				DrawExtendGraph
				(
					MOVES_POS_X - Number::SIZE_X,
					MOVES_POS_Y,
					MOVES_POS_X - Number::SIZE_X + ( Number::SIZE_X * MOVES_MAGNI_X ),
					MOVES_POS_Y + ( Number::SIZE_Y * MOVES_MAGNI_Y ),
					Number::GetHandle( movNum % 10, true ),
					TRUE
				);

				continue;
			}
			// else

			DrawExtendGraph
			(
				MOVES_POS_X - ( Number::SIZE_X >> 1 ) - ( ( ( Number::SIZE_X >> 1 ) + 8/* 字間 */ ) * digit ),
				MOVES_POS_Y,
				MOVES_POS_X - ( Number::SIZE_X >> 1 ) - ( ( ( Number::SIZE_X >> 1 ) + 8/* 字間 */ ) * digit ) + ( Number::SIZE_X * MOVES_MAGNI_X ),
				MOVES_POS_Y + ( Number::SIZE_Y * MOVES_MAGNI_Y ),
				Number::GetHandle( movNum % 10, true ),
				TRUE
			);

			movNum /= 10;
		}
	}
	// 手数何以内
	if ( pNumMoves )
	{
		int x = 448;
		int y = 640;
		// GetMousePoint( &x, &y );

		int textLength = 352;
		std::vector<int> data = pNumMoves->GetData();
		for ( int i = 0; i < 3; i++ )
		{
			DrawExtendFormatStringToHandle
			(
				x + ( textLength  * i ),
				y,
				3.0, 3.0,
				GetColor( 124, 246, 255 ),
				hFont,
				"手数 %d 以内",
				data.at( 2 - i )
			);
		}
	}

	// 背景としての，暗い星
	{
		// HACK:星の数が３つじゃないなら，ここも変える必要がある

		int i = scast<int>( recordStars.size() );
		for ( ; i < 3; i++ )
		{
			// ClearUpdate()内のものと同一

			Vector2 base{ 602.0f, 864.0f };
			float interval = scast<float>( ( 160 + ClearImage::SIZE_STAR_X ) * i );
			base.x += interval;

			DrawRotaGraph
			(
				scast<int>( base.x ),
				scast<int>( base.y ),
				1.0,
				ToRadian( 0 ),
				ClearImage::hRecordStar[0],
				TRUE
			);
		}
	}

	if ( isShowClearMenu )
	{
		SetDrawBright( 128, 128, 128 );
	}
	else
	{
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
	}

	if ( clearTimer < ClearRelated::FADE_WAIT )
	{
		return;
	}
	// else

	if ( pBoard )
	{
		pBoard->Draw( hScreenShot, shake );
	}

	for ( const RecordStar &it : recordStars )
	{
		it.Draw( shake );
	}

	// 「次へ」表示
	{
		constexpr int POS_Y = 960;

		DrawGraph
		(
			gotoNextPosX,
			POS_Y,
			ClearImage::hGotoNext,
			TRUE
		);
	}

	SetDrawBright( 255, 255, 255 );

	if ( !isShowClearMenu )
	{
		return;
	}
	// else

	bool isFinalStage =
		( stageNumber == FileIO::GetMaxStageNumber() )
		? true
		: false;

	// 項目
	DrawGraph
	(
		608,
		288,
		ClearImage::GetStatementHandle( 0, isFinalStage ),
		TRUE
	);
	// 強調
	DrawGraph
	(
		608,
		288,
		ClearImage::GetStatementHandle( choice + 1, isFinalStage ),
		TRUE
	);
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

	// ポーズ
	DrawGraph
	(
		scast<int>( pausePos.x ),
		scast<int>( pausePos.y ),
		PauseImage::GetHandle(),
		TRUE
	);

	// 項目
	DrawGraph
	(
		608,
		288,
		PauseImage::GetMenuHandle( 0 ),
		TRUE
	);
	// 強調
	DrawGraph
	(
		608,
		288,
		PauseImage::GetMenuHandle( choice + 1 ),
		TRUE
	);
}

void Game::SelectDrawUI()
{

}

void Game::GameDrawUI()
{
	// 手数
	{
		DrawGraph
		(
			FRAME_POS_X - 160,
			FRAME_POS_Y,
			GameImage::hMovesUI,
			TRUE
		);

		// 数字
		{
			const int MOVES_POS_X = 112;
			const int MOVES_POS_Y = 144;

			const int MOVES_MAGNI_X = 1;
			const int MOVES_MAGNI_Y = 1;

			int movNum = numMoves;
			for ( int digit = 0; digit < 2; digit++ )
			{
				if ( numMoves < 10 )
				{
					if ( digit != 0 )
					{
						continue;
					}
					// else

					DrawExtendGraph
					(
						MOVES_POS_X - ( Number::SIZE_X >> 1 ),
						MOVES_POS_Y,
						MOVES_POS_X - ( Number::SIZE_X >> 1 ) + ( Number::SIZE_X * MOVES_MAGNI_X ),
						MOVES_POS_Y + ( Number::SIZE_Y * MOVES_MAGNI_Y ),
						Number::GetHandle( movNum % 10, true ),
						TRUE
					);

					continue;
				}
				// else

				DrawExtendGraph
				(
					MOVES_POS_X - ( Number::SIZE_X >> 1 ) - ( ( ( Number::SIZE_X >> 1 ) + 8/* 字間 */ ) * digit ),
					MOVES_POS_Y,
					MOVES_POS_X - ( Number::SIZE_X >> 1 ) - ( ( ( Number::SIZE_X >> 1 ) + 8/* 字間 */ ) * digit ) + ( Number::SIZE_X * MOVES_MAGNI_X ),
					MOVES_POS_Y + ( Number::SIZE_Y * MOVES_MAGNI_Y ),
					Number::GetHandle( movNum % 10, true ),
					TRUE
				);

				movNum /= 10;
			}
		}
	}

	// 星レベルの表示
	{
		const Vector2 BASE{ scast<float>( FRAME_POS_X + FRAME_WIDTH ),scast<float>( FRAME_POS_Y ) };
		const Vector2 TWEAK{ 64.0f, 0 };
		const Vector2 HALF_SIZE{ Grid::GetSize().x * 0.5f , Grid::GetSize().y * 0.5f };

		// 段階の矢印
		DrawGraph
		(
			scast<int>( BASE.x ),
			scast<int>( BASE.y ),
			StarImage::GetGradeHandle(),
			TRUE
		);

		for ( int i = 1; i <= Star::MAX_LEVEL; i++ )
		{
			double angle = ( i % 2 ) ? 0 : 45.0;

			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
			DrawRotaGraph
			(
				scast<int>( BASE.x + TWEAK.x + HALF_SIZE.x ),									// 中心座標
				scast<int>( BASE.y + TWEAK.y + HALF_SIZE.y ) + ( ( i - 1 ) * StarImage::SIZE ),	// 中心座標
				1.0, ToRadian( angle ),
				StarImage::GetHandle( i, 0 ),
				TRUE
			);
			SetDrawBlendMode( DX_BLENDMODE_ADD, 255 );
			DrawRotaGraph
			(
				scast<int>( BASE.x + TWEAK.x + HALF_SIZE.x ),									// 中心座標
				scast<int>( BASE.y + TWEAK.y + HALF_SIZE.y ) + ( ( i - 1 ) * StarImage::SIZE ),	// 中心座標
				1.0, ToRadian( angle ),
				StarImage::GetHandle( i, 0 ),
				TRUE
			);
		}
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
	}
}

void Game::ClearDrawUI()
{
	
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