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
	constexpr int SIZE = 96;// 64;

	void Load();
	void Release();

	int  GetHandle( int level, int animIndex );
}

class Star
{
public:
	static constexpr int MAX_LEVEL = 6;
private:
	int row;	// 行, 0始まり
	int column;	// 列, 0始まり

	int width;	// マス目式の大きさ, 1始まり
	int height;	// マス目式の大きさ, 1始まり

	int level;	// 1 ~ 6

	float angle;// Degree, ClockWise

	Anim anim;
public:
	Star() : row( 0 ), column( 0 ), width( 1 ), height( 1 ),
		level( 6 ),
		angle( 0 ),
		anim()
	{}
	~Star() {}

	void Init( int row, int column, int width, int height, int level, bool flagSetAnimeOnly = false );
	void Uninit();

	void Update();

	void CalcRotate();

	void Draw( Vector2 shake ) const;
public:
	Box  FetchColWorldPos() const
	{
		Vector2 halfSize;
		halfSize.x = ( ( width  * Grid::GetSize().x ) * 0.5f );
		halfSize.y = ( ( height * Grid::GetSize().y ) * 0.5f );

		Box tmp =
		{
			scast<float>( FRAME_POS_X + ( row		* Grid::GetSize().x ) ) + halfSize.x,
			scast<float>( FRAME_POS_Y + ( column	* Grid::GetSize().y ) ) + halfSize.y,
			halfSize.x - 1/* 意図しない重なりを防ぐため*/,
			halfSize.y - 1/* 意図しない重なりを防ぐため*/,
			true
		};

		return tmp;
	}
	int  GetLevel() const
	{
		return level;
	}

	void BeExposed();

	void AcquireData( int *Row, int *Column, int *Width, int *Height, int *Level ) const
	{
		if ( nullptr != Row		)	{ *Row		= row;		}
		if ( nullptr != Column	)	{ *Column	= column;	}
		if ( nullptr != Width	)	{ *Width	= width;	}
		if ( nullptr != Height	)	{ *Height	= height;	}
		if ( nullptr != Level	)	{ *Level	= level;	}
	}
	void SetData( int Row, int Column, int Width, int Height, int Level )
	{
		if ( NULL != Row	) { row		= Row;		}
		if ( NULL != Column	) { column	= Column;	}
		if ( NULL != Width	) { width	= Width;	}
		if ( NULL != Height	) { height	= Height;	}
		if ( NULL != Level	) { level	= Level;	}
	}
};

class StarMng
{
private:
	std::vector<Star> stars;

	std::vector<std::vector<int>> levelStorage;	// Ctrl+Zにて使用

#if USE_IMGUI

	int stageNumber;	// 1始まり
	int choiseRow;		// 0始まり
	int choiseColumn;	// 0始まり
	int width;			// 1始まり
	int height;			// 1始まり
	int level;			// 1始まり

#endif // USE_IMGUI

public:
	StarMng() : stars(), levelStorage(){}
	~StarMng()
	{
		std::vector<Star>().swap( stars );
		std::vector<std::vector<int>>().swap( levelStorage );
	}

	void Init( int stageNumber );
	void Uninit();

	void Update();
	void ClearUpdate();

	void Draw( Vector2 shake ) const;
public:
	int  GetArraySize() const
	{
		return scast<int>( stars.size() );
	}
	Box  FetchColWorldPos( int index ) const
	{
		assert( index < scast<int>( stars.size() ) );

		return stars.at( index ).FetchColWorldPos();
	}
	int  FetchLevel( int index ) const
	{
		assert( index < scast<int>( stars.size() ) );

		return stars.at( index ).GetLevel();
	}

	void SaveLog();
	bool Undo();	// 成功したらTRUE

	void Expose( int index )
	{
		assert( index < scast<int>( stars.size() ) );

		stars.at( index ).BeExposed();
	}

	bool IsEqualLevels() const;
private:
	bool CanSaveLog() const
	{
		// セーブできない状況があると思っていたが，そうでもなさそう？
		if ( scast<int>( stars.size() ) == scast<int>( levelStorage.size() ) )
		{
			// return false;
		}
		// else
		return true;
	}
	bool CanUndo() const
	{
		if ( scast<int>( levelStorage.size() ) )
		{
			return true;
		}
		// else
		return false;
	}
#if USE_IMGUI

	void ChangeParametersByImGui();

	void SetStar();
	void RemoveStar();

	void DrawUI() const;

#endif // USE_IMGUI
};

#endif //INCLUDED_STAR_H_
