#ifndef INCLUDED_MANAGER_H_
#define INCLUDED_MANAGER_H_

//--------------------
//
//		Manager.h
//
//--------------------
class Scene;

//�V�[���Ǘ��N���X
class SceneMng
{
public:
	Scene *mpScene;	//���݂̃V�[�����w���|�C���^
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