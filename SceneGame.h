#ifndef INCLUDED_GAME_H_
#define INCLUDED_GAME_H_

#include <memory>

#include "Scene.h"
#include "SceneManager.h"

#include "Camera.h"
#include "Star.h"
#include "NumMoves.h"

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
		Clear
	};
private:
	int  stageNumber;	// 1�n�܂�

	int  numMoves;		// ���̎萔�C0�n�܂�C�A���h�D�Ō���

	State state;

	std::unique_ptr<Camera>   pCamera;
	std::unique_ptr<StarMng>  pStarMng;
	std::unique_ptr<NumMoves> pNumMoves;

	bool isPause;
	bool isDrawCollision;
public:
	Game( SceneMng *pMng ) : Scene( pMng ),
		stageNumber( 1 ), numMoves( 0 ),
		state( State::Game ),
		pCamera( nullptr ), pStarMng( nullptr ), pNumMoves( nullptr ),
		isPause( false ), isDrawCollision( false )
		{}
	~Game() {}

	void Init();
	void Uninit();

	void Update();
	void SelectUpdate();
	void GameUpdate();
	void ClearUpdate();

	bool Exposure();	// ����������TRUE

	bool IsInputPauseButton();
	void PauseUpdate();

	void PrepareChangeSceneToTitle();

	void Draw();
	void DrawUI();
private:
	void CollisionCheck();
	void ShowCollisionArea();
};

#endif //INCLUDED_GAME_H_
