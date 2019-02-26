#ifndef INCLUDED_GAME_H_
#define INCLUDED_GAME_H_

#include <memory>

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
	int  stageNumber;	// 1�n�܂�
	int  numMoves;		// ���̎萔�C0�n�܂�C�A���h�D�Ō���

	int	 choice;		// 0�n�܂�, �|�[�Y�ƃ��U���g�����˂Ă���

	int  hScreenShot;

	int	 hFont;			// JF�h�b�gK12

	State state;
	State nextState;

	std::unique_ptr<Cursor>   pCursor;

	std::unique_ptr<Camera>   pCamera;
	std::unique_ptr<StarMng>  pStarMng;
	std::unique_ptr<NumMoves> pNumMoves;

	std::unique_ptr<Board>	  pBoard;

	bool isOpenFade;	// FadeBegin ~ FadeEnd �܂�TRUE

	bool isClearMoment;		// ���ꂪ�I���Ȃ�X�N�V�������C�I�t�ɂ��ăN���A�֑J�ڂ�����
	bool isTakeScreenShot;

	bool isShowClearMenu;

	bool isPause;
	bool isDrawCollision;
public:
	Game( SceneMng *pMng ) : Scene( pMng ),
		stageNumber( 1 ), numMoves( 0 ),
		choice( 0 ),
		hScreenShot( 0 ), hFont( 0 ),
		state( State::Select ), nextState( State::Null ),
		pCursor( nullptr ),
		pCamera( nullptr ), pStarMng( nullptr ), pNumMoves( nullptr ),
		pBoard( nullptr ),
		isOpenFade( false ),
		isClearMoment( false ), isTakeScreenShot( false ),
		isShowClearMenu( false ),
		isPause( false ), isDrawCollision( false )
		{}
	~Game() {}

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

	bool Exposure();	// ����������TRUE

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
