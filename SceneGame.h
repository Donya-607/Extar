#ifndef INCLUDED_GAME_H_
#define INCLUDED_GAME_H_

#include <memory>

#include "Scene.h"
#include "SceneManager.h"

#include "Player.h"

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
	State state;

	std::unique_ptr<Player> pPlayer;

	bool isPause;
public:
	Game( SceneMng *pMng ) : Scene( pMng ),
		state( State::Game ),
		pPlayer(),
		isPause( false )
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
