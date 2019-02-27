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


		hshutter	= LoadGraph( "./Data/Images/Camera/Shutter.png" );


	}
	void Release()
	{
		DeleteGraph( hGameBG	);
		DeleteGraph( hFrameBG	);
		DeleteGraph( hFrameUI	);
		hGameBG		= 0;
		hFrameBG	= 0;
		hFrameUI	= 0;


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

namespace PauseImage
{
	constexpr int SIZE_X = 736;
	constexpr int SIZE_Y = 576;

	constexpr int NUM_ROW = 5;

	// 0:All, 1:ToGame, 2:Retry, 3:ToSelect, 4:ToTitle
	static int hPauseStatements[NUM_ROW];

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

namespace HumanImage
{
	constexpr int NUM_ROW	 = 6;
	constexpr int NUM_COLUMN = 3;
	constexpr int NUM_ALL = NUM_ROW * NUM_COLUMN;

	constexpr int SIZE_X = 320;// 256
	constexpr int SIZE_Y = 360;// 352

	static int hBody[NUM_ALL];
	static int hArm[NUM_ALL];

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( 0 != hBody[0] )
		{
			return;
		}
		// else

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
	}
	void Release()
	{
		for ( int i = 0; i < NUM_ALL; i++ )
		{
			DeleteGraph( hBody[i] );
			hBody[i] = 0;
		}
	}

	int  GetBodyHandle( int index )
	{
		assert( 0 <= index && index < NUM_ALL );

		return hBody[index];
	}
	int  GetArmHandle( int index )
	{
		assert( 0 <= index && index < NUM_ALL );

		return hArm[index];
	}
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
	int x = 0 , y = 0;
	GetMousePoint( &x, &y );

	int r = DrawRotaGraph
	(
		scast<int>( pos.x ),
		scast<int>( pos.y ),
		scale,
		ToRadian( angle ),
		ClearImage::hRecordStar[( isGlow ) ? 1 : 0],
		TRUE
	);
	r++;
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
	ClearImage::Load();
	PauseImage::Load();
	HumanImage::Load();

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
	numMoves = 0;

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

	armPos = { 0, scast<float>( SCREEN_HEIGHT - HumanImage::SIZE_Y ) };
}
void Game::ClearInit()
{
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

	CameraImage::Release();
	StarImage::Release();
	StageImage::Release();
	CursorImage::Release();
	BoardImage::Release();

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


	// シャッター演出
	{
		if ( shutter_flag == true )
		{
			switch ( shutter_state )
			{
			case 0:
				str_up_pos.y += str_speed.y;
				str_down_pos.y -= str_speed.y;
				if ( str_up_pos.y >= 768 >> 1 )
				{
					shutter_state++;
				}
				break;

			case 1:
				str_up_pos.y -= str_speed.y;
				str_down_pos.y += str_speed.y;
				if ( str_up_pos.y < 64 - 768 )
				{
					shutter_flag = false;
					shutter_state = 0;
					str_up_pos.y = str_up_pos.y - 768.0f;
					str_down_pos.y = str_down_pos.y + 768.0f;

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

		// FadeBegin();
		shutter_flag = true;
		isOpenFade = false;
	}

	// カメラの更新より先に判定し，描画後にスクショが始まるようにする
	if ( nextState == State::Null && pStarMng->IsEqualLevels() )
	{
		PlaySE( M_E_NEXT );

		nextState = State::Clear;
		isClearMoment = true;

		FadeEnd();	// フラグの初期化したいため
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

			PlaySE( M_RECORD_STAR );
		}
	}
	for ( RecordStar &it : recordStars )
	{
		it.Update();
	}

	if ( isShowClearMenu )
	{
		// PauseUpdateのものと同一
		constexpr int MAX_MENU = 4;

		bool isUp = false, isDown = false;

		if ( IS_TRG_UP ) { isUp = true; }
		if ( IS_TRG_DOWN ) { isDown = true; }

		int lower = ( stageNumber == FileIO::GetMaxStageNumber() ) ? 1 : 0;
		if ( ( lower	< choice )		 && isUp	&& !isDown	) { choice -= 1; PlaySE( M_E_NEXT ); }
		if ( ( choice	< MAX_MENU - 1 ) && isDown	&& !isUp	) { choice += 1; PlaySE( M_E_NEXT ); }

		assert( lower	<= choice  && choice < MAX_MENU );

		if ( IS_TRG_EXPOSURE && nextState == State::Null )
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

	// 「次へ」の移動
	if ( !isShowClearMenu && ClearRelated::FADE_WAIT + ClearRelated::GOTO_NEXT_WAIT < clearTimer )
	{
		constexpr int SPD = -5;
		gotoNextPosX += SPD;	// 等速

		const int DESTINATION = SCREEN_WIDTH - ClearImage::SIZE_GOTO_NEXT_X;
		if ( gotoNextPosX < DESTINATION )
		{
			gotoNextPosX = DESTINATION;
		}

		if ( IS_TRG_EXPOSURE )
		{
			PlaySE( M_E_NEXT );

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

	// TODO:Exposureの音を鳴らすタイミングは，成功が確定してからかどうか。
	PlaySE( M_EXPOSURE );

	// アンドゥ用
	pStarMng->SaveLog();

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

	isClearMoment = false;
	isTakeScreenShot = true;
}

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

		// 他ＰＧによる作業
		if ( shutter_flag )
		{
			constexpr int MAX_SIZE_Y = 768;

			/*
			// 上から下
			{
				int size = scast<int>( str_up_pos.y + MAX_SIZE_Y ) - FRAME_POS_Y;

				if ( 0 < size )
				{
					DrawRectGraph
					(
						FRAME_POS_X, FRAME_POS_Y,
						0, 0,
						FRAME_WIDTH,
						size,
						GameImage::hshutter,
						TRUE
					);
				}
			}
			// 下から上
			{
				int size = FRAME_POS_Y - scast<int>( str_down_pos.y );

				if ( 0 < size )
				{
					DrawRectGraph
					(
						FRAME_POS_X, FRAME_POS_Y - size,
						0, 0,
						FRAME_WIDTH,
						size,
						GameImage::hshutter,
						TRUE
					);
				}
			}
			*/

			//シャッター(上から下)
			DrawGraph
			(
				scast<int>( str_up_pos.x ),
				scast<int>( str_up_pos.y - 768.0f ),
				GameImage::hshutter,
				TRUE
			);

			//シャッター(下から上)
			DrawGraph
			(
				scast<int>( str_down_pos.x ),
				scast<int>( str_down_pos.y + 768.0f ),
				GameImage::hshutter,
				TRUE
			);
		}

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


	Grid::Draw( shake );

	if ( pStarMng )
	{
		pStarMng->Draw( shake );
	}

	if ( isClearMoment )
	{
		TakeScreenShot();
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

		// 腕
		DrawGraph
		(
			scast<int>( armPos.x ),
			scast<int>( armPos.y ),
			HumanImage::GetArmHandle( animIndex ),
			TRUE
		);
	}

	if ( nextState == State::Null && pCamera )
	{
		pCamera->Draw( shake );
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

		// 腕
		DrawGraph
		(
			scast<int>( armPos.x ),
			scast<int>( armPos.y ),
			HumanImage::GetArmHandle( animIndex ),
			TRUE
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
		const int STAGE_POS_Y = 144;

		const int MOVES_POS_X = 288;
		const int MOVES_POS_Y = 492;

		const int STAGE_MAGNI_X = 2;
		const int STAGE_MAGNI_Y = 2;

		const int MOVES_MAGNI_X = 1;
		const int MOVES_MAGNI_Y = 1;

		int stgNum = stageNumber;
		int movNum = numMoves;
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

				// StageNumber
				DrawExtendGraph
				(
					STAGE_POS_X - Number::SIZE_X,
					STAGE_POS_Y,
					STAGE_POS_X - Number::SIZE_X + ( Number::SIZE_X * STAGE_MAGNI_X ),
					STAGE_POS_Y + ( Number::SIZE_Y * STAGE_MAGNI_Y ),
					Number::GetHandle( stgNum, true ),
					TRUE
				);

				// MovesNumber
				DrawExtendGraph
				(
					MOVES_POS_X - Number::SIZE_X,
					MOVES_POS_Y,
					MOVES_POS_X - Number::SIZE_X + ( Number::SIZE_X * MOVES_MAGNI_X ),
					MOVES_POS_Y + ( Number::SIZE_Y * MOVES_MAGNI_Y ),
					Number::GetHandle( movNum, true ),
					TRUE
				);

				continue;
			}
			// else

			// StageNumber
			DrawExtendGraph
			(
				STAGE_POS_X - ( Number::SIZE_X >> 1 ) - ( Number::SIZE_X * digit ),
				STAGE_POS_Y,
				STAGE_POS_X - ( Number::SIZE_X >> 1 ) - ( Number::SIZE_X * digit ) + ( Number::SIZE_X * STAGE_MAGNI_X ),
				STAGE_POS_Y + ( Number::SIZE_Y * STAGE_MAGNI_Y ),
				Number::GetHandle( stgNum % 10, true ),
				TRUE
			);

			// MovesNumber
			DrawExtendGraph
			(
				MOVES_POS_X - Number::SIZE_X,
				MOVES_POS_Y,
				MOVES_POS_X - Number::SIZE_X + ( Number::SIZE_X * MOVES_MAGNI_X ),
				MOVES_POS_Y + ( Number::SIZE_Y * MOVES_MAGNI_Y ),
				Number::GetHandle( movNum % 10, true ),
				TRUE
			);

			stgNum /= 10;
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

		DrawExtendStringToHandle
		(
			gotoNextPosX,
			POS_Y,
			3.0, 3.0,
			"次へ",
			GetColor( 124, 246, 255 ),
			hFont
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

	// 項目
	DrawGraph
	(
		608,
		288,
		PauseImage::GetHandle( 0 ),
		TRUE
	);
	// 強調
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
	// 手数
	{
		DrawExtendFormatString
		(
			10, 10,
			2.0, 2.0,
			GetColor( 200, 200, 200 ),
			"今の手数：%d", numMoves
		);
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
	/*
	DrawExtendFormatStringToHandle
	(
		300, 300,
		6.0, 6.0,
		GetColor( 32, 32, 32 ),
		hFont,
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

		DrawExtendStringToHandle
		(
			360, 400,
			6.0, 6.0,
			result.c_str(),
			colours[nowRank],
			hFont
		);
	}
	*/
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