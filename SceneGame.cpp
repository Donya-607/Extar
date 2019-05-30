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

#undef min
#undef max

namespace
{
	constexpr int PARTICLE_AMOUNT = 6;
	constexpr int SS_PROBABILITY  = 128;
}

namespace TextBehavior
{
	// ���̐��ƃt���[���̐��͑����Ă�������

	const std::vector<std::string> TUTORIAL =
	{
		"����ف[�A���S�҂���H�@����������ˁ�",
		"���F�̘g���J�����͈̔͂���I",
		"�͈͓��ɐ��������߂�ƁA�y�L�[�ŘI�����g�����",

		"�I���́A�͈͓��ɂ��鐯�𖾂邭�ł����",
		"���ׂĂ̐��̖��邳�����낦�悤��",
		"�G���^�[�L�[�ł�����x�������I"
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
		"���F�̘g",
		"�I��",
		"���ׂ�",
		"�G���^�[�L�["
	};

	const std::vector<std::string> RAND_SAY =
	{
		"�t�@�C�g�H�I",
		"��[�Y�ނˁ[",
		"����Ȃ��E�E�E",

		"���̒��q�@���̒��q�I",
		"����΂�[�I",
		"��[�A�ǂ�����񂾂낤�E�E�E",

		"�����Ȃ��E�E�E",
		"��������������",
		"�������ꂢ���ˁ[",

		"�L���b��"
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
		"�ǂ��ˁI",
		"���߂łƂ��I",
		"�i�C�X�I",
		"������ˁI"
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
	static int hStatement;
	static int hUsage;

	void Load()
	{
		// ���łɒl�������Ă�����C�ǂݍ��񂾂��̂Ƃ݂Ȃ��Ĕ�΂�
		if ( 0 != hSelectBG )
		{
			return;
		}
		// else

		hSelectBG	= LoadGraph( "./Data/Images/BG/Select.png" );
		hStatement	= LoadGraph( "./Data/Images/Select/Statement.png" );
		hUsage		= LoadGraph( "./Data/Images/Select/Usage.png" );
	}
	void Release()
	{
		DeleteGraph( hSelectBG );
		DeleteGraph( hStatement );
		DeleteGraph( hUsage );
		hSelectBG = 0;
		hStatement = 0;
		hUsage = 0;
	}
}
namespace GameImage
{
	static int hGameBG;
	static int hFrameBG;
	static int hFrameUI;
	//static int hMovesUI;


	static int hshutter;


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
		//hMovesUI	= LoadGraph( "./Data/Images/UI/NumberOfMoves.png" );


