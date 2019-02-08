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
	};
public:
	State	state;
public:
	Title( SceneMng *pMng ) : Scene( pMng ),
	state( State::Main )
	{}
	~Title() {}

	void Init();
	void Uninit();

	void Update();
	void MainUpdate();

	void PrepareChangeSceneToGame();

	void Draw();
};

#endif //INCLUDED_TITLE_H_
