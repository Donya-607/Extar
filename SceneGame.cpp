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
	constexpr int LIMIT_STAGE_NUMBER = 12;

	constexpr int PARTICLE_AMOUNT = 6;
	constexpr int SS_PROBABILITY  = 128;
}

#define STRING_FOR_MOVIE ( 0 )

namespace TextBehavior
{
	// 文の要素数と表示フレームの要素数は揃えてください
	// TUTORIAL_XXXは初めのステージのもの，
	// TUROAIL_TOGGLE_XXXはカメラの回転のものです

	const std::vector<std::string> TUTORIAL =
	{
	#if STRING_FOR_MOVIE

		"やっほー、エクスタ案内人のえっちゃんだよ！",

		"君はカメラの枠を動かせるよ。",
		"枠内にある星を露光して、星の明るさをあげられるよ。",
		"すべての星の明るさを揃えよう！",

		"星の明るさには、１等星から６等星まであるよ。",
		"露光は、星の明るさを１段階あげられるよ。",

		"がむしゃらにやってもできるけど・・・",
		"うまくまとめられるように操作すると・・・",
		"さっきよりも短く揃えられるよ！",

		"短い手数で揃えてみよう☆",

		"ステージは全部で30ステージ！",
		"すべてのステージの星を揃えていこう！",

	#else

		"やっほー！初心者さんかな？　やり方教えるね♪",
		"水色の枠がカメラの範囲だよ！",

		"範囲内に星をおさめると、ＡかＸで露光が使えるよ",
		"そうそう！いい感じ♪",

		"露光は、範囲内にある星を明るくできるよ",
		"すべての星の明るさをそろえて撮影しよう！",
		"セレクトボタンでもう一度教えるよ！"

	#endif // STRING_FOR_MOVIE
	};
	const std::vector<int> TUTORIAL_SHOW_FRAME =
	{
	#if STRING_FOR_MOVIE

		300,
		
		300,
		300,
		300,

		300,
		300,

		300,
		300,
		300,

		300,

		300,
		-1,

	#else

		180,
		180,

		120,
		120,

		180,
		180,
		-1

	#endif // STRING_FOR_MOVIE
	};
	using PSCond = ProgressStorage::Conditions;
	const std::vector<PSCond> TUTORIAL_CONDITIONS =
	{
	#if STRING_FOR_MOVIE

		PSCond::WaitingTime,

		PSCond::WaitingTime,
		PSCond::WaitingTime,
		PSCond::WaitingTime,

		PSCond::WaitingTime,
		PSCond::WaitingTime,

		PSCond::WaitingTime,
		PSCond::WaitingTime,
		PSCond::WaitingTime,

		PSCond::WaitingTime,

		PSCond::WaitingTime,
		PSCond::IMPOSSIBLE,

	#else

		PSCond::WaitingTime,
		PSCond::WaitingTime,

		PSCond::InputExposure	| PSCond::WaitingTime,
		PSCond::WaitingTime,

		PSCond::WaitingTime,
		PSCond::WaitingTime,
		PSCond::IMPOSSIBLE

	#endif // STRING_FOR_MOVIE
	};
	using PSInput = ProgressStorage::BitInput;
	const std::vector<PSInput> TUTORIAL_PERMISSIONS =
	{
	#if STRING_FOR_MOVIE

		PSInput::ALL_OK,

		PSInput::ALL_OK,
		PSInput::ALL_OK,
		PSInput::ALL_OK,

		PSInput::ALL_OK,
		PSInput::ALL_OK,

		PSInput::ALL_OK,
		PSInput::ALL_OK,
		PSInput::ALL_OK,

		PSInput::ALL_OK,

		PSInput::ALL_OK,
		PSInput::ALL_OK,

	#else

		PSInput::Move,
		PSInput::Move,

		PSInput::Move | PSInput::Exposure,
		PSInput::Move | PSInput::Exposure,

		PSInput::Move | PSInput::Exposure,
		PSInput::Move | PSInput::Exposure,
		PSInput::Move | PSInput::Exposure

	#endif // STRING_FOR_MOVIE
	};

	// 矢印を生成するテキスト番号（０始まり）
	constexpr int TUTORIAL_TOGGLE_GENERATE_ARROW_TIMING = 1;
	const std::vector<std::string> TUTORIAL_TOGGLE =
	{
		"おっ！なかなか難しそうな空だね",
		"Ｒでカメラを縦にすることができるよ！",

		"その調子！",
		"ここからはもっといい写真を撮っていこう♪",

		"セレクトボタンでもう一度教えるよ！"
	};
	const std::vector<int> TUTORIAL_TOGGLE_SHOW_FRAME =
	{
		180,
		120,

		120,
		180,

		-1
	};
	const std::vector<PSCond> TUTORIAL_TOGGLE_CONDITIONS =
	{
		PSCond::WaitingTime,
		PSCond::WaitingTime | PSCond::InputToggle,

		PSCond::WaitingTime,
		PSCond::WaitingTime,

		PSCond::IMPOSSIBLE
	};
	const std::vector<PSInput> TUTORIAL_TOGGLE_PERMISSIONS =
	{
		PSInput::Move,
		PSInput::Move | PSInput::Toggle,

		PSInput::Move | PSInput::Toggle,
		PSInput::Move | PSInput::Exposure | PSInput::Toggle,

		PSInput::Move | PSInput::Exposure | PSInput::Toggle
	};

	const std::vector<std::string> EMPHASIS_STR =	// RGB( 87, 101, 255 )
	{
		// チュートリアル時のみ，すべての文章にて，
		// ここの強調文字一覧が調べられ，どれかに引っかかり次第，それのみが強調されます

	#if STRING_FOR_MOVIE

		"星",
		"露光",
		"エクスタ",

	#else

		"水色の枠",
		"露光",
		"すべて",
		"セレクトボタン",

		"Ｒ",

	#endif // STRING_FOR_MOVIE
	};

	// 三点リーダは「・・・」とする（３文字）

	const std::vector<std::string> RAND_SAY =
	{
		"がんばれ～！",
		"キラッ★",
		"ファイトォ！",

		"綺麗に撮ってね！",
		"写真楽しみにしてる♪",
		"上手に撮れるかなぁ",
		"星がきれいだねー",
	};
	const std::vector<int> RAND_SAY_SHOW_FRAME =
	{
		180,
		90,
		180,

		180,
		180,
		180,
		180,
	};
	
	enum Reactions
	{
		CantExposure,
		WhyDontYouUndo,
		Uhhh_1,
		Uhhh_2,
		Uhhh_3,
		Uhhh_4,
		GoodVibes_1,
		GoodVibes_2,

		REACTION_END
	};
	const std::vector<std::string> REACTION_SAY =
	{
		"それ以上は露光できないよ！",
		"一度明るさをＢかＹで戻してみる？",

		"迷うなぁ",
		"ん～悩むねぇ",
		"ん～どうするんだろう・・・",
		"難しいなぁ・・・",

		"その調子その調子！",
		"いい感じかも♪",
	};
	const std::vector<int> REACTION_SAY_SHOW_FRAME =
	{
		180,
		180,

		180,
		180,
		180,
		180,

		180,
		180,
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
	static int hStatement;
	static int hUsage;

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
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
	static int hUsageR;
	//static int hMovesUI;
	static int hArrow;


	static int hshutter;

	constexpr int WIDTH_USAGE_R  = 280;
	constexpr int HEIGHT_USAGE_R = 190;

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
		hUsageR		= LoadGraph( "./Data/Images/UI/UsageR.png" );
		//hMovesUI	= LoadGraph( "./Data/Images/UI/NumberOfMoves.png" );
		hArrow		= LoadGraph( "./Data/Images/UI/Arrow.png" );


		hshutter	= LoadGraph( "./Data/Images/Camera/Shutter.png" );


	}
	void Release()
	{
		DeleteGraph( hGameBG	);
		DeleteGraph( hFrameBG	);
		DeleteGraph( hFrameUI	);
		DeleteGraph( hUsageR	);
		DeleteGraph( hArrow		);
		//DeleteGraph( hMovesUI );
		hGameBG		= 0;
		hFrameBG	= 0;
		hFrameUI	= 0;
		hUsageR		= 0;
		hArrow		= 0;
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
	constexpr int GOTO_NEXT_SPACE	= 32;
}

namespace MilkyWayImage
{
	static int hMilkyWay[2];

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
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

	static int hBalloon;	// フキダシ

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
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

// 横着な名前空間。別のヘッダやソースファイルなど作ってそちらで管理すべきである
namespace
{
	static float   arrowShakeSpeed  = 16.0f;
	static float   arrowShakeAmount = 16.0f;
	static Vector2 arrowBasePos{ 1700.0f, 800.0f };

	class HorizontalArrow
	{
	private:
		float   radian = 0.0f;
		Vector2 pos{};
		Vector2 shake{};
	public:
		void Reset()
		{
			radian  = 0.0f;
			pos     = { 0.0f, 0.0f };
			shake.x = 0.0f;
			shake.y = 0.0f;
		}
		void SetPosition( const Vector2 &initPos )
		{
			pos = initPos;
		}

		void Update()
		{
			radian += ToRadian( arrowShakeSpeed );

			shake.y = sinf( radian ) * arrowShakeAmount;
		}

		void Draw( int handle )
		{
			DrawGraphF
			(
				( pos + shake ).x,
				( pos + shake ).y,
				handle, TRUE
			);
		}
	};

	static std::unique_ptr<HorizontalArrow> pArrow{ nullptr };
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
	timer = std::min( TIMER_MAX, timer );

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

void RecordStar::SkipPerformance()
{
	scale = 1.0f;
	timer = TIMER_MAX;
}

void Game::Wink::Init()
{
	Lottering();
}
void Game::Wink::Uninit()
{
	waitFrame = -1;
	animIndex = 0;
}

void Game::Wink::Update()
{
	Wait();
	Animation();
}

void Game::Wink::Lottering()
{
	constexpr int MIN_SEC = 2;
	waitFrame = rand() % 3;
	waitFrame += MIN_SEC;

	waitFrame *= 60;

	animIndex = 0;
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
	FileIO::SetStageLimit( LIMIT_STAGE_NUMBER );
	isUnlockedStage = false;

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
	UnlockAnnouncer::LoadImages();

	pArrow.reset();

	hFont = CreateFontToHandle
	(
		"メイリオ",
		// "JFドットK12",
		-1,				// サイズ（-1：デフォルト）
		-1,				// 太さ（-1：デフォルト）
		DX_FONTTYPE_NORMAL
	);

	pSSMng.reset( new ShootingStarMng() );
	pSSMng->Init();

	pParticleMng.reset( new ParticleMng() );
	pParticleMng->Init();

	pUnlockAnnouncer.reset();

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

	numMoves	= 0;

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

	pArrow.reset();

	pCamera.reset( new Camera() );
	pCamera->Init( stageNumber );

	pStarMng.reset( new StarMng() );
	pStarMng->Init( stageNumber );

	pNumMoves.reset( new NumMoves() );
	pNumMoves->Init( stageNumber );

	pRotator.reset();
	if ( stageNumber == 1 )
	{
		pProgress = std::make_unique<ProgressStorage>
		(
			TextBehavior::TUTORIAL_CONDITIONS,
			TextBehavior::TUTORIAL_PERMISSIONS
		);
	}
	else
	if ( stageNumber == LIMIT_STAGE_NUMBER && !isUnlockedStage )
	{
		pProgress = std::make_unique<ProgressStorage>
		(
			TextBehavior::TUTORIAL_TOGGLE_CONDITIONS,
			TextBehavior::TUTORIAL_TOGGLE_PERMISSIONS
		);
	}
	else
	{
		pProgress.reset();
	}

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

	succeedCounter		= 0;
	timeSinceSucceed	= 0;
	failedCounter		= 0;
	timeSinceFailed		= 0;
	lastResult			= false;
	timeSinceOperated	= 0;

	armPos		= { 0, scast<float>( ( SCREEN_HEIGHT - HumanImage::SIZE_Y ) + HumanBehavior::HAND_LET_DOWN_PLUS_Y ) };
	pausePos	= { 704.0f, 64.0f };

	isOpenBalloon  = true;
	isTalkReaction = false;
	isDoneMoveArm  = false;

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
	UnlockAnnouncer::ReleaseImages();

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

	pRotator.reset();
	pProgress.reset();

	succeedCounter		= 0;
	timeSinceSucceed	= 0;
	failedCounter		= 0;
	timeSinceFailed		= 0;
	lastResult			= false;
	timeSinceOperated	= 0;

	isTalkReaction		= false;
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

	if ( TRG( KEY_INPUT_G ) )
	{
		GenerateRotator();
	}
	
	if ( TRG( KEY_INPUT_F ) && state == State::Select )
	{
		pUnlockAnnouncer = std::make_unique<UnlockAnnouncer>();
	}

#if USE_IMGUI
	FileIO::UpdateShowWindowState();
#endif // USE_IMGUI

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
					isOpenFade = true;
				}
				break;

			}
		}
	}
}

