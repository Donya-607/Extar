#ifndef INCLUDED_GAME_H_
#define INCLUDED_GAME_H_

#include <memory>
#include <vector>

#include "Scene.h"
#include "SceneManager.h"

#include "Particle.h"

#include "Camera.h"
#include "Star.h"
#include "NumMoves.h"
#include "Cursor.h"

#include "Board.h"

#include "ShootingStar.h"

#include "ProgressStorage.h"
#include "Rotator.h"
#include "UnlockAnnouncer.h"

//--------------------
//
//		Game.h
//
//--------------------

class RecordStar
{
private:
	static constexpr int TIMER_MAX = 999; // 最大値識別用
private:
	int   timer;

	float angle;		// Degree
	float scale;
	float rotateSpd;
	float magniSpd;		// 拡大縮小

	Vector2 pos;		// Center

	bool isGlow;
public:
	RecordStar() : timer( 0 ),
		angle( 0 ), scale( 0 ),
		rotateSpd( 0 ), magniSpd( 0 ),
		pos(),
		isGlow( false )
	{}
	~RecordStar() {}

	void Init( Vector2 centerPos, bool isGlowStar );

	void Update();
	int  GetTimer() const
	{
		return timer;
	}
	Vector2 GetPos() const
	{
		return pos;
	}

	void Draw(  Vector2 shake ) const;
public:
	bool IsGlow() const { return isGlow; }
public:
	void SkipPerformance();
};

class Game : public Scene
{
private:
	class Wink
	{
	private:
		static constexpr int ANIME_SPD = 3;
	private:
		int waitFrame;
		int timer;
		int animIndex;
	public:
		Wink() : waitFrame( 0 ), timer( 0 ), animIndex( 0 ) {}
	public:
		void Init();
		void Uninit();
		void Update();
		int  GetAnimeIndex() const { return animIndex; }
	private:
		void Lottering();
		void Wait();
		void Animation();
	};
private:
	enum class State : int
	{
		Select = 0,
		Game,
		Clear,
		GotoTitle,
		Null
	};
private:
	int  stageNumber;	// 1始まり
	int  numMoves;		// 今の手数，0始まり，アンドゥで減る

	int  selectTimer;

	int  sStarState;	// 0:明るく, 1:暗く
	int  sStarTimer;

	int  pauseTimer;
	int  choice;		// 0始まり, ポーズとリザルトを兼ねている

	int  clearTimer;
	int  hScreenShot;

	int  hFont;			// JFドットK12
	int  mouthIndex;	// 0始まり
	int  balloonLength;	// 0 のときは更新しない
	int  textTimer;		// テキスト関連で使用
	int  textLength;	// 0 のときは更新しない
	int  textExtendInterval; // １文字増やすのに使う間隔用のタイマ
	int  textNumber;	// 0始まり

	// 反応関連の台詞用の変数群
	// ここでの「失敗」は「明るさ最大の星に対して露光を使った」場合のことを指す
	int  succeedCounter;	// 露光が連続で成功した回数
	int  timeSinceSucceed;	// 露光が成功してからの経過フレーム（失敗か一定時間経過でリセット，成功ではそのまま）
	int  failedCounter;		// 露光が連続で失敗した回数
	int  timeSinceFailed;	// 露光が失敗してからの経過フレーム（成功か一定時間経過でリセット，失敗ではそのまま）
	bool lastResult;		// 最後の露光の結果を格納（上の４つの変数では判断がしづらかったため）
	int  timeSinceOperated;	// 最後の操作（移動除く）からの経過フレーム（移動以外の操作をするか一定時間経過でリセット）

	int  gotoNextPosX;	// LeftTop, リザルト画面で使用

	State state;
	State nextState;

	Wink wink;

	Vector2 selectPos;	// LeftTop

	Vector2 armPos;		// LeftTop

	Vector2 pausePos;	// LeftTop

	std::unique_ptr<ShootingStarMng> pSSMng;
	std::unique_ptr<ParticleMng> pParticleMng;

	std::unique_ptr<Cursor>   pCursor;
	std::unique_ptr<UnlockAnnouncer> pUnlockAnnouncer;

	std::unique_ptr<Camera>   pCamera;
	std::unique_ptr<StarMng>  pStarMng;
	std::unique_ptr<NumMoves> pNumMoves;
	std::unique_ptr<Rotator>  pRotator;
	std::unique_ptr<ProgressStorage> pProgress;

	std::unique_ptr<Board>	  pBoard;
	std::vector<RecordStar>	  recordStars;

	bool isOpenFade;	// FadeBegin ~ FadeEnd までFALSE

	bool isOpenBalloon;	// テキストボックスの更新挙動 TRUE:開く, FALSE:閉じる
	bool isTalkReaction;// 反応関連のセリフをしゃべっている間はTRUE

	bool isClearMoment;	// これがオンならスクショを取り，オフにしてクリアへ遷移させる
	bool isTakeScreenShot;
	bool isDoneMoveArm;

	bool isShowClearMenu;

	bool isPause;
	bool isDrawCollision;
	bool isUnlockedStage;// 制限している最大のステージをクリアしたら真。
private:	// 他ＰＧによる作業
	Vector2 str_up_pos;
	Vector2 str_down_pos;
	Vector2 str_speed;
	bool shutter_flag;
	int shutter_state;

	int DrawStart_X;
	int DrawStart_Y;
	int Src_X;
	int Src_Y;
	float Wid;
	float Hgh;

	float Src_Down_Y;
	float Hgh_Down;
public:
	Game( SceneMng *pMng ) : Scene( pMng ),
		stageNumber( 1 ), numMoves( 0 ),
		selectTimer( 0 ),
		sStarState( 0 ), sStarTimer( 0 ),
		pauseTimer( 0 ),
		choice( 0 ),
		clearTimer( 0 ),
		hScreenShot( 0 ), hFont( 0 ),
		mouthIndex( 0 ),
		balloonLength( 0 ),
		textTimer( 0 ),
		textLength( 0 ), textExtendInterval( 0 ), textNumber( 0 ),
		succeedCounter( 0 ), timeSinceSucceed( 0 ),
		failedCounter( 0 ), timeSinceFailed( 0 ),
		lastResult( false ),
		timeSinceOperated( 0 ),
		gotoNextPosX( SCREEN_WIDTH ),
		state( State::Select ), nextState( State::Null ),
		wink(),
		selectPos(),
		armPos( { scast<float>( SCREEN_WIDTH ), scast<float>( SCREEN_HEIGHT ) } ),
		pausePos(),
		pSSMng( nullptr ), pParticleMng( nullptr ),
		pCursor( nullptr ), pUnlockAnnouncer( nullptr ),
		pCamera( nullptr ), pStarMng( nullptr ), pNumMoves( nullptr ),
		pRotator( nullptr ), pProgress( nullptr ),
		pBoard( nullptr ), recordStars(),
		isOpenFade( false ),
		isOpenBalloon( true ), isTalkReaction( false ),
		isClearMoment( false ), isTakeScreenShot( false ),
		isDoneMoveArm( false ),
		isShowClearMenu( false ),
		isPause( false ), isDrawCollision( false ),
		isUnlockedStage( false ),

		str_up_pos( { 0, 0 } ),
		str_down_pos( { 192.0f, 64.0f } ),
		str_speed( { 0.0f, 50.0f } ),
		shutter_flag( false ),
		shutter_state( 0 ),

		DrawStart_X( 190 ),
		DrawStart_Y( 60 ),
		Src_X( 14 ),
		Src_Y( 150 ),
		Hgh( 0 ),
		Wid( 1538.0f ),

		Src_Down_Y( -790.0f ),
		Hgh_Down( 780.0f )

		{}
	~Game() { std::vector<RecordStar>().swap( recordStars ); }

	void Init();
	void SelectInit();
	void GameInit();
	void ClearInit();

	void Uninit();
	void SelectUninit();
	void GameUninit();
	void ClearUninit();

	void Update();
	void SelectUpdate();
	void GameUpdate();
	void ClearUpdate();

	void MilkyWayUpdate();

	void GenerateRotator();
	void RotatorUpdate();
	void RotatorDraw();
	void RotateStars( const Rotator &rotator );

	void BalloonUpdate();

	void TalkReaction( int textIndex );
	void ReactionUpdate();
	void UsedExposure( bool succeeded );
	void UsedOperate();
	void ResetExposureCount();

	void FadeBegin();
	void FadeCheck();
	void FadeDone();
	void FadeEnd();

	bool Exposure();	// 成功したらTRUE

	bool IsInputPauseButton();
	void PauseUpdate();

	void PrepareChangeSceneToTitle();

	void TakeScreenShot();

#if DEBUG_MODE

	void DebugTakeStageScreenShot();

#endif // DEBUG_MODE


	void Draw();
	void SelectDraw();
	void GameDraw();
	void ClearDraw();

	void TextDraw();

	void PauseDraw();

	void SelectDrawUI();
	void GameDrawUI();
	void ClearDrawUI();
};

#endif //INCLUDED_GAME_H_
