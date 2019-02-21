#include "DxLib.h"	// DrawLine(), GetColor()に使用
#include "Common.h"

#include "Grid.h"

namespace Grid
{
	Vector2 gridSize{ 0, 0 };	// Whole Size

	Vector2 GetSize()
	{
		return gridSize;
	}
	void SetSize( Vector2 wholeSize )
	{
		gridSize = wholeSize;
	}

	int GetRowMax()
	{
		return FRAME_WIDTH  / scast<int>( gridSize.x );
	}
	int GetColumnMax()
	{
		return FRAME_HEIGHT / scast<int>( gridSize.y );
	}

	void Draw( Vector2 shake )
	{
		int iShakeX = scast<int>( shake.x );
		int iShakeY = scast<int>( shake.y );

		// 縦線
		{
			int end = scast<int>( FRAME_WIDTH / gridSize.x );
			for ( int x = 0; x < end; x++ )
			{
				int pos = scast<int>( gridSize.x * x );
				// 画面内チェック
				if ( pos < 0 || FRAME_WIDTH < pos )
				{
					continue;
				}
				// else

				DrawLine
				(
					FRAME_POS_X + pos			- iShakeX,
					FRAME_POS_Y + 0				- iShakeY,
					FRAME_POS_X + pos			- iShakeX,
					FRAME_POS_Y + FRAME_HEIGHT	- iShakeY,
					GetColor( 255, 32, 80 ),
					3
				);
			}
		}
		// 横線
		{
			int end = scast<int>( FRAME_HEIGHT / gridSize.y );
			for ( int y = 0; y < end; y++ )
			{
				int pos = scast<int>( gridSize.y * y );
				// 画面内チェック
				if ( pos < 0 || FRAME_HEIGHT < pos )
				{
					continue;
				}
				// else

				DrawLine
				(
					FRAME_POS_X + 0				- iShakeX,
					FRAME_POS_Y + pos			- iShakeY,
					FRAME_POS_X + FRAME_WIDTH	- iShakeX,
					FRAME_POS_Y + pos			- iShakeY,
					GetColor( 255, 32, 80 ),
					3
				);
			}
		}
	}
}