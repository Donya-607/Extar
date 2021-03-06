#ifndef INCLUDED_TITLE_H_
#define INCLUDED_TITLE_H_

#include <memory>

#include "Scene.h"
#include "SceneManager.h"

#include "Vector2.h"

#include "ShootingStar.h"

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
private:
	State	state;
	State	nextState;

	std::unique_ptr<ShootingStarMng> pSSMng;

	bool	isOpenFade;	// TRUEのときはフェードが晴れていて，動ける
private:	// 他ＰＧによる作業
	Vector2 star_pos;
	Vector2 E_pos;
	Vector2 Ku_pos;
	Vector2 Su_pos;
	Vector2 Ta_pos;

	int  Count;
	int Count2;
	int Count3;
	int Count4;

	int flash_rand;
	int flash_state;
	int flash_val;
	int val_speed;
	int timer;
	Vector2 flash_scale;
	Vector2 scale_up_speed;
	float angle;// Degree, ClockWise
	float angle_speed;

	int alpha_val;

	Vector2 push_pos;

	int shooting_star_1_val;
	int shooting_star_1_val_state;
	int shooting_star_2_val;
	int shooting_star_2_val_state;
public:
	Title( SceneMng *pMng ) : Scene( pMng ),
		state( State::Main ), nextState( State::Null ),
		pSSMng( nullptr ),
		isOpenFade( false ),

		star_pos(), E_pos(), Ku_pos(), Su_pos(), Ta_pos(), timer( 0 ),
		Count( 0 ), Count2( 0 ), Count3( 0 ), Count4( 0 ),
		flash_rand( 0 ), flash_state( 0 ), flash_val( 0 ),
		angle( 0 ), flash_scale(),
		scale_up_speed( { 0.3f, 0.3f } ), angle_speed( 100 ),
		val_speed( 50 ),alpha_val( 255 ),
		push_pos( { 330.0f, 650.0f } ),
		shooting_star_1_val( 50 ), shooting_star_2_val( 255 ),
		shooting_star_1_val_state( 0 ), shooting_star_2_val_state( 1 )
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
