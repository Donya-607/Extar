#ifndef INCLUDED_STAR_H_
#define INCLUDED_STAR_H_

#include <vector>

#include "Animation.h"
#include "Vector2.h"

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
	int row;	// 行, 0始まり
	int column;	// 列, 0始まり

	int width;	// マス目式の大きさ, 1始まり
	int height;	// マス目式の大きさ, 1始まり

	int level;	// 1 ~ 6

	Anim anim;
public:
	Star() : row( 0 ), column( 0 ), width( 1 ), height( 1 ),
		level( 6 ),
		anim()
	{}
	~Star() {}

	void Init( int row, int column, int width, int height, int level );
	void Uninit();

	void Update();

	void Draw( Vector2 shake ) const;
};

class StarMng
{
private:
	std::vector<Star> stars;
public:
	StarMng() : stars() {}
	~StarMng() { std::vector<Star>().swap( stars ); }

	void Init( int stageNumber );
	void Uninit();

	void Update();

	void Draw( Vector2 shake ) const;
};

#endif //INCLUDED_STAR_H_
