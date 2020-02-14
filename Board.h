#ifndef INCLUDED_BOARD_H_
#define INCLUDED_BOARD_H_

#include <memory>

#include "Vector2.h"

namespace BoardImage
{
	constexpr int MARGIN = 32;	// スクショの上下左右にできる隙間
	constexpr int SIZE_X = 768 + MARGIN + MARGIN;
	constexpr int SIZE_Y = 384 + MARGIN + MARGIN;

	void Load();
	void Release();

	int  GetBoardHandle();
}

class Picture
{
private:
	Vector2 basePos;	// Center
	Vector2 pos;		// Center

	float angle;		// Degree
public:
	Picture() : basePos(), pos(), angle( 0 ) {}
	~Picture() {}

	void Init();
	void Uninit();

	void Update();

	void Draw( int handle, Vector2 shake ) const;
public:
	void SetPos( Vector2 centerPos )
	{
		basePos = centerPos;
	}
};

class Board
{
private:
	float angle;	// Degree

	Vector2 pos;	// Center
	Vector2 velo;	// velocity

	std::unique_ptr<Picture> pPic;
public:
	Board() : pos(), velo(), angle( 0 ), pPic() {}
	~Board() {}

	void Init( Vector2 centerPos );
	void Uninit();

	void Update();

	void Draw( int hPicture, Vector2 shake ) const;
public:
	void SkipPerformance();
private:
	void Move();

	void StopInDestination();
};

#endif //INCLUDED_BOARD_H_
