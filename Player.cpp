#include <array>

#include "DxLib.h"
#include "Music.h"
#include "Common.h"
#include "Input.h"

#include "ScreenShake.h"

#include "Player.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif

namespace PlayerImage
{
	constexpr int NUM_ROW  = 3;	// 横
	constexpr int NUM_CLUM = 4;	// 縦
	constexpr int NUM_ALL  = NUM_ROW * NUM_CLUM;

	constexpr int SPD_IDLE = 4;
	constexpr int SPD_OPENING_IDLE = 4;
	constexpr int SPD_MOVE = 4;
	constexpr int SPD_OPENING_MOVE = 4;

	static int hPlayer[NUM_ALL];

	static int hIdle[NUM_ROW];
	static int hOpeningIdle[NUM_ROW];
	static int hMove[NUM_ROW];
	static int hOpeningMove[NUM_ROW];

	void Load()
	{
		if ( 0 != hPlayer[0] )
		{
			return;
		}
		// else

		LoadDivGraph
		(
			"./Data/Images/Player/Player.png",
			NUM_ALL,
			NUM_ROW, NUM_CLUM,
			WIDTH,   HEIGHT,
			hPlayer
		);

		// 各動作別の配列に格納していく
		{
			int index = 0;
			int end = NUM_ROW;
			// 待機
			for ( int i = 0; i < end; i++ )
			{
				hIdle[i] = hPlayer[index++];
			}
			end = NUM_ROW;
			// 構え待機
			for ( int i = 0; i < end; i++ )
			{
				hOpeningIdle[i] = hPlayer[index++];
			}
			end = NUM_ROW;
			// 移動
			for ( int i = 0; i < end; i++ )
			{
				hMove[i] = hPlayer[index++];
			}
			end = NUM_ROW;
			// 構え移動
			for ( int i = 0; i < end; i++ )
			{
				hOpeningMove[i] = hPlayer[index++];
			}
		}
	}
	void Release()
	{
		for ( int i = 0; i < NUM_ALL; i++ )
		{
			DeleteGraph( hPlayer[i] );
			hPlayer[i] = 0;
		}
		for ( int i = 0; i < NUM_ROW; i++ )
		{
			DeleteGraph( hIdle[i]			);
			DeleteGraph( hOpeningIdle[i]	);
			DeleteGraph( hMove[i]			);
			DeleteGraph( hOpeningMove[i]	);
			hIdle[i]		= 0;
			hOpeningIdle[i]	= 0;
			hMove[i]		= 0;
			hOpeningMove[i]	= 0;
		}
	}

	int *GetImageHundle( Player::Anime animeType )
	{
		const std::array<int *, NUM_CLUM> animes =
		{
			hIdle, hOpeningIdle, hMove, hOpeningMove
		};

		return animes[scast<int>( animeType )];
	}
}

void Player::Init()
{
	PlayerImage::Load();

	const Vector2 INIT_POS{ 960.0f, 960.0f };
	pos = INIT_POS;

	SetAnime( Anime::Idle );

	col.Set
	(
		0,			// Xオフセット
		0,			// Yオフセット
		32.0f,		// サイズの半分
		56.0f,		// サイズの半分
		true		// 当たり判定適用フラグ
	);
}
void Player::Uninit()
{
	PlayerImage::Release();
}

void Player::Update()
{
#if USE_IMGUI

	ImGui::Begin( "Player_Collision" );

	ImGui::SliderFloat( "X_Offset", &col.cx, scast<float>( -PlayerImage::WIDTH  ), scast<float>( PlayerImage::WIDTH  ) );
	ImGui::SliderFloat( "Y_Offset", &col.cy, scast<float>( -PlayerImage::HEIGHT ), scast<float>( PlayerImage::HEIGHT ) );

	ImGui::SliderFloat( "Width",  &col.w, 1.0f, scast<float>( PlayerImage::WIDTH  ) );
	ImGui::SliderFloat( "Height", &col.h, 1.0f, scast<float>( PlayerImage::HEIGHT ) );

	ImGui::End();

#endif

	Open();

	Move();

	anim.Update();
}
void Player::Open()
{
	if ( !IS_PRESS_J_X_OPEN )
	{
		if ( isOpen )
		{
			PlaySE( M_OPEN );
			isOpen = false;
		}
		return;
	}
	// else

	if ( !isOpen )
	{
		PlaySE( M_OPEN );
		isOpen = true;
	}
}
void Player::Move()
{
#if USE_IMGUI

	static float MOVE_SPD = 12.0f;

	ImGui::Begin( "Player_Move" );

	ImGui::SliderFloat( "SideMove_Speed", &MOVE_SPD, 0.0f, 64.0f );
	ImGui::SliderFloat( "Y_Position", &pos.y, 0.0f, scast<float>( SCREEN_HEIGHT ) );

	ImGui::End();
#else

	constexpr float MOVE_SPD = 12.0f;

#endif // USE_IMGUI

	bool isLeft = false;
	bool isRight = false;

	// 入力確認
	{
		if ( IS_PRESS_LEFT )
		{
			isLeft = true;
		}
		if ( IS_PRESS_RIGHT )
		{
			isRight = true;
		}
	}
	// 入力適用
	{
		if ( isLeft && !isRight )
		{
			velocity.x = -MOVE_SPD;
		}
		else if ( isRight && !isLeft )
		{
			velocity.x = MOVE_SPD;
		}
		else
		{
			velocity.x = 0;
		}
	}

	pos += velocity;

	// 画面外チェック
	{
		float plLeft  = ( pos.x + col.cx ) - col.w;
		float plRight = ( pos.x + col.cx ) + col.w;

		if ( plLeft < 0 )
		{
			pos.x = ( 0 + col.cx ) + col.w;
		}
		else if ( SCREEN_WIDTH < plRight )
		{
			pos.x = ( SCREEN_WIDTH - col.cx ) - col.w;
		}
	}

	// アニメ変更
	{
		if	(
			( isLeft  && !isRight ) ||
			( isRight && !isLeft  )
			)
		{
			if ( !isOpen && anim.hnd != PlayerImage::GetImageHundle( Anime::Move ) )
			{
				SetAnime( Anime::Move );
			}
			else if ( isOpen && anim.hnd != PlayerImage::GetImageHundle( Anime::OpeningMove ) )
			{
				SetAnime( Anime::OpeningMove );
			}
			
			isFlipX = ( isLeft ) ? true : false;
		}
		else
		{
			if ( !isOpen && anim.hnd != PlayerImage::GetImageHundle( Anime::Idle ) )
			{
				SetAnime( Anime::Idle );
			}
			if ( isOpen && anim.hnd != PlayerImage::GetImageHundle( Anime::OpeningIdle ) )
			{
				SetAnime( Anime::OpeningIdle );
			}
		}
	}
}

void Player::SetAnime( Player::Anime animeType )
{
	switch ( animeType )
	{
	case Player::Anime::Idle:
		{
			anim.Set
			(
				0,						// アニメ再生範囲・先頭
				PlayerImage::NUM_ROW,	// アニメ再生範囲・末尾
				anim.index,				// アニメ開始番号
				PlayerImage::SPD_IDLE,	// 再生速度
				PlayerImage::GetImageHundle( animeType ),	// 画像ハンドル
				true,					// ループ再生させるかどうか
				false					// 逆再生させるかどうか
			);
		}
		break;
	case Player::Anime::OpeningIdle:
		{
			anim.Set
			(
				0,						// アニメ再生範囲・先頭
				PlayerImage::NUM_ROW,	// アニメ再生範囲・末尾
				anim.index,				// アニメ開始番号
				PlayerImage::SPD_OPENING_IDLE,				// 再生速度
				PlayerImage::GetImageHundle( animeType ),	// 画像ハンドル
				true,					// ループ再生させるかどうか
				false					// 逆再生させるかどうか
			);
		}
		break;
	case Player::Anime::Move:
		{
			anim.Set
			(
				0,						// アニメ再生範囲・先頭
				PlayerImage::NUM_ROW,	// アニメ再生範囲・末尾
				anim.index,				// アニメ開始番号
				PlayerImage::SPD_MOVE,	// 再生速度
				PlayerImage::GetImageHundle( animeType ),	// 画像ハンドル
				true,					// ループ再生させるかどうか
				false					// 逆再生させるかどうか
			);
		}
		break;
	case Player::Anime::OpeningMove:
		{
			anim.Set
			(
				0,						// アニメ再生範囲・先頭
				PlayerImage::NUM_ROW,	// アニメ再生範囲・末尾
				anim.index,				// アニメ開始番号
				PlayerImage::SPD_OPENING_MOVE,				// 再生速度
				PlayerImage::GetImageHundle( animeType ),	// 画像ハンドル
				true,					// ループ再生させるかどうか
				false					// 逆再生させるかどうか
			);
		}
		break;
	default:
		assert( !"Argument Error : Player::SetAnime()" );
		break;
	}
}

Box  Player::GetColWorldPos() const
{
	Box tmp = col;
	tmp.cx += pos.x;
	tmp.cy += pos.y;

	return tmp;
}

void Player::Draw( Vector2 shake )
{
	DrawRotaGraph
	(
		scast<int>( pos.x - shake.x ),
		scast<int>( pos.y - shake.y ),
		1.0, ToRadian( 0 ),
		anim.GetNowHandle(),
		TRUE,
		isFlipX
	);
}