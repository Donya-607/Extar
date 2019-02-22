#ifndef INCLUDED_STAR_H_
#define INCLUDED_STAR_H_

#include <vector>

#include "Common.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

#include "Animation.h"
#include "Collision.h"
#include "Vector2.h"

#include "Grid.h"

namespace StarImage
{
	constexpr int SIZE = 64;

	void Load();
	void Release();

	int  GetHandle( int level, int animIndex );
}

class Star
{
public:
	static constexpr int MAX_LEVEL = 6;
private:
	int row;	// �s, 0�n�܂�
	int column;	// ��, 0�n�܂�

	int width;	// �}�X�ڎ��̑傫��, 1�n�܂�
	int height;	// �}�X�ڎ��̑傫��, 1�n�܂�

	int level;	// 1 ~ 6

	Anim anim;
public:
	Star() : row( 0 ), column( 0 ), width( 1 ), height( 1 ),
		level( 6 ),
		anim()
	{}
	~Star() {}

	void Init( int row, int column, int width, int height, int level, bool flagSetAnimeOnly = false );
	void Uninit();

	void Update();

	void Draw( Vector2 shake ) const;
public:
	Box FetchColWorldPos() const
	{
		Vector2 halfSize;
		halfSize.x = ( ( width  * Grid::GetSize().x ) * 0.5f );
		halfSize.y = ( ( height * Grid::GetSize().y ) * 0.5f );

		Box tmp =
		{
			scast<float>( FRAME_POS_X + ( row		* Grid::GetSize().x ) ) + halfSize.x,
			scast<float>( FRAME_POS_Y + ( column	* Grid::GetSize().y ) ) + halfSize.y,
			halfSize.x - 1/* �Ӑ}���Ȃ��d�Ȃ��h������*/,
			halfSize.y - 1/* �Ӑ}���Ȃ��d�Ȃ��h������*/,
			true
		};

		return tmp;
	}

	void AcquireData( int *Row, int *Column, int *Width, int *Height, int *Level ) const
	{
		*Row		= row;
		*Column		= column;
		*Width		= width;
		*Height		= height;
		*Level		= level;
	}
	void SetData( int Row, int Column, int Width, int Height, int Level )
	{
		row		= Row;
		column	= Column;
		width	= Width;
		height	= Height;
		level	= Level;
	}
};

class StarMng
{
private:
	std::vector<Star> stars;

#if USE_IMGUI

	int stageNumber;	// 1�n�܂�
	int choiseRow;		// 0�n�܂�
	int choiseColumn;	// 0�n�܂�
	int width;			// 1�n�܂�
	int height;			// 1�n�܂�
	int level;			// 1�n�܂�

#endif // USE_IMGUI

public:
	StarMng() : stars() {}
	~StarMng() { std::vector<Star>().swap( stars ); }

	void Init( int stageNumber );
	void Uninit();

	void Update();

	void Draw( Vector2 shake ) const;
public:
	int GetArraySize() const
	{
		return scast<int>( stars.size() );
	}
	Box FetchColWorldPos( int index ) const
	{
		assert( index < scast<int>( stars.size() ) );

		Box tmp = stars.at( index ).FetchColWorldPos();
		return tmp;
	}
private:
#if USE_IMGUI

	void ChangeParametersByImGui();

	void SetStar();
	void RemoveStar();

	void DrawUI() const;

#endif // USE_IMGUI
};

#endif //INCLUDED_STAR_H_
