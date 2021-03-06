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

//--------------------
//
//		Game.h
//
//--------------------

class RecordStar
{
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
	int  textExtendInterval;
	int  textNumber;	// 0始まり

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

	std::unique_ptr<Camera>   pCamera;
	std::unique_ptr<StarMng>  pStarMng;
	std::unique_ptr<NumMoves> pNumMoves;

	std::unique_ptr<Board>	  pBoard;
	std::vector<RecordStar>	  recordStars;

	bool isOpenFade;	// FadeBegin ~ FadeEnd までFALSE

	bool isOpenBalloon;	// テキストボックスの更新挙動 TRUE:開く, FALSE:閉じる

	bool isClearMoment;	// これがオンならスクショを取り，オフにしてクリアへ遷移させる
	bool isTakeScreenShot;
	bool isDoneMoveArm;

	bool isShowClearMenu;

	bool isPause;
	bool isDrawCollision;
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
		gotoNextPosX( SCREEN_WIDTH ),
		state( State::Select ), nextState( State::Null ),
		wink(),
		selectPos(),
		armPos( { scast<float>( SCREEN_WIDTH ), scast<float>( SCREEN_HEIGHT ) } ),
		pausePos(),
		pSSMng( nullptr ), pParticleMng( nullptr ),
		pCursor( nullptr ),
		pCamera( nullptr ), pStarMng( nullptr ), pNumMoves( nullptr ),
		pBoard( nullptr ), recordStars(),
		isOpenFade( false ),
		isOpenBalloon( true ),
		isClearMoment( false ), isTakeScreenShot( false ),
		isDoneMoveArm( false ),
		isShowClearMenu( false ),
		isPause( false ), isDrawCollision( false ),

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

	void BalloonUpdate();

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