void Game::SelectUpdate()
{
	// セレクト文字
	{
		constexpr int INTERVAL = 120;
		constexpr float AMPL = 0.6f;
		selectTimer++;
		selectPos.y += sinf( ( PI / INTERVAL ) * selectTimer ) * AMPL;
	}

	if ( pSSMng )
	{
		// 発生
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

	if ( pUnlockAnnouncer )
	{
		pUnlockAnnouncer->Update();
		if ( pUnlockAnnouncer->ShouldRemove() )
		{
			pUnlockAnnouncer.reset();
		}
	}

	if ( pCursor )
	{
		bool canDecision = ( pUnlockAnnouncer ) ? false : true;
		pCursor->Update( isOpenFade/* フェードイン中の操作制限のため */, canDecision );

		stageNumber = pCursor->GetNowStageNumber();

		if ( nextState == State::Null && pCursor->IsDecision() )
		{
			// 演出中は選択できない
			if ( pUnlockAnnouncer )
			{
				PlaySE( M_CANT_PUSH );
			}
			else
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
		// この条件の中には，クリア判定時に１度だけ入る想定
		if ( nextState == State::Null )
		{
			nextState = State::Clear;

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

			isOpenFade = false;	// 操作不能にするため
			isDoneMoveArm = false;

			GenerateRotator();

			PlaySE( M_ALIGN_STARS );
		}

		// 上の「クリア判定の瞬間のみ入るプロセス」内でpRotatorを生成しているので，
		// それによる星の回転演出が終わるまでは，クリア演出が止まるようになるという算段
		if ( !pRotator )
		{
			armPos.y -= HumanBehavior::HAND_RISE_SPD_Y;

			if ( armPos.y < ( SCREEN_HEIGHT - HumanImage::SIZE_Y ) )
			{
				// armPos.y = scast<float>( SCREEN_HEIGHT - HumanImage::SIZE_Y );
				armPos.y += HumanBehavior::HAND_RISE_SPD_Y;

				isClearMoment	= true;
				isDoneMoveArm	= true;
			}
		}
	}

	if ( pCamera && isOpenFade/* フェードイン中の操作制限のため */ )
	{
		bool allowMove		= true;
		bool allowExposure	= true;
		bool allowToggle	= true;
		if ( pProgress )
		{
			using Input		= ProgressStorage::BitInput;
			allowMove		= pProgress->IsAllowInput( Input::Move		);
			allowExposure	= pProgress->IsAllowInput( Input::Exposure	);
			allowToggle		= pProgress->IsAllowInput( Input::Toggle	);
		}
		if ( stageNumber < LIMIT_STAGE_NUMBER )
		{
			allowToggle		= false;
		}
		pCamera->Update( allowMove, allowExposure, allowToggle );

		if ( pCamera->IsExposure() && nextState == State::Null )
		{
			if ( Exposure() )
			{
				numMoves++;

				if ( pProgress )
				{
					pProgress->DoneConditions( ProgressStorage::Conditions::InputExposure );
				}
				else
				{
					UsedOperate();
				}
			}
		}
	}

	// 流れ星
	if ( pSSMng )
	{
		// 発生
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

	// 盤面
	if ( pStarMng )
	{
		pStarMng->Update();

	#if DEBUG_MODE
		if ( TRG( KEY_INPUT_SEMICOLON ) )
		{
			pStarMng->AlignLevelsDebug();
		}
	#endif // DEBUG_MODE
	}

	// アンドゥ
	if ( pStarMng && pCamera )
	{
		bool canUndo = isOpenFade && nextState == State::Null;
		if ( pProgress && canUndo ) // すでに使えない状態ならば通す意味はない
		{
			// 露光が使える == アンドゥも使える，とみなす
			canUndo = pProgress->IsAllowInput( ProgressStorage::BitInput::Exposure );
		}

		if ( IsTrigger( InputTrigger::Undo ) && canUndo )
		{
			if ( pStarMng->Undo(), pCamera->Undo()/* HACK:ちゃんと両方での成功を条件に取るべきである */ )
			{
				numMoves--;
				UsedOperate();
				ResetExposureCount();
				PlaySE( M_UNDO );
			}
			else // 露光のログが無くアンドゥできなかった場合
			{
				PlaySE( M_CANT_PUSH );
			}
		}
	}

	if ( pNumMoves )
	{
		pNumMoves->Update();
	}

	wink.Update();

	MilkyWayUpdate();

	RotatorUpdate();
	if ( pRotator )
	{
		RotateStars( *pRotator );
	}

	// 進める条件の確認
	if ( pProgress )
	{
		using Cond = ProgressStorage::Conditions;

		bool wantMove = IsTrigger( InputTrigger::Up ) || IsTrigger( InputTrigger::Down ) || IsTrigger( InputTrigger::Left ) || IsTrigger( InputTrigger::Right );
		if ( wantMove )
		{
			pProgress->DoneConditions( Cond::InputMove );
		}

		// 露光の条件は，露光成功時のみにしたいため，カメラの更新箇所にて見ている

		if ( IsTrigger( InputTrigger::ToggleCamera ) )
		{
			pProgress->DoneConditions( Cond::InputToggle );
		}
	}

	// 反応間連の台詞の条件の確認
	if ( nextState == State::Null && state == State::Game ) // ゲーム中のみ判断する
	{
		if ( !pProgress ) // チュートリアル時は出さない
		{
			ReactionUpdate();
		}
	}

	// Ｒボタンへの矢印の，生成・削除条件の確認
	if ( stageNumber == LIMIT_STAGE_NUMBER && !isUnlockedStage )
	{
		if ( textNumber == TextBehavior::TUTORIAL_TOGGLE_GENERATE_ARROW_TIMING )
		{
			if ( !pArrow )
			{
				pArrow = std::make_unique<HorizontalArrow>();
				pArrow->Reset();
				pArrow->SetPosition( arrowBasePos );
			}
		}
		else
		{
			pArrow.reset();
		}
	}

	if ( pArrow )
	{
		pArrow->Update();

	#if USE_IMGUI
		if ( FileIO::IsShowImGuiWindow() && ImGui::Begin( "Arrow to R" ) )
		{
			ImGui::DragFloat ( "Shaking Speed",  &arrowShakeSpeed,  0.1f );
			ImGui::DragFloat ( "Shaking Amount", &arrowShakeAmount );
			ImGui::DragFloat2( "Arrow Pos",      &arrowBasePos.x );
			pArrow->SetPosition( arrowBasePos );
			
			ImGui::End();
		}
	#endif // USE_IMGUI
	}

	BalloonUpdate();

#if USE_IMGUI

	FileIO::UpdateNowStageNumberByImGui();
	if ( FileIO::IsCreateNewStage() )
	{
		if ( pCamera   )
		{
			pCamera->SaveData();
		}
		if ( pStarMng  )
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
	constexpr int CLEAR_TIMER_MAX = ClearRelated::FADE_WAIT + ClearRelated::GOTO_NEXT_WAIT;

	bool skipPerformance = false;
	if ( IsTrigger( InputTrigger::Exposure ) )
	{
		bool nowSkippable = true; // フェードしきってから，クリア演出が終わる（「次へ」が出てくる）までの間のみスキップ可能
		if ( clearTimer < ClearRelated::FADE_WAIT )	{ nowSkippable = false; }
		if ( CLEAR_TIMER_MAX <= clearTimer )		{ nowSkippable = false; }

		if ( nowSkippable )
		{
			skipPerformance = true;
			clearTimer = CLEAR_TIMER_MAX;
		}
	}

	clearTimer++;
	clearTimer = std::min( CLEAR_TIMER_MAX, clearTimer );

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
		// 発生
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

	if ( ClearRelated::FADE_WAIT <= clearTimer && pBoard )
	{
		if ( skipPerformance )
		{
			pBoard->SkipPerformance();
		}

		pBoard->Update();
	}

	// RecordStarの生成管理
	if ( ClearRelated::FADE_WAIT <= clearTimer )
	{
		// HACK:星の数が３つじゃないなら，これも変える必要がある
		constexpr int RECORD_STAR_COUNT = 3;

		const std::array<int, RECORD_STAR_COUNT> generateFrames =
		{
			ClearRelated::FADE_WAIT + 50/* 基準 */,
			ClearRelated::FADE_WAIT + 50 + 30/* 間隔 */,
			ClearRelated::FADE_WAIT + 50 + ( 30 * 2 )
		};

		auto GenerateImpl = [&]( int nextGenerateIndex )
		{
			const float   interval = scast<float>( ( 160 + ClearImage::SIZE_STAR_X ) * nextGenerateIndex );
			const Vector2 base{ 602.0f + interval, 864.0f };

			int nowRank = pNumMoves->CalcRank( numMoves );	// 0始まり
			// 達成難易度は，右からの降順で並んでいるとする（左のほうが達成されやすい）
			bool isGlow =
				( nextGenerateIndex <= scast<int>( generateFrames.size() ) - 1 - nowRank )
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
		};

		const int nextGenerate = scast<int>( recordStars.size() ); // 生成するたびにサイズが増えていくことを利用
		if	(
				nextGenerate < scast<int>( generateFrames.size() ) &&
				clearTimer == generateFrames[nextGenerate]	// 短絡評価
			)
		{
			GenerateImpl( nextGenerate );
		}

		if ( skipPerformance )
		{
			while ( scast<int>( recordStars.size() ) < RECORD_STAR_COUNT )
			{
				// このラムダ式の中で recordStars のサイズが増えるため，無限ループにはならない
				GenerateImpl( scast<int>( recordStars.size() ) );
			}
			for ( auto &it : recordStars )
			{
				// ここで呼んでしまうと，二重生成防止条件の都合でパーティクル生成条件に引っかからなくなるため，あとで呼ぶ
				// it.SkipPerformance();
			}
		}
	}
	for ( RecordStar &it : recordStars )
	{
		it.Update();

		if ( it.IsGlow() && pParticleMng )
		{
			auto Generate = [&]()
			{
				pParticleMng->
					Generate( PARTICLE_AMOUNT, it.GetPos(), /* isBig = */ true );
			};

			constexpr int GENERATE_PARTICLE_TIME = 8;

			if ( skipPerformance )
			{
				// スキップするときは，まだ生成していない（生成フレームに満たない）もののみ生成
				if ( it.GetTimer() <  GENERATE_PARTICLE_TIME )
				{
					Generate();

					// ここで呼ぶと，IsGlow()が真の実体のみ呼ぶことになるため適さない
					// it.SkipPerformance();
				}
			}
			else
			{
				// スキップしないときは，生成フレームになったら生成
				if ( it.GetTimer() == GENERATE_PARTICLE_TIME )
				{
					Generate();
				}
			}
		}

		// ここなら，やりたかった二重生成の防止を達成したあとなので呼んでも大丈夫。ただし，無駄に毎フレーム入ってしまう
		if ( skipPerformance )
		{
			it.SkipPerformance();
		}
	}

	// 項目選択
	if ( isShowClearMenu && isOpenFade/* 項目を選びフェードが始まっていたら入力を受け付けない */ )
	{
		// PauseUpdateのものと同一
		constexpr int MAX_MENU = 4;

		bool isUp = false, isDown = false;

		if ( IsTrigger( InputTrigger::Up	) ) { isUp		= true; }
		if ( IsTrigger( InputTrigger::Down	) ) { isDown	= true; }

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

		if ( IsTrigger( InputTrigger::Exposure ) && nextState == State::Null )
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
	if ( !isShowClearMenu && ClearRelated::FADE_WAIT + ClearRelated::GOTO_NEXT_WAIT <= clearTimer )
	{
		const int   DESTINATION = SCREEN_WIDTH - ( ClearImage::SIZE_GOTO_NEXT_X + ClearRelated::GOTO_NEXT_SPACE );
		const float RESISTANCE = 0.3f;

		float dist = fabsf( scast<float>( DESTINATION - gotoNextPosX ) );
		gotoNextPosX -= scast<int>( dist * RESISTANCE );

		if (gotoNextPosX < DESTINATION)
		{
			gotoNextPosX = DESTINATION;
		}

		if ( IsTrigger( InputTrigger::Exposure ) && !skipPerformance/* スキップした瞬間もボタンを押していることになり，そのままこの条件にも入るが，それは意図しない */ && isOpenFade/* 強制的にセレクト画面へ戻す際の連打防止 */ )
		{
			PlaySE( M_DECISION );

			// 制限を解放するステージの場合，必ずセレクト画面にもどす
			if ( !isUnlockedStage && stageNumber == LIMIT_STAGE_NUMBER )
			{
				nextState = State::Select;
				FadeBegin();
			}
			else
			{
				isShowClearMenu = true;
			}
		}
	}

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

#define USE_IMGUI_FOR_ROTATOR ( true && DEBUG_MODE && USE_IMGUI )
#if USE_IMGUI_FOR_ROTATOR
namespace ROTATOR
{
	static int   destroyFrame	= 110;
	static float generatePos	= 2048.0f;
	static float lineWidth		= 128.0f;
	static float moveSpeed		= -24.0f;
}
#endif // USE_IMGUI_FOR_ROTATOR
void Game::GenerateRotator()
{
	// HACK: もし同時に複数個出る可能性があるなら，unique_ptrではなくvectorにする
	// 現在は存在していたものは消されて，上書きされるようになっている

#if USE_IMGUI_FOR_ROTATOR
	pRotator = std::make_unique<Rotator>( ROTATOR::destroyFrame, ROTATOR::generatePos, ROTATOR::lineWidth, ROTATOR::moveSpeed );
#else
	constexpr int   DESTROY_FRAME	= 110;
	constexpr float GENERATE_POS	= 2048.0f;
	constexpr float LINE_WIDTH		= 128.0f;
	constexpr float MOVE_SPEED		= -24.0f;
	pRotator = std::make_unique<Rotator>( DESTROY_FRAME, GENERATE_POS, LINE_WIDTH, MOVE_SPEED );
#endif // USE_IMGUI_FOR_ROTATOR
}
void Game::RotatorUpdate()
{
#if USE_IMGUI_FOR_ROTATOR
	if ( FileIO::IsShowImGuiWindow() )
	{
		ImGui::Begin( "Rotator" );

		ImGui::SliderInt  ( "DestroyFrame",	&ROTATOR::destroyFrame,	1,		1024	);
		ImGui::SliderFloat( "GeneratePos",	&ROTATOR::generatePos,	0.0f,	1920.0f	);
		ImGui::SliderFloat( "Line Width",	&ROTATOR::lineWidth,	0.1f,	32.0f	);
		ImGui::DragFloat  ( "Move Speed",	&ROTATOR::moveSpeed,	0.5f	);

		ImGui::End();
	}
#endif // USE_IMGUI_FOR_ROTATOR

	if ( !pRotator ) { return; }
	// else

	pRotator->Update();

	if ( pRotator->ShouldRemove() )
	{
		pRotator.reset();
	}
}
void Game::RotatorDraw()
{
#if DEBUG_MODE

	if ( !pRotator ) { return; }
	// else

	if ( !PRESS_SHIFT ) { return; }
	// else

	pRotator->DrawHitBox( 128, 128, 182 );

#endif // DEBUG_MODE
}
#undef USE_IMGUI_FOR_ROTATOR
void Game::RotateStars( const Rotator &rotator )
{
	if ( !pStarMng ) { return; }
	// else

	const Box rotatorBody = rotator.GetHitBox();

	Box star{};
	
	const int starCount = pStarMng->GetArraySize();
	for ( int i = 0; i < starCount; ++i )
	{
		star = pStarMng->FetchColWorldPos( i );
		if ( Box::IsHitBox( star, rotatorBody ) )
		{
			pStarMng->Rotate( i );
		}
	}
}

void Game::BalloonUpdate()
{
	textTimer++;

	// フキダシの枠部分の伸び縮み
	auto UpdateBalloon = [&]()
	{
		constexpr float LOWER_REMAIN		= 12.0f;	// 残りがこれ以下であれば，一気に限界値にする
		constexpr float STRETCH_RESISTANCE	= 0.3f;		// 差分に掛け算して，変化量に動きをもたせる

		if ( isOpenBalloon )	// ひらく
		{
			if ( balloonLength < HumanImage::SIZE_BALLOON_X )
			{
				float remaining = scast<float>( HumanImage::SIZE_BALLOON_X - balloonLength );

				if ( remaining <= LOWER_REMAIN )
				{
					balloonLength = HumanImage::SIZE_BALLOON_X;
				}
				else
				{
					balloonLength += scast<int>( remaining * STRETCH_RESISTANCE );

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

			if ( remaining <= LOWER_REMAIN )
			{
				balloonLength = 0;
			}
			else
			{
				balloonLength -= scast<int>( remaining * STRETCH_RESISTANCE );

				if ( balloonLength < 0 )
				{
					balloonLength = 0;
				}
			}
		}
	};

	// フキダシの更新，これは必ず通す
	if ( 0 != balloonLength )
	{
		UpdateBalloon();
	}

	// クリア台詞
	if ( nextState == State::Clear || state == State::Clear )
	{
		// 表示時間経過確認
		if ( 0 != textLength )
		{
			int  showFrame =  TextBehavior::CLEAR_SAY_SHOW_FRAME[textNumber];
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

	// チュートリアル台詞
	if ( pProgress )
	{
		const bool isFirstVer = ( stageNumber == 1 );

		const std::vector<std::string>	&baseTexts = ( isFirstVer )
			? TextBehavior::TUTORIAL
			: TextBehavior::TUTORIAL_TOGGLE;
		const std::vector<int>			&showFrames = ( isFirstVer )
			? TextBehavior::TUTORIAL_SHOW_FRAME
			: TextBehavior::TUTORIAL_TOGGLE_SHOW_FRAME;

		constexpr int TUTORIAL_TEXT_START_FRAME		= 80; // フェードのための待ち時間
		constexpr int TUTORIAL_BALLOON_START_FRAME	= TUTORIAL_TEXT_START_FRAME - 20; // フキダシが出てからテキストを表示するための差分
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
			if ( textNumber < scast<int>( baseTexts.size() ) - 1 )
			{
				int sumFrame = 0;
				for ( int i = 0; i <= textNumber; i++ )
				{
					sumFrame += showFrames[i];
				}

				bool isOverCurrentShowFrame = sumFrame <= textTimer - TUTORIAL_TEXT_START_FRAME;
				if ( isOverCurrentShowFrame )
				{
					 pProgress->DoneConditions( ProgressStorage::Conditions::WaitingTime );
				}

				if ( pProgress->IsCompleteNowConditions() )
				{
					// 時間条件は満たしているが他を満たしていない場合に増えすぎてしまうため，正しいタイマーに補正する
					textTimer = sumFrame + TUTORIAL_TEXT_START_FRAME;

					textLength = 1;
					textExtendInterval = 0;

					textNumber++;
					pProgress->AdvanceProgress();
				}
			}
			else // else文にすると，すべて表示した後でないとリセットできないようになる
			if ( IsTrigger( InputTrigger::Select ) )
			{
				textTimer  = TUTORIAL_TEXT_START_FRAME + showFrames[0];

				textLength = 1;
				textExtendInterval = 0;

				textNumber = 1;

				pProgress->ResetProgress();
				pProgress->AdvanceProgress(); // リセット関数ではゼロになるが，戻したいのは１のため，ひとつ進める
			}
		}

		// 文字数増加確認
		if ( 0 != textLength && ( textLength * 2 ) <= scast<int>( baseTexts[textNumber].size() ) )
		{
			constexpr int INTERVAL = 3;
			textExtendInterval++;
			if ( INTERVAL <= textExtendInterval )
			{
				textExtendInterval = 0;
				textLength++;

				// 口変化
				{
					if ( scast<int>( baseTexts[textNumber].size() ) <= ( textLength * 2 ) )
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

	// 反応関連の台詞
	if ( isTalkReaction )
	{
		// 発言トリガーは別の場所で行い，ここではその更新のみ行う
		
		// 表示時間経過確認
		if ( 0 != textLength )
		{
			int  showFrame =  TextBehavior::REACTION_SAY_SHOW_FRAME[textNumber];
			if ( showFrame <= textTimer )
			{
				textLength = 0;
				textExtendInterval = 0;

				isOpenBalloon  = false;
				isTalkReaction = false;
			}
		}

		// 文字数増加確認
		if ( 0 != textLength && ( textLength * 2 ) <= scast<int>( TextBehavior::REACTION_SAY[textNumber].size() ) )
		{
			constexpr int INTERVAL = 3;
			textExtendInterval++;
			if ( INTERVAL <= textExtendInterval )
			{
				textExtendInterval = 0;
				textLength++;

				// 口変化
				{
					if ( scast<int>( TextBehavior::REACTION_SAY[textNumber].size() ) <= ( textLength * 2 ) )
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

	// それ以外のランダム発言

	constexpr int INTERVAL_SECONDS	= 20;
	constexpr int SAY_INTERVAL		= INTERVAL_SECONDS * 60;
	int remTimer = textTimer % SAY_INTERVAL;
	{
		constexpr int TEXT_START_FRAME		= 80;
		constexpr int BALLOON_START_FRAME	= TEXT_START_FRAME - 20;
		// 何もしゃべっていないときからのランダム生成
		if ( state == State::Game )	// クリア後は新しくは出さない
		{
			if ( remTimer == TEXT_START_FRAME )
			{
				textLength = 1; // 更新条件である Length != 0 を突破する最小値にする
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
			int  showFrame =  TextBehavior::RAND_SAY_SHOW_FRAME[textNumber];
			if ( showFrame <= remTimer - TEXT_START_FRAME )
			{
				textLength = 0;
				textExtendInterval = 0;

				textNumber++; // HACK:これいる？

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

void Game::TalkReaction( int textIndex )
{
	// チュートリアル中は出さない
	if ( stageNumber == 1 || ( stageNumber == LIMIT_STAGE_NUMBER && !isUnlockedStage ) ) { return; }
	// else

	assert( 0 <= textIndex && textIndex < TextBehavior::Reactions::REACTION_END );

	// クリア時の台詞表示処理を参照。
	// フキダシが伸び切るのを待っていないという問題があるが，今回は見過ごす

	if ( !balloonLength )
	{
		constexpr int INIT_LENGTH = 64;
		balloonLength = INIT_LENGTH;
	}

	textTimer			= 0; // すでにしゃべっている最中でも，表示時間を設定通りに動作させるため。textTimerをいじること自体は問題ないと判断
	textLength			= 1;
	textExtendInterval	= 0;
	textNumber			= textIndex;

	isOpenBalloon		= true;
	isTalkReaction		= true;
}
namespace Reaction
{
	static constexpr int sayBorderCountSucceed	= 8;
	static constexpr int sayBorderCountFailed	= 3;
	static constexpr int sayBorderFrameSucceed	= 5 * 60;
	static constexpr int sayBorderFrameFailed	= 5 * 60;
	static constexpr int sayBorderFrameOperated	= 10 * 60;
}
void Game::ReactionUpdate()
{
	timeSinceSucceed++;
	timeSinceFailed++;
	timeSinceOperated++;

	if ( Reaction::sayBorderFrameSucceed <= timeSinceSucceed )
	{
		timeSinceSucceed	= 0;
	}
	if ( Reaction::sayBorderFrameFailed <= timeSinceFailed )
	{
		timeSinceFailed		= 0;
	}
	if ( Reaction::sayBorderFrameOperated <= timeSinceOperated )
	{
		timeSinceOperated	= 0;

		constexpr std::array<TextBehavior::Reactions, 5> UHHH_TEXTS
		{
			TextBehavior::Reactions::WhyDontYouUndo,
			TextBehavior::Reactions::Uhhh_1,
			TextBehavior::Reactions::Uhhh_2,
			TextBehavior::Reactions::Uhhh_3,
			TextBehavior::Reactions::Uhhh_4,
		};
		int textIndex = rand() % UHHH_TEXTS.size();
		TalkReaction( UHHH_TEXTS[textIndex] );
	}
}
void Game::UsedExposure( bool succeeded )
{
	using Reactions = TextBehavior::Reactions;

	// 最後に行った動作と違っていたら，タイマとカウンタをリセットする
	auto ResetStatus = [&]()
	{
		failedCounter		= 0;
		timeSinceFailed		= 0;
		succeedCounter		= 0;
		timeSinceSucceed	= 0;
	};

	if ( succeeded )
	{
		if ( !lastResult )
		{
			ResetStatus();
		}

		succeedCounter++;

		bool isOverBorder = Reaction::sayBorderCountSucceed <= succeedCounter;
		bool isOverFastly = timeSinceSucceed < Reaction::sayBorderFrameSucceed;
		if ( isOverBorder && isOverFastly )
		{
			succeedCounter   = 0;
			timeSinceSucceed = 0;

			constexpr std::array<Reactions, 2> GOOD_TEXTS
			{
				Reactions::GoodVibes_1,
				Reactions::GoodVibes_2,
			};
			int textIndex = rand() % GOOD_TEXTS.size();
			TalkReaction( GOOD_TEXTS[textIndex] );
		}
	}
	else
	{
		if ( lastResult )
		{
			ResetStatus();
		}

		failedCounter++;

		bool isOverBorder = Reaction::sayBorderCountFailed <= failedCounter;
		bool isOverFastly = timeSinceFailed < Reaction::sayBorderFrameFailed;
		if ( isOverBorder && isOverFastly )
		{
			failedCounter   = 0;
			timeSinceFailed = 0;

			TalkReaction( Reactions::CantExposure );
		}
	}

	lastResult = succeeded;
}
void Game::UsedOperate()
{
	timeSinceOperated = 0;
}
void Game::ResetExposureCount()
{
	succeedCounter   = 0;
	timeSinceSucceed = 0;
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

	// 制限を解放するステージだった場合。これは下のif文より先にみないといけない（下のif文によりステージ番号がインクリメントされると，そのままこちらも通ってしまう）
	if ( !isUnlockedStage && stageNumber == LIMIT_STAGE_NUMBER && state == State::Clear )
	{
		isUnlockedStage  = true;
		pUnlockAnnouncer = std::make_unique<UnlockAnnouncer>();
		stageNumber += 1; // 次のステージにカーソルを合わせておくため
		FileIO::ResetStageLimit();
	}

	// 「次のステージ」を選んだ場合
	if ( isShowClearMenu && state == State::Clear && nextState == State::Game )
	{
		if ( choice == 0 &&  stageNumber < FileIO::GetMaxStageNumber() )
		{
			stageNumber++;
		}
	}

	state		= nextState;
	nextState	= State::Null;

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

	isPause		= false;
	isOpenFade	= false;
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

	const Box camera = pCamera->FetchColWorldPos();

	std::vector<int>		targets{};		// 露光の適用対象たち
	std::vector<Vector2>	particlePos{};

	constexpr int VIBRATE_POWER = 400;
	constexpr int VIBRATE_TIME  = 100; // m/sec

	// 適用番号の検査
	bool isCovered = false;	// 星が一つでもカメラの枠内に入っていれば真
	const int end = pStarMng->GetArraySize();
	for ( int i = 0; i < end; i++ )
	{
		Box star = pStarMng->FetchColWorldPos( i );
		if ( Box::IsHitBox( camera, star ) )
		{
			isCovered = true;

			// １が入っている場合，使えない
			if ( pStarMng->FetchLevel( i ) <= 1 )
			{
				pCamera->SetShake();
				StartJoypadVibration( DX_INPUT_PAD1, VIBRATE_POWER, VIBRATE_TIME );
				UsedExposure( /* succeeded = */ false );
				PlaySE( M_FAILURE );

				return false;
			}
			// else
			if ( IsInsideStarFourCorners( camera, star ) )
			{
				targets.push_back( i );
				particlePos.push_back( { star.cx, star.cy } );
			}
			else // カメラの枠が星に触れてはいるが，星のサイズが大きく，はみだしている場合に入る（露光は失敗する）
			{
				pCamera->SetShake();
				StartJoypadVibration( DX_INPUT_PAD1, VIBRATE_POWER, VIBRATE_TIME );
				UsedExposure( /* succeeded = */ false );
				PlaySE( M_FAILURE );

				return false;
			}
		}
	}

	// 星をまったく収めていなければ，失敗として終わる
	if ( !isCovered )
	{
		pCamera->SetShake();
		StartJoypadVibration( DX_INPUT_PAD1, VIBRATE_POWER >> 1, VIBRATE_TIME >> 1 );
		// 「それ以上は露光できない」とは言えないためはぶく
		// UsedExposure( /* succeeded = */ false );
		PlaySE( M_FAILURE );
		return false;
	}
	// else

	// HACK:上のfor文内でreturnを使っているので，この条件には入ることがない？
	// 星を収めてはいるものの，失敗していたら終わる
	if ( !scast<int>( targets.size() ) || !end )
	{
		UsedExposure( /* succeeded = */ false );
		return false;
	}
	// else

	// 以下，成功

	pCamera->SetGlow();
	UsedExposure( /* succeeded = */ true );

	// 露光音は，成功が確定してから鳴らす（失敗時の音もあるため）
	PlaySE( M_EXPOSURE );

	// アンドゥ用
	pStarMng->SaveLog();
	pCamera->SaveLog();

	// 適用
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
			!IsTrigger( InputTrigger::Pause )	|| // 要求：ポーズボタンが押された瞬間である
			state != State::Game				|| // 要求：ゲームシーンである
			!isOpenFade							|| // 要求：フェード中ではない
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

	// 項目を選びフェードが始まっていたら入力を受け付けない
	if ( !isOpenFade ) { return; }
	// else

	// choice は 0 始まりである

	constexpr int MAX_MENU = 4;

	bool isUp = false, isDown = false;

	if ( IsTrigger( InputTrigger::Up	) ) { isUp		= true; }
	if ( IsTrigger( InputTrigger::Down	) ) { isDown	= true; }

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

	if ( IsTrigger( InputTrigger::Exposure ) && nextState == State::Null )
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

	if ( pUnlockAnnouncer )
	{
		int bright = pUnlockAnnouncer->GetDrawBright();
		SetDrawBright( bright, bright, bright );
	}

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

	// 流れ星
	if ( pSSMng )
	{
		pSSMng->Draw( shake );
	}

	if ( pParticleMng )
	{
		pParticleMng->Draw( shake );
	}

	// 文字
	{
		// セレクト
		DrawGraph
		(
			scast<int>( selectPos.x ),
			scast<int>( selectPos.y ),
			SelectImage::hStatement,
			TRUE
		);
		// 操作方法
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

	if ( pUnlockAnnouncer )
	{
		SetDrawBright( 255, 255, 255 );

		pUnlockAnnouncer->Draw();
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

	}

	// 流れ星
	if ( pSSMng )
	{
		pSSMng->Draw( shake );
	}

	// 天の川
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

	// 枠のUI
	//if ( !isPause )
	{
		DrawGraph
		(
			0, 0,
			GameImage::hFrameUI,
			TRUE
		);

		if ( LIMIT_STAGE_NUMBER <= stageNumber )
		{
			DrawGraph
			(
				SCREEN_WIDTH  - GameImage::WIDTH_USAGE_R,
				SCREEN_HEIGHT - GameImage::HEIGHT_USAGE_R,
				GameImage::hUsageR,
				TRUE
			);
		}
	}

	// 手数目標
	if ( pNumMoves )
	{
		int x = 0, y = 0;
		GetMousePoint( &x, &y );
		
		constexpr int BASE_X = 100;
		constexpr int BASE_Y = 360;

		constexpr int INTERVAL = ClearImage::SIZE_STAR_Y >> 1;

		std::vector<int> dataAscendingOrder = pNumMoves->GetData();
		int content = -1;
		int nowRank = pNumMoves->CalcRank( numMoves );	// 0 始まり
		int glowImageIndex = -1;
		size_t end = dataAscendingOrder.size();
		for ( size_t i = 0; i < end; ++i )
		{
			content = dataAscendingOrder[i];

			// 星
			{
				glowImageIndex = ( scast<size_t>( nowRank ) <= i ) ? 1 : 0;
				DrawRotaGraph
				(
					BASE_X,
					BASE_Y + ( INTERVAL * i ) + 8/* 星の中心が画像の中心ではないので，少しずらしている */,
					0.5,
					ToRadian( 0 ),
					ClearImage::hRecordStar[glowImageIndex],
					TRUE
				);
			}

			// 手数
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

	if ( nextState != State::Clear && pCamera )
	{
		pCamera->Draw( shake );
	}
	
	// 人
	{
		// 体
		DrawGraph
		(
			0,
			SCREEN_HEIGHT - HumanImage::SIZE_Y,
			HumanImage::GetBodyHandle( wink.GetAnimeIndex() ),
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
			HumanImage::GetArmHandle( NULL ),
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
		TextDraw();
	}
	// Ｒボタンへの矢印
	if ( pArrow )
	{
		pArrow->Draw( GameImage::hArrow );
	}

#if DEBUG_MODE
	RotatorDraw();
#endif // DEBUG_MODE
}

void Game::ClearDraw()
{
	Vector2 shake = GetShakeAmount();

	// フェードアウトしていくものたち
	if ( !isShowClearMenu )
	{
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

		}

		// 天の川
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

		// 枠のUI
		{
			DrawGraph
			(
				0, 0,
				GameImage::hFrameUI,
				TRUE
			);
			if ( LIMIT_STAGE_NUMBER <= stageNumber )
			{
				DrawGraph
				(
					SCREEN_WIDTH  - GameImage::WIDTH_USAGE_R,
					SCREEN_HEIGHT - GameImage::HEIGHT_USAGE_R,
					GameImage::hUsageR,
					TRUE
				);
			}
		}

		// 手数目標
		if ( pNumMoves )
		{
			int x = 0, y = 0;
			GetMousePoint( &x, &y );

			constexpr int BASE_X = 100;
			constexpr int BASE_Y = 360;

			constexpr int INTERVAL = ClearImage::SIZE_STAR_Y >> 1;

			std::vector<int> dataAscendingOrder = pNumMoves->GetData();
			int content = -1;
			int nowRank = pNumMoves->CalcRank( numMoves );	// 0 始まり
			int glowImageIndex = -1;
			size_t end = dataAscendingOrder.size();
			for ( size_t i = 0; i < end; ++i )
			{
				content = dataAscendingOrder[i];

				// 星
				{
					glowImageIndex = ( scast<size_t>( nowRank ) <= i ) ? 1 : 0;
					DrawRotaGraph
					(
						BASE_X,
						BASE_Y + ( INTERVAL * i ) + 8/* 星の中心が画像の中心ではないので，少しずらしている */,
						0.5,
						ToRadian( 0 ),
						ClearImage::hRecordStar[glowImageIndex],
						TRUE
					);
				}

				// 手数
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

		// 星
		if ( pStarMng )
		{
			pStarMng->Draw( shake );
		}

		// 人
		if ( 0 ) // フェードアウトさせないようにした
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

		GameDrawUI();
	}

	constexpr int NOT_USE_BLEND = -1;
	int blendAlpha = NOT_USE_BLEND;
	if ( isShowClearMenu )
	{
		SetDrawBright( 64, 64, 64 );
	}
	else if ( clearTimer < 255 )
	{
		blendAlpha = ( 255 / ClearRelated::FADE_WAIT )/* AMPL */ * clearTimer;
	}
	else
	{
		blendAlpha = 255;
	}

	if ( blendAlpha != NOT_USE_BLEND )
	{
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, blendAlpha );
	}

	// BG
	DrawGraph
	(
		0, 0,
		ClearImage::hClearBG,
		TRUE
	);

	// 流れ星
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

	// 人（フェードとは無関係にするため，内部でブレンドモードをいったんいじっている）
	{
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 );

		// 体
		DrawGraph
		(
			0,
			SCREEN_HEIGHT - HumanImage::SIZE_Y,
			HumanImage::GetBodyHandle( wink.GetAnimeIndex() ),
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
			HumanImage::GetArmHandle( NULL ),
			TRUE
		);

		SetDrawBlendMode( DX_BLENDMODE_ALPHA, blendAlpha );
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

	if ( clearTimer < ClearRelated::FADE_WAIT ) { return; }
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

	if ( !isShowClearMenu ) { return; }
	// else

	bool isFinalStage =
		( stageNumber == FileIO::GetMaxStageNumber() )
		? true
		: false;

	// 操作方法
	DrawGraph
	(
		0, 0,
		SelectImage::hUsage,
		TRUE
	);

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

void Game::TextDraw()
{
	// 現在 textLength があるかどうかは呼び出し側で判断している

	// クリア台詞
	if ( nextState == State::Clear )
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

		return;
	}
	// else

	// チュートリアル台詞
	if ( stageNumber == 1 || ( stageNumber == LIMIT_STAGE_NUMBER && !isUnlockedStage ) )
	{
		const bool isFirstVer = ( stageNumber == 1 );

		const std::vector<std::string>	&baseTexts = ( isFirstVer )
			? TextBehavior::TUTORIAL
			: TextBehavior::TUTORIAL_TOGGLE;
		const std::vector<int>			&showFrames = ( isFirstVer )
			? TextBehavior::TUTORIAL_SHOW_FRAME
			: TextBehavior::TUTORIAL_TOGGLE_SHOW_FRAME;

		int index = textNumber % scast<int>( baseTexts.size() );
		int length = textLength * 2/* 日本語で２バイト文字なので，倍にして対応 */;
		if ( scast<int>( baseTexts[index].size() ) <= textLength )
		{
			length = scast<int>( baseTexts[index].size() );
		}

		constexpr int DIST_X	= 80;
		constexpr int DIST_Y	= 52;
		constexpr double MAGNI	= 2.0;

		int emphasisPos = 0, i = 0;
		for ( ; i < scast<int>( TextBehavior::EMPHASIS_STR.size() ); i++ )
		{
			emphasisPos = baseTexts[index].find( TextBehavior::EMPHASIS_STR[i] );
			if ( emphasisPos != std::string::npos )
			{
				break;
			}
		}

		// ループが終わったら，見つからなかったということになる
		if ( i == scast<int>( TextBehavior::EMPHASIS_STR.size() ) )
		{
			DrawExtendStringToHandle
			(
				HumanBehavior::BALLOON_POS_X + DIST_X,
				HumanBehavior::BALLOON_POS_Y + DIST_Y,
				MAGNI, MAGNI,
				( baseTexts[index].substr( 0, length ) ).c_str(),
				GetColor( 42, 97, 110 ),
				hFont
			);

			return;
		}
		// else

		// HACK:文字列と色の違いを除けばほとんど同じ処理なので，ループ文にまとめられるかも？

		if ( length <= 0 ) { return; }
		// else

		int charWidth = 0;
		std::string text = "";

		// 強調する文字列まで
		text = baseTexts[index].substr( 0, std::min( length, emphasisPos ) );

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

		if ( length <= 0 ) { return; }
		// else

		// 強調文字列
		text = baseTexts[index].substr( emphasisPos, std::min( length, scast<int>( TextBehavior::EMPHASIS_STR[i].size() ) ) );

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

		if ( length <= 0 ) { return; }
		// else

		// その後の文字列
		text = baseTexts[index].substr( emphasisPos + TextBehavior::EMPHASIS_STR[i].size(), length );

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

	// ランダムまたは反応関連の台詞

	// 使用する台詞配列のポインタ
	const std::vector<std::string> *pTexts = &TextBehavior::RAND_SAY;
	if ( isTalkReaction )
	{
		pTexts = &TextBehavior::REACTION_SAY;
	}

	int index  = textNumber % scast<int>( pTexts->size() );
	int length = textLength * 2/* 日本語で２バイト文字なので，倍にして対応 */;
	const std::string &useText = pTexts->at( index );
	if ( scast<int>( useText.size() ) <= textLength )
	{
		length = scast<int>( useText.size() );
	}

	constexpr int DIST_X = 80;
	constexpr int DIST_Y = 52;

	DrawExtendStringToHandle
	(
		HumanBehavior::BALLOON_POS_X + DIST_X,
		HumanBehavior::BALLOON_POS_Y + DIST_Y,
		2.0, 2.0,
		( useText.substr( 0, length ) ).c_str(),
		GetColor( 42, 97, 110 ),
		hFont
	);
}

void Game::PauseDraw()
{
	// 操作方法
	DrawGraph
	(
		0, 0,
		SelectImage::hUsage,
		TRUE
	);

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
	// ステージ数
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
				POS_X + tweakX + ( ( ( MAX_DIGIT - 1 ) - digit ) * ( Number::SIZE_X >> 1 )/* 字間 */ ),
				POS_Y,
				POS_X + ( Number::SIZE_X >> 1 ) + tweakX + ( ( ( MAX_DIGIT - 1 ) - digit ) * ( Number::SIZE_X >> 1 )/* 字間 */ ),
				POS_Y + ( Number::SIZE_Y >> 1 ),
				Number::GetHandle( stgNum % 10, true ),
				TRUE
			);

			stgNum /= 10;
		}
	}

	// 手数
	{
		//DrawGraph
		//(
		//	FRAME_POS_X - 160,
		//	FRAME_POS_Y,
		//	GameImage::hMovesUI,
		//	TRUE
		//);

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