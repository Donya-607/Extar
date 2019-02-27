#ifndef INCLUDED_GAME_H_
#define INCLUDED_GAME_H_

#include <memory>
#include <vector>

#include "Scene.h"
#include "SceneManager.h"

#include "Camera.h"
#include "Star.h"
#include "NumMoves.h"
#include "Cursor.h"

#include "Board.h"

//--------------------
//
//		Game.h
//
//--------------------

class RecordStar
{
private:
	Vector2 pos;		// Center

	float angle;		// Degree
	float scale;
	float rotateSpd;
	float magniSpd;		// 拡大縮小

	bool isGlow;
public:
	RecordStar() : pos(),
		angle( 0 ), scale( 0 ),
		rotateSpd( 0 ), magniSpd( 0 ),
		isGlow( false )
	{}
	~RecordStar() {}

	void Init( Vector2 centerPos, bool isGlowStar );

	void Update();

	void Draw(  Vector2 shake ) const;
};

class Game : public Scene
{
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

	int	 choice;		// 0始まり, ポーズとリザルトを兼ねている

	int  clearTimer;
	int  hScreenShot;

	int	 hFont;			// JFドットK12

	int  gotoNextPosX;	// LeftTop, リザルト画面で使用

	State state;
	State nextState;

	std::unique_ptr<Cursor>   pCursor;

	std::unique_ptr<Camera>   pCamera;
	std::unique_ptr<StarMng>  pStarMng;
	std::unique_ptr<NumMoves> pNumMoves;

	std::unique_ptr<Board>	  pBoard;
	std::vector<RecordStar>	  recordStars;

	bool isOpenFade;	// FadeBegin ~ FadeEnd までTRUE

	bool isClearMoment;	// これがオンならスクショを取り，オフにしてクリアへ遷移させる
	bool isTakeScreenShot;

	bool isShowClearMenu;

	bool isPause;
	bool isDrawCollision;
private:	// 他ＰＧによる作業
	Vector2 str_up_pos;
	Vector2 str_down_pos;
	Vector2 str_speed;
	bool shutter_flag;
	int shutter_state;
public:
	Game( SceneMng *pMng ) : Scene( pMng ),
		stageNumber( 1 ), numMoves( 0 ),
		choice( 0 ),
		clearTimer( 0 ),
		hScreenShot( 0 ), hFont( 0 ),
		gotoNextPosX( SCREEN_WIDTH ),
		state( State::Select ), nextState( State::Null ),
		pCursor( nullptr ),
		pCamera( nullptr ), pStarMng( nullptr ), pNumMoves( nullptr ),
		pBoard( nullptr ), recordStars(),
		isOpenFade( false ),
		isClearMoment( false ), isTakeScreenShot( false ),
		isShowClearMenu( false ),
		isPause( false ), isDrawCollision( false ),

		str_up_pos( { 192.0f,64.0f } ),
		str_down_pos( { 192.0f,64.0f } ),
		str_speed( { 0.0f,70.0f } ),
		shutter_flag( false ),
		shutter_state( 0 )

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

	void FadeBegin();
	void FadeCheck();
	void FadeDone();
	void FadeEnd();

	bool Exposure();	// 成功したらTRUE

	bool IsInputPauseButton();
	void PauseUpdate();

	void PrepareChangeSceneToTitle();

	void TakeScreenShot();

	void Draw();
	void SelectDraw();
	void GameDraw();
	void ClearDraw();

	void PauseDraw();

	void SelectDrawUI();
	void GameDrawUI();
	void ClearDrawUI();
private:
	void CollisionCheck();
	void ShowCollisionArea();
};

#endif //INCLUDED_GAME_H_
