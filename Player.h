#ifndef INCLUDED_PLAYER_H_
#define INCLUDED_PLAYER_H_

#include "Animation.h"
#include "Collision.h"
#include "Vector2.h"

//--------------------
//
//		Player.h
//
//--------------------

class Player
{
public:
	enum class Anime : int
	{
		Idle = 0,
		OpeningIdle,
		Move,
		OpeningMove,
	};
private:
	Vector2	pos;		// ���S�_
	Vector2 velocity;

	Anim	anim;

	Box		col;

	bool	isOpen;		// �J�������\���Ă��邩�ǂ���
	bool	isFlipX;	// �A�j���̍��E���]���g�����ǂ����C�摜�͉E����
public:
	Player() : pos(), velocity(),
		anim(), col(),
		isOpen( false ), isFlipX( false )
	{}
	~Player() {}

	void Init();
	void Uninit();

	void Update();
	void Open();	// �J���������܂���
	void Move();

	void SetAnime( Player::Anime animeType );

	Vector2 GetPos() const
	{
		return pos;
	}
	Box  GetColWorldPos() const;

	void Draw( Vector2 shake );
};

namespace PlayerImage
{
	constexpr int WIDTH  = 128;
	constexpr int HEIGHT = 128;

	void Load();
	void Release();

	int *GetImageHundle( Player::Anime animeType );
}

#endif //INCLUDED_PLAYER_H_