		hshutter	= LoadGraph( "./Data/Images/Camera/Shutter.png" );


	}
	void Release()
	{
		DeleteGraph( hGameBG	);
		DeleteGraph( hFrameBG	);
		DeleteGraph( hFrameUI	);
		//DeleteGraph( hMovesUI );
		hGameBG		= 0;
		hFrameBG	= 0;
		hFrameUI	= 0;
		//hMovesUI	= 0;


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
		// ���łɒl�������Ă�����C�ǂݍ��񂾂��̂Ƃ݂Ȃ��Ĕ�΂�
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
	static int hMilkyWay[2];

	void Load()
	{
		// ���łɒl�������Ă�����C�ǂݍ��񂾂��̂Ƃ݂Ȃ��Ĕ�΂�
		if ( 0 != hMilkyWay[0] )
		{
			return;
		}
		// else

		hMilkyWay[0] = LoadGraph( "./Data/Images/Title/TitleStar1.png" );
		hMilkyWay[1] = LoadGraph( "./Data/Images/Title/TitleStar2.png" );
	}
	void Release()
	{
		DeleteGraph( hMilkyWay[0] );
		DeleteGraph( hMilkyWay[1] );
		hMilkyWay[0] = 0;
		hMilkyWay[1] = 0;
	}

	int  GetHandle( int index )
	{
		assert( 0 <= index && index <= 1 );

		return hMilkyWay[index];
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
	constexpr int NUM_BODY = 4;

	constexpr int SIZE_X = 256;
	constexpr int SIZE_Y = 352;

	static int hBody[NUM_BODY];
	static int hArm;

	constexpr int NUM_MOUTH_ROW = 3;
	constexpr int SIZE_MOUTH_X  = 256;
	constexpr int SIZE_MOUTH_Y  = 352;
	static int hMouth[NUM_MOUTH_ROW];

	constexpr int SIZE_BALLOON_X = 928;
	constexpr int SIZE_BALLOON_Y = 160;

	static int hBalloon;	// �t�L�_�V

	void Load()
	{
		// ���łɒl�������Ă�����C�ǂݍ��񂾂��̂Ƃ݂Ȃ��Ĕ�΂�
		if ( 0 != hBody[0] ) { return; }
		// else

		
		LoadDivGraph
		(
			"./Data/Images/Human/Body.png",
			NUM_BODY,
			NUM_BODY, 1,
			SIZE_X, SIZE_Y,
			hBody
		);
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
		for ( int i = 0; i < NUM_BODY; i++ )
		{
			DeleteGraph( hBody[i] );
			hBody[i] = 0;
		}
		DeleteGraph( hArm );
		hArm = 0;
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
		if ( index < 0 || NUM_BODY <= index ) { return NULL; }
		// else

		return hBody[index];
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
	timer++;

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

void Game::Wink::Init()
{
	Lottering();
}
void Game::Wink::Uninit()
{
	waitFrame = -1;
}

void Game::Wink::Update()
{
	Wait();
	Animation();
}

void Game::Wink::Lottering()
{
	constexpr int MIN_SEC = 1;
	waitFrame = rand() % 3;
	waitFrame += MIN_SEC;

	waitFrame *= 60;
}

void Game::Wink::Wait()
{
	if ( 0 < waitFrame )
	{
		waitFrame--;
		if ( waitFrame == 0 )
		{
			timer = ANIME_SPD;
		}
		return;
	}
}
void Game::Wink::Animation()
{
	if ( waitFrame != 0 ) { return; }
	// else

	timer--;
	if ( timer <= 0 )
	{
		if ( animIndex < HumanImage::NUM_BODY - 1 )
		{
			animIndex++;
			timer = ANIME_SPD;
		}
		else
		{
			animIndex = 0;
			Lottering();
		}
	}
}

void Game::Init()
{
	FileIO::ReadAllCamera();
	FileIO::ReadAllStars();
	FileIO::ReadAllNumMoves();

	Number::Load();

	ParticleImage::Load();

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
		"JF�h�b�gK12",
		-1,				// �T�C�Y�i-1�F�f�t�H���g�j
		-1,				// �����i-1�F�f�t�H���g�j
		DX_FONTTYPE_NORMAL
	);

	pSSMng.reset( new ShootingStarMng() );
	pSSMng->Init();

	pParticleMng.reset( new ParticleMng() );
	pParticleMng->Init();

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

	selectTimer	= 0;
	selectPos	= { 710.0f, 40.0f };

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

	wink.Init();

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

	// ���̂o�f�̍��
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

	ParticleImage::Release();

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
	hFont = 0;

	if ( pSSMng ) { pSSMng->Uninit(); }
	if ( pParticleMng ) { pParticleMng->Uninit(); }

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

	wink.Uninit();

	DeleteGraph( hScreenShot );
	hScreenShot = 0;

	if ( pStarMng  ) { pStarMng->Uninit();  }
	if ( pNumMoves ) { pNumMoves->Uninit(); }
	if ( pBoard	   ) { pBoard->Uninit();	}

	recordStars.clear();

	// �B��
	armPos = { scast<float>( SCREEN_WIDTH ), scast<float>( SCREEN_HEIGHT ) };
}

void Game::Update()
{
#if DEBUG_MODE

	if ( TRG( KEY_INPUT_C ) )
	{
		char debugstoper = 0;
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

	if ( pParticleMng )
	{
		pParticleMng->Update();
	}

	// �V���b�^�[���o
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
					isOpenFade = true;
				}
				break;

			}
		}
	}

#if DEBUG_MODE

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
	// �Z���N�g����
	{
		constexpr int INTERVAL = 120;
		constexpr float AMPL = 0.6f;
		selectTimer++;
		selectPos.y += sinf( ( PI / INTERVAL ) * selectTimer ) * AMPL;
	}

	if ( pSSMng )
	{
		// ����
		if ( isOpenFade )	// DEBUG
		{
			// constexpr int PROBABILITY = 12;// 256;
			if ( !( rand() % SS_PROBABILITY ) )
			{
				const std::vector<float> POS_X =
				{
					256.0f * 1,
					256.0f * 2,
					256.0f * 3,
					256.0f * 4,
					256.0f * 5,
					256.0f * 6,
					256.0f * 7,
					256.0f * 8
				};
				const std::vector<float> POS_Y =
				{
					0
				};

				float x = POS_X[rand() % scast<int>( POS_X.size() )];
				float y = POS_Y[rand() % scast<int>( POS_Y.size() )];

				pSSMng->GenerateStar( { x, y } );
			}
		}

		pSSMng->Update();
	}

	if ( pCursor )
	{
		pCursor->Update( isOpenFade/* �t�F�[�h�C�����̑��쐧���̂��� */ );

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

	// �J�����̍X�V����ɔ��肵�C�`���ɃX�N�V�����n�܂�悤�ɂ���
	if ( !isDoneMoveArm && pStarMng->IsEqualLevels() )
	{
		if ( nextState == State::Null )
		{
			/* �e�L�X�g�{�b�N�X���B��
			{
				mouthIndex = 0;
				textLength = 0;
				textExtendInterval = 0;

				isOpenBalloon = false;
			}
			*/

			// �N���A�䎌���o��
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

			isOpenFade = false;	// ����s�\�ɂ��邽��
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

	if ( pCamera && isOpenFade/* �t�F�[�h�C�����̑��쐧���̂��� */ )
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

	if ( pSSMng )
	{
		// ����
		if (  isOpenFade )	// DEBUG
		{
			// constexpr int PROBABILITY = 12;// 256;
			if ( !( rand() % SS_PROBABILITY ) )
			{
				const std::vector<Vector2> POS =
				{
					{ 256.0f, 0 },
					{ 384.0f, 0 },
					{ 1920.0f, 640.0f },
					{ 1920.0f, 768.0f },
				};

				Vector2 pos = POS[rand() % scast<int>( POS.size() )];

				pSSMng->GenerateStar( pos );
			}
		}

		pSSMng->Update();
	}

	if ( pStarMng )
	{
		pStarMng->Update();

		if ( IS_TRG_UNDO && isOpenFade && nextState == State::Null )
		{
			if ( pStarMng->Undo(), pCamera->Undo()/* HAC:�����Ɨ����ł̐����������Ɏ��ׂ��ł��� */ )
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

	wink.Update();

	MilkyWayUpdate();

	BalloonUpdate();

#if DEBUG_MODE

	if ( TRG( KEY_INPUT_E ) && isOpenFade/* �A�Ŗh�~ */ )
	{
		PlaySE( M_E_NEXT );

		nextState = State::Clear;
		isClearMoment = true;

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
	clearTimer++;

	if ( pStarMng )
	{
		pStarMng->ClearUpdate();
	}

	if ( clearTimer < ClearRelated::FADE_WAIT )
	{
		BalloonUpdate();
	}

	wink.Update();

	if ( pSSMng )
	{
		// ����
		if ( isOpenFade )	// DEBUG
		{
			// constexpr int PROBABILITY = 12;// 256;
			if ( !( rand() % SS_PROBABILITY ) )
			{
				const std::vector<float> POS_X =
				{
					256.0f * 1,
					256.0f * 2,
					256.0f * 3,
					256.0f * 4,
					256.0f * 5,
					256.0f * 6,
					256.0f * 7,
					256.0f * 8,
				};
				const std::vector<float> POS_Y =
				{
					0
				};

				float x = POS_X[rand() % scast<int>( POS_X.size() )];
				float y = POS_Y[rand() % scast<int>( POS_Y.size() )];

				pSSMng->GenerateStar( { x, y } );
			}
		}

		pSSMng->Update();
	}

	if ( pBoard )
	{
		if ( ClearRelated::FADE_WAIT < clearTimer )
		{
			pBoard->Update();
		}
	}

	// RecordStar�̐����Ǘ�
	{
		// HACK:���̐����R����Ȃ��Ȃ�C�������ς���K�v������
		const std::array<int, 3> generateFrames =
		{
			ClearRelated::FADE_WAIT + 50/* � */,
			ClearRelated::FADE_WAIT + 50 + 30/* �Ԋu */,
			ClearRelated::FADE_WAIT + 50 + ( 30 * 2 )
		};

		int nextGenerate = scast<int>( recordStars.size() );
		if	(
				nextGenerate < scast<int>( generateFrames.size() ) &&
				clearTimer == generateFrames[nextGenerate]	// �Z���]��
			)
		{
			Vector2 base{ 602.0f, 864.0f };
			float interval = scast<float>( ( 160 + ClearImage::SIZE_STAR_X ) * nextGenerate );
			base.x += interval;

			int nowRank = pNumMoves->CalcRank( numMoves );	// 0�n�܂�
			// �B����Փx�́C�E����̍~���ŕ���ł���Ƃ���i���̂ق����B������₷���j
			bool isGlow =
				( nextGenerate <= scast<int>( generateFrames.size() ) - 1 - nowRank )
				? true
				: false;

			recordStars.push_back( RecordStar() );
			recordStars.back().Init( base, isGlow );

			if ( isGlow )
			{
				StartJoypadVibration( DX_INPUT_PAD1, 450, 300 );
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

		constexpr int GENERATE_PARTICLE_TIME = 8;
		if ( it.GetTimer() == GENERATE_PARTICLE_TIME )
		{
			if ( pParticleMng )
			{
				pParticleMng->Generate( PARTICLE_AMOUNT, it.GetPos(), /* isBig = */true );
			}
		}
	}

	// ���ڑI��
	if ( isShowClearMenu )
	{
		// PauseUpdate�̂��̂Ɠ���
		constexpr int MAX_MENU = 4;

		bool isUp = false, isDown = false;

		if ( IS_TRG_UP ) { isUp = true; }
		if ( IS_TRG_DOWN ) { isDown = true; }

		int lower = ( stageNumber == FileIO::GetMaxStageNumber() ) ? 1 : 0;
		if ( ( lower	< choice		) && isUp && !isDown ) { choice -= 1; PlaySE( M_SELECT ); }
		if ( ( choice	< MAX_MENU - 1	) && isDown && !isUp ) { choice += 1; PlaySE( M_SELECT ); }

		/*	// �㉺���q����
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

	// �u���ցv�̈ړ�
	if ( !isShowClearMenu && ClearRelated::FADE_WAIT + ClearRelated::GOTO_NEXT_WAIT < clearTimer )
	{
		const int   DESTINATION = SCREEN_WIDTH - ( ClearImage::SIZE_GOTO_NEXT_X + 32/* �]�� */);
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

	if ( TRG( KEY_INPUT_E ) && isOpenFade/* �A�Ŗh�~ */ )
	{
		PlaySE( M_E_NEXT );

		nextState = State::Select;
		FadeBegin();
	}

#endif // DEBUG_MODE

	ShakeUpdate();
}

void Game::MilkyWayUpdate()
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

	// �t�L�_�V�̍X�V
	if ( 0 != balloonLength )
	{
		constexpr int LOWER = 12;
		constexpr float DIV = 0.3f;

		if ( isOpenBalloon )	// �Ђ炭
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
		else	// �Ƃ���
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

	// �N���A�䎌
	if ( nextState == State::Clear || state == State::Clear )
	{
		// �\�����Ԍo�ߊm�F
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

		// �����������m�F
		if ( 0 != textLength && ( textLength * 2 ) <= scast<int>( TextBehavior::CLEAR_SAY[textNumber].size() ) )
		{
			constexpr int INTERVAL = 3;
			textExtendInterval++;
			if ( INTERVAL <= textExtendInterval )
			{
				textExtendInterval = 0;
				textLength++;

				// ���ω�
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

	// �`���[�g���A��
	if ( stageNumber == 1 )
	{
		constexpr int TUTORIAL_TEXT_START_FRAME		= 80;
		constexpr int TUTORIAL_BALLOON_START_FRAME	= TUTORIAL_TEXT_START_FRAME - 20;
		if ( state == State::Game )	// �N���A��͐V�����͏o���Ȃ�
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

		// �\�����Ԍo�ߊm�F
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
			else // else���ɂ���ƁC���ׂĕ\��������łȂ��ƃ��Z�b�g�ł��Ȃ��悤�ɂȂ�
			if ( IS_TRG_SELECT )
			{
				textTimer = TUTORIAL_TEXT_START_FRAME + TextBehavior::TUTORIAL_SHOW_FRAME[0];

				textLength = 1;
				textExtendInterval = 0;

				textNumber = 1;
			}
		}

		// �����������m�F
		if ( 0 != textLength && ( textLength * 2 ) <= scast<int>( TextBehavior::TUTORIAL[textNumber].size() ) )
		{
			constexpr int INTERVAL = 3;
			textExtendInterval++;
			if ( INTERVAL <= textExtendInterval )
			{
				textExtendInterval = 0;
				textLength++;

				// ���ω�
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

	constexpr int SAY_INTERVAL	= 60 * 20/* �b�� */;
	int remTimer = textTimer % SAY_INTERVAL;
	{
		constexpr int TEXT_START_FRAME		= 80;
		constexpr int BALLOON_START_FRAME	= TEXT_START_FRAME - 20;
		if ( state == State::Game )	// �N���A��͐V�����͏o���Ȃ�
		{
			if ( remTimer == TEXT_START_FRAME )
			{
				textLength = 1;
			}
			if ( remTimer == BALLOON_START_FRAME )
			{
				// �����ԍ��Z�o
				{
					int size = scast<int>( TextBehavior::RAND_SAY.size() );
					textNumber = rand() % size;
				}

				isOpenBalloon = true;

				constexpr int INIT_LENGTH = 64;
				balloonLength = INIT_LENGTH;
			}
		}

		// �\�����Ԍo�ߊm�F
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

		// �����������m�F
		if ( 0 != textLength && ( textLength * 2 ) <= scast<int>( TextBehavior::RAND_SAY[textNumber].size() ) )
		{
			constexpr int INTERVAL = 3;
			textExtendInterval++;
			if ( INTERVAL <= textExtendInterval )
			{
				textExtendInterval = 0;
				textLength++;

				// ���ω�
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
	pos.x += scast<float>( SCREEN_WIDTH  ) * 0.2f/* �ʒu�̒��� */;
	pos.y -= scast<float>( SCREEN_HEIGHT ) * 1.0f/* �ʒu�̒��� */;

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

	// �u���̃X�e�[�W�v��I�񂾏ꍇ
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
		{ star.cx - star.w, star.cy - star.h },	// ����
		{ star.cx + star.w, star.cy - star.h },	// �E��
		{ star.cx - star.w, star.cy + star.h },	// ����
		{ star.cx + star.w, star.cy + star.h }	// �E��
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

	std::vector<int> targets{};
	std::vector<Vector2> particlePos{};

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
				pCamera->SetShake();
				PlaySE( M_FAILURE );

				return false;
			}
			// else
			if ( IsInsideStarFourCorners( camera, star ) )
			{
				targets.push_back( i );
				particlePos.push_back( { star.cx, star.cy } );
			}
			else
			{
				pCamera->SetShake();
				PlaySE( M_FAILURE );

				return false;
			}
		}
	}

	// �K�p�Ώۂ����Ȃ�������I���
	if ( !scast<int>( targets.size() ) || !end )
	{
		return false;
	}
	// else

	pCamera->SetGlow();

	// TODO:Exposure�̉���炷�^�C�~���O�́C�������m�肵�Ă��炩�ǂ����B
	PlaySE( M_EXPOSURE );

	// �A���h�D�p
	pStarMng->SaveLog();
	pCamera->SaveLog();

	// �K�p
	for ( int i = 0; i < scast<int>( targets.size() ); i++ )
	{
		pStarMng->Expose( targets.at( i ) );
	}
	if ( pParticleMng )
	{
		for ( int i = 0; i < scast<int>( particlePos.size() ); i++ )
		{
			pParticleMng->Generate( PARTICLE_AMOUNT, particlePos.at( i ), /* isBig = */false );
		}
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
	// �|�[�Y����
	{
		constexpr int INTERVAL	= 120;
		constexpr float AMPL	= 0.6f;
		pauseTimer++;
		pausePos.y += sinf( ( PI / INTERVAL ) * pauseTimer ) * AMPL;
	}

	// choice �� 0 �n�܂�ł���

	constexpr int MAX_MENU = 4;

	bool isUp = false, isDown = false;

	if ( IS_TRG_UP	 ) { isUp	= true; }
	if ( IS_TRG_DOWN ) { isDown	= true; }

	int lower = ( stageNumber == FileIO::GetMaxStageNumber() ) ? 1 : 0;
	if ( ( 0		< choice		)	&& isUp		&& !isDown	) { choice -= 1; PlaySE( M_SELECT ); }
	if ( ( choice	< MAX_MENU - 1	)	&& isDown	&& !isUp	) { choice += 1; PlaySE( M_SELECT ); }
	
	/*	// �㉺���q����
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

	if ( isPause )
	{
		SetDrawBright( 255, 255, 255 );

		PauseDraw();
	}

	SetDrawBright( 255, 255, 255 );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
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

	// ���ꐯ
	if ( pSSMng )
	{
		pSSMng->Draw( shake );
	}

	if ( pParticleMng )
	{
		pParticleMng->Draw( shake );
	}

	// ����
	{
		// �Z���N�g
		DrawGraph
		(
			scast<int>( selectPos.x ),
			scast<int>( selectPos.y ),
			SelectImage::hStatement,
			TRUE
		);
		// ������@
		DrawGraph
		(
			0, 0,
			SelectImage::hUsage,
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

	}

	// ���ꐯ
	if ( pSSMng )
	{
		pSSMng->Draw( shake );
	}

	// �V�̐�
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

	// �g��UI
	//if ( !isPause )
	{
		DrawGraph
		(
			0, 0,
			GameImage::hFrameUI,
			TRUE
		);
	}

	// �萔�ڕW
	if ( pNumMoves )
	{
		int x = 0, y = 0;
		GetMousePoint( &x, &y );
		
		constexpr int BASE_X = 100;
		constexpr int BASE_Y = 360;

		constexpr int INTERVAL = ClearImage::SIZE_STAR_Y >> 1;

		std::vector<int> dataAscendingOrder = pNumMoves->GetData();
		int content = -1;
		int nowRank = pNumMoves->CalcRank( numMoves );	// 0 �n�܂�
		int glowImageIndex = -1;
		size_t end = dataAscendingOrder.size();
		for ( size_t i = 0; i < end; ++i )
		{
			content = dataAscendingOrder[i];

			// ��
			{
				glowImageIndex = ( scast<size_t>( nowRank ) <= i ) ? 1 : 0;
				DrawRotaGraph
				(
					BASE_X,
					BASE_Y + ( INTERVAL * i ) + 8/* ���̒��S���摜�̒��S�ł͂Ȃ��̂ŁC�������炵�Ă��� */,
					0.5,
					ToRadian( 0 ),
					ClearImage::hRecordStar[glowImageIndex],
					TRUE
				);
			}

			// �萔
			{
				int movNum = content;
				for ( int digit = 0; digit < 2; digit++ )
				{
					if ( content < 10 )
					{
						if ( digit != 0 ) { continue; }
						// else

						DrawRotaGraph
						(
							BASE_X,
							BASE_Y + ( INTERVAL * i ),
							0.5,
							0,
							Number::GetHandle( movNum % 10, true ),
							TRUE
						);

						continue;
					}
					// else

					DrawRotaGraph
					(
						BASE_X + ( Number::SIZE_X >> 3 ) - ( ( Number::SIZE_X >> 2 ) * digit ),
						BASE_Y + ( INTERVAL * i ),
						0.5,
						0,
						Number::GetHandle( movNum % 10, true ),
						TRUE
					);

					movNum /= 10;
				}
			}
		}
	}

	// Grid::Draw( shake );

	if ( pParticleMng )
	{
		pParticleMng->Draw( shake );
	}

	if ( pStarMng )
	{
		pStarMng->Draw( shake );
	}

	// ���o�f�ɂ����
	if ( shutter_flag )
	{
		//�V���b�^�[(�ォ�牺)
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

		//�V���b�^�[(�������)
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

	if ( nextState != State::Clear && pCamera )
	{
		pCamera->Draw( shake );
	}
	
	// �l
	{
		// ��
		DrawGraph
		(
			0,
			SCREEN_HEIGHT - HumanImage::SIZE_Y,
			HumanImage::GetBodyHandle( wink.GetAnimeIndex() ),
			TRUE
		);

		// ��
		DrawGraph
		(
			0,
			SCREEN_HEIGHT - HumanImage::SIZE_Y,
			HumanImage::GetMouthHandle( mouthIndex ),
			TRUE
		);

		// �r
		DrawGraph
		(
			scast<int>( armPos.x ),
			scast<int>( armPos.y ),
			HumanImage::GetArmHandle( NULL ),
			TRUE
		);
	}

	// �t�L�_�V
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
	// �Z���t
	if ( 0 != textLength )
	{
		TextDraw();
	}
}

void Game::ClearDraw()
{
	Vector2 shake = GetShakeAmount();

	// �t�F�[�h�A�E�g���Ă������̂���
	if ( !isShowClearMenu )
	{
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

		}

		// �V�̐�
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

		// �g��UI
		DrawGraph
		(
			0, 0,
			GameImage::hFrameUI,
			TRUE
		);

		// �萔�ڕW
		if ( pNumMoves )
		{
			int x = 0, y = 0;
			GetMousePoint( &x, &y );

			constexpr int BASE_X = 100;
			constexpr int BASE_Y = 360;

			constexpr int INTERVAL = ClearImage::SIZE_STAR_Y >> 1;

			std::vector<int> dataAscendingOrder = pNumMoves->GetData();
			int content = -1;
			int nowRank = pNumMoves->CalcRank( numMoves );	// 0 �n�܂�
			int glowImageIndex = -1;
			size_t end = dataAscendingOrder.size();
			for ( size_t i = 0; i < end; ++i )
			{
				content = dataAscendingOrder[i];

				// ��
				{
					glowImageIndex = ( scast<size_t>( nowRank ) <= i ) ? 1 : 0;
					DrawRotaGraph
					(
						BASE_X,
						BASE_Y + ( INTERVAL * i ) + 8/* ���̒��S���摜�̒��S�ł͂Ȃ��̂ŁC�������炵�Ă��� */,
						0.5,
						ToRadian( 0 ),
						ClearImage::hRecordStar[glowImageIndex],
						TRUE
					);
				}

				// �萔
				{
					int movNum = content;
					for ( int digit = 0; digit < 2; digit++ )
					{
						if ( content < 10 )
						{
							if ( digit != 0 ) { continue; }
							// else

							DrawRotaGraph
							(
								BASE_X,
								BASE_Y + ( INTERVAL * i ),
								0.5,
								0,
								Number::GetHandle( movNum % 10, true ),
								TRUE
							);

							continue;
						}
						// else

						DrawRotaGraph
						(
							BASE_X + ( Number::SIZE_X >> 3 ) - ( ( Number::SIZE_X >> 2 ) * digit ),
							BASE_Y + ( INTERVAL * i ),
							0.5,
							0,
							Number::GetHandle( movNum % 10, true ),
							TRUE
						);

						movNum /= 10;
					}
				}
			}
		}

		// ��
		if ( pStarMng )
		{
			pStarMng->Draw( shake );
		}

		// �l
		{
			int animIndex = 0;

			// ��
			DrawGraph
			(
				0,
				SCREEN_HEIGHT - HumanImage::SIZE_Y,
				HumanImage::GetBodyHandle( animIndex ),
				TRUE
			);

			// ��
			DrawGraph
			(
				0,
				SCREEN_HEIGHT - HumanImage::SIZE_Y,
				HumanImage::GetMouthHandle( mouthIndex ),
				TRUE
			);

			// �r
			DrawGraph
			(
				scast<int>( armPos.x ),
				scast<int>( armPos.y ),
				HumanImage::GetArmHandle( animIndex ),
				TRUE
			);
		}
		// �t�L�_�V
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
		// �Z���t
		if ( 0 != textLength )
		{
			int index = textNumber % scast<int>( TextBehavior::CLEAR_SAY.size() );
			int length = textLength * 2/* ���{��łQ�o�C�g�����Ȃ̂ŁC�{�ɂ��đΉ� */;
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

		GameDrawUI();
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

	// ���ꐯ
	if ( pSSMng )
	{
		pSSMng->Draw( shake );

		if ( clearTimer < 255 )
		{
			int alpha = ( 255 / ClearRelated::FADE_WAIT )/* AMPL */ * clearTimer;

			SetDrawBlendMode( DX_BLENDMODE_ALPHA, alpha );
		}
		else
		{
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 );
		}
	}

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
		// HACK:�X�e�[�W���͂Q�P�^�ɂ����܂�z��ł���
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
				STAGE_POS_X - ( Number::SIZE_X >> 1 ) - ( ( Number::SIZE_X + 8/* ���� */ ) * digit ),
				STAGE_POS_Y,
				STAGE_POS_X - ( Number::SIZE_X >> 1 ) - ( ( Number::SIZE_X + 8/* ���� */ ) * digit ) + ( Number::SIZE_X * STAGE_MAGNI_X ),
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
				MOVES_POS_X - ( Number::SIZE_X >> 1 ) - ( ( ( Number::SIZE_X >> 1 ) + 8/* ���� */ ) * digit ),
				MOVES_POS_Y,
				MOVES_POS_X - ( Number::SIZE_X >> 1 ) - ( ( ( Number::SIZE_X >> 1 ) + 8/* ���� */ ) * digit ) + ( Number::SIZE_X * MOVES_MAGNI_X ),
				MOVES_POS_Y + ( Number::SIZE_Y * MOVES_MAGNI_Y ),
				Number::GetHandle( movNum % 10, true ),
				TRUE
			);

			movNum /= 10;
		}
	}
	// �萔���ȓ�
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
				"�萔 %d �ȓ�",
				data.at( 2 - i )
			);
		}
	}

	// �w�i�Ƃ��ẮC�Â���
	{
		// HACK:���̐����R����Ȃ��Ȃ�C�������ς���K�v������

		int i = scast<int>( recordStars.size() );
		for ( ; i < 3; i++ )
		{
			// ClearUpdate()���̂��̂Ɠ���

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

	if ( pParticleMng )
	{
		pParticleMng->Draw( shake );
	}

	for ( const RecordStar &it : recordStars )
	{
		it.Draw( shake );
	}

	// �u���ցv�\��
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

	// ����
	DrawGraph
	(
		608,
		288,
		ClearImage::GetStatementHandle( 0, isFinalStage ),
		TRUE
	);
	// ����
	DrawGraph
	(
		608,
		288,
		ClearImage::GetStatementHandle( choice + 1, isFinalStage ),
		TRUE
	);
}

void Game::TextDraw()
{
	if ( nextState == State::Clear )	// �N���A
	{
		int index = textNumber % scast<int>( TextBehavior::CLEAR_SAY.size() );
		int length = textLength * 2/* ���{��łQ�o�C�g�����Ȃ̂ŁC�{�ɂ��đΉ� */;
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

		return;
	}
	// else

	if ( stageNumber == 1 )	// �`���[�g���A��
	{
		int index = textNumber % scast<int>( TextBehavior::TUTORIAL.size() );
		int length = textLength * 2/* ���{��łQ�o�C�g�����Ȃ̂ŁC�{�ɂ��đΉ� */;
		if ( scast<int>( TextBehavior::TUTORIAL[index].size() ) <= textLength )
		{
			length = scast<int>( TextBehavior::TUTORIAL[index].size() );
		}

		constexpr int DIST_X	= 80;
		constexpr int DIST_Y	= 52;
		constexpr double MAGNI	= 2.0;

		int emphasisPos = 0, i = 0;
		for ( ; i < scast<int>( TextBehavior::EMPHASIS_STR.size() ); i++ )
		{
			emphasisPos = TextBehavior::TUTORIAL[index].find( TextBehavior::EMPHASIS_STR[i] );
			if ( emphasisPos != std::string::npos )
			{
				break;
			}
		}

		if ( i == scast<int>( TextBehavior::EMPHASIS_STR.size() ) )	// ���[�v���I�������C������Ȃ������Ƃ������ƂɂȂ�
		{
			DrawExtendStringToHandle
			(
				HumanBehavior::BALLOON_POS_X + DIST_X,
				HumanBehavior::BALLOON_POS_Y + DIST_Y,
				MAGNI, MAGNI,
				( TextBehavior::TUTORIAL[index].substr( 0, length ) ).c_str(),
				GetColor( 42, 97, 110 ),
				hFont
			);

			return;
		}
		// else

		// HACK:������ƐF�̈Ⴂ�������΂قƂ�Ǔ��������Ȃ̂ŁC���[�v���ɂ܂Ƃ߂��邩���H

		if ( length <= 0 )
		{
			return;
		}
		// else

		int charWidth = 0;
		std::string text = "";

		// �������镶����܂�
		text = TextBehavior::TUTORIAL[index].substr( 0, std::min( length, emphasisPos ) );

		// Draw
		{
			DrawExtendStringToHandle
			(
				HumanBehavior::BALLOON_POS_X + DIST_X + charWidth,
				HumanBehavior::BALLOON_POS_Y + DIST_Y,
				MAGNI, MAGNI,
				text.c_str(),
				GetColor( 42, 97, 110 ),
				hFont
			);
			charWidth += GetDrawExtendStringWidthToHandle
			(
				MAGNI,
				text.c_str(),
				text.size(),
				hFont
			);
		}

		length -= scast<int>( text.size() );

		if ( length <= 0 )
		{
			return;
		}
		// else

		// ����������
		text = TextBehavior::TUTORIAL[index].substr( emphasisPos, std::min( length, scast<int>( TextBehavior::EMPHASIS_STR[i].size() ) ) );

		// Draw
		{
			DrawExtendStringToHandle
			(
				HumanBehavior::BALLOON_POS_X + DIST_X + charWidth,
				HumanBehavior::BALLOON_POS_Y + DIST_Y,
				MAGNI, MAGNI,
				text.c_str(),
				GetColor( 194, 36, 0 ),
				hFont
			);
			charWidth += GetDrawExtendStringWidthToHandle
			(
				MAGNI,
				text.c_str(),
				text.size(),
				hFont
			);
		}

		length -= scast<int>( text.size() );

		if ( length <= 0 )
		{
			return;
		}
		// else

		// ���̌�̕�����
		text = TextBehavior::TUTORIAL[index].substr( emphasisPos + TextBehavior::EMPHASIS_STR[i].size(), length );

		// Draw
		{
			DrawExtendStringToHandle
			(
				HumanBehavior::BALLOON_POS_X + DIST_X + charWidth,
				HumanBehavior::BALLOON_POS_Y + DIST_Y,
				MAGNI, MAGNI,
				text.c_str(),
				GetColor( 42, 97, 110 ),
				hFont
			);
		}

		return;
	}
	// else

	// �����_������

	int index = textNumber % scast<int>( TextBehavior::RAND_SAY.size() );
	int length = textLength * 2/* ���{��łQ�o�C�g�����Ȃ̂ŁC�{�ɂ��đΉ� */;
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

void Game::PauseDraw()
{
	// �|�[�Y
	DrawGraph
	(
		scast<int>( pausePos.x ),
		scast<int>( pausePos.y ),
		PauseImage::GetHandle(),
		TRUE
	);

	// ����
	DrawGraph
	(
		608,
		288,
		PauseImage::GetMenuHandle( 0 ),
		TRUE
	);
	// ����
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
	// �X�e�[�W��
	//if ( !isPause )
	{
		constexpr int MAX_DIGIT = 2;

		const int POS_X = 160;
		const int POS_Y = 0;

		int stgNum = stageNumber;
		for ( int digit = 0; digit < MAX_DIGIT; digit++ )
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
					POS_X,
					POS_Y,
					POS_X + ( Number::SIZE_X >> 1 ),
					POS_Y + ( Number::SIZE_Y >> 1 ),
					Number::GetHandle( stgNum % 10, true ),
					TRUE
				);

				continue;
			}
			// else

			int tweakX = 0;

			DrawExtendGraph
			(
				POS_X + tweakX + ( ( ( MAX_DIGIT - 1 ) - digit ) * ( Number::SIZE_X >> 1 )/* ���� */ ),
				POS_Y,
				POS_X + ( Number::SIZE_X >> 1 ) + tweakX + ( ( ( MAX_DIGIT - 1 ) - digit ) * ( Number::SIZE_X >> 1 )/* ���� */ ),
				POS_Y + ( Number::SIZE_Y >> 1 ),
				Number::GetHandle( stgNum % 10, true ),
				TRUE
			);

			stgNum /= 10;
		}
	}

	// �萔
	{
		//DrawGraph
		//(
		//	FRAME_POS_X - 160,
		//	FRAME_POS_Y,
		//	GameImage::hMovesUI,
		//	TRUE
		//);

		// ����
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
					MOVES_POS_X - ( Number::SIZE_X >> 1 ) - ( ( ( Number::SIZE_X >> 1 ) + 8/* ���� */ ) * digit ),
					MOVES_POS_Y,
					MOVES_POS_X - ( Number::SIZE_X >> 1 ) - ( ( ( Number::SIZE_X >> 1 ) + 8/* ���� */ ) * digit ) + ( Number::SIZE_X * MOVES_MAGNI_X ),
					MOVES_POS_Y + ( Number::SIZE_Y * MOVES_MAGNI_Y ),
					Number::GetHandle( movNum % 10, true ),
					TRUE
				);

				movNum /= 10;
			}
		}
	}

	// �����x���̕\��
	{
		const Vector2 BASE{ scast<float>( FRAME_POS_X + FRAME_WIDTH ),scast<float>( FRAME_POS_Y ) };
		const Vector2 TWEAK{ 64.0f, 0 };
		const Vector2 HALF_SIZE{ Grid::GetSize().x * 0.5f , Grid::GetSize().y * 0.5f };

		// �i�K�̖��
		//DrawGraph
		//(
		//	scast<int>( BASE.x ),
		//	scast<int>( BASE.y ),
		//	StarImage::GetGradeHandle(),
		//	TRUE
		//);

		for ( int i = 1; i <= Star::MAX_LEVEL; i++ )
		{
			double angle = ( i % 2 ) ? 0 : 45.0;

			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
			DrawRotaGraph
			(
				scast<int>( BASE.x + TWEAK.x + HALF_SIZE.x ),									// ���S���W
				scast<int>( BASE.y + TWEAK.y + HALF_SIZE.y ) + ( ( i - 1 ) * StarImage::SIZE ),	// ���S���W
				1.0, ToRadian( angle ),
				StarImage::GetHandle( i, 0 ),
				TRUE
			);
			SetDrawBlendMode( DX_BLENDMODE_ADD, 255 );
			DrawRotaGraph
			(
				scast<int>( BASE.x + TWEAK.x + HALF_SIZE.x ),									// ���S���W
				scast<int>( BASE.y + TWEAK.y + HALF_SIZE.y ) + ( ( i - 1 ) * StarImage::SIZE ),	// ���S���W
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