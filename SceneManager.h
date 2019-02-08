#ifndef INCLUDED_MANAGER_H_
#define INCLUDED_MANAGER_H_

//--------------------
//
//		Manager.h
//
//--------------------
class Scene;

//シーン管理クラス
class SceneMng
{
public:
	Scene *mpScene;	//現在のシーンを指すポインタ
public:
	SceneMng()
	{
		mpScene = nullptr;
	}
	~SceneMng()
	{
		delete mpScene;
	}

	void Init()
	{
		mpScene->Init();
	}
	void Update()
	{
		mpScene->Update();
	}
	void Draw()
	{
		mpScene->Draw();
	}
};

#endif //INCLUDED_MANAGER_H_