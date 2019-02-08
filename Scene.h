#ifndef INCLUDED_SCENE_H_
#define INCLUDED_SCENE_H_

//--------------------
//
//		Scene.h
//
//--------------------
class SceneMng;

class Scene
{
public:
	Scene() : mpMng()		{}
	Scene( SceneMng *pMng ) : mpMng( pMng )	{}
	virtual ~Scene()		{};

	virtual void Init()		= 0;
	virtual void Update()	= 0;
	virtual void Draw()		= 0;
protected:
	SceneMng *mpMng;
};

#endif //INCLUDED_SCENE_H_