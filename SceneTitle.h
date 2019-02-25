#ifndef INCLUDED_TITLE_H_
#define INCLUDED_TITLE_H_

#include <memory>

#include "Scene.h"
#include "SceneManager.h"

#include "Vector2.h"

//--------------------
//
//		Title.h
//
//--------------------

class Title : public Scene
{
private:
	enum class State : int
	{
		Main = 0,
		HowToPlay,
		GotoGame,
		Null
	};
public:
	State	state;
	State	nextState;
public:
	Title( SceneMng *pMng ) : Scene( pMng ),
	state( State::Main ), nextState( State::Null )
	{}
	~Title() {}

	void Init();
	void MainInit();

	void Uninit();
	void MainUninit();

	void Update();
	void MainUpdate();

	void FadeBegin();
	void FadeCheck();
	void FadeDone();
	void FadeEnd();

	void PrepareChangeSceneToGame();

	void Draw();
};

#endif //INCLUDED_TITLE_H_
