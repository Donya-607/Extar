#ifndef INCLUDED_GAME_H_
#define INCLUDED_GAME_H_

#include <memory>

#include "Scene.h"
#include "SceneManager.h"

#include "Camera.h"

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
		Game = 0,
		FOO
	};
public:
	int stageNumber;	// 1Žn‚Ü‚è

	State state;

	std::unique_ptr<Camera> pCamera;

	bool isPause;
	bool isDrawCollision;
public:
	Game( SceneMng *pMng ) : Scene( pMng ),
		stageNumber( 1 ),
		state( State::Game ),
		pCamera( nullptr ),
		isPause( false ), isDrawCollision( false )
		{}
	~Game() {}

	void Init();
	void Uninit();

	void Update();
	void GameUpdate();

	bool IsInputPauseButton();
	void PauseUpdate();

	void PrepareChangeSceneToTitle();

	void Draw();
	
	void CollisionCheck();
	void ShowCollisionArea();
};

#endif //INCLUDED_GAME_H_
