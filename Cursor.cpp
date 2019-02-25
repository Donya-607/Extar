#include "DxLib.h"
#include "Common.h"
#include "Music.h"
#include "Input.h"

#include "Cursor.h"

#include "FileIO.h"

namespace CursorImage
{
	static int hCursor;

	void Load()
	{
		// Ç∑Ç≈Ç…ílÇ™ì¸Ç¡ÇƒÇ¢ÇΩÇÁÅCì«Ç›çûÇÒÇæÇ‡ÇÃÇ∆Ç›Ç»ÇµÇƒîÚÇŒÇ∑
		if ( 0 != hCursor )
		{
			return;
		}
		// else

		hCursor = LoadGraph( "./Data/Images/Camera/CameraFrame.png" );
	}
	void Release()
	{
		DeleteGraph( hCursor );
		hCursor = 0;
	}

	int  GetHandle()
	{
		return hCursor;
	}
}
namespace SelectStage
{
	const Vector2 LEFT_TOP_POS{ 192.0f, 64.0f };
	const Vector2 SIZE{ 672.0f, 378.0f };
	const Vector2 MARGIN{ 64.0f, 32.0f };

	static const int maxRow = 4;

	Vector2 GetPosLeftTop()
	{
		return LEFT_TOP_POS;
	}
	Vector2 GetSize()
	{
		return SIZE;
	}
	Vector2 GetMargin()
	{
		return MARGIN;
	}

	int  GetMaxRow()
	{
		return maxRow;
	}
	int  CalcMaxColumn()
	{
		int div = FileIO::GetMaxStageNumber() / maxRow;
		int rem = FileIO::GetMaxStageNumber() % maxRow;

		return div + rem;
	}

	void Draw()
	{

	}
}

void Cursor::Init()
{
	pos = SelectStage::GetPosLeftTop();
}
void Cursor::Uninit()
{

}

void Cursor::Update()
{
	Move();
	if ( !isDoneMove )
	{
		Interpolate();
	}
}

void Cursor::Move()
{
	constexpr float RESPONSE_MOVE_AMOUNT = 16.0f;	// âüÇµÇΩèuä‘Ç…ÅCÇ©Ç»ÇÁÇ∏à⁄ìÆÇ≥ÇπÇÈó 

	bool isUp	= false, isDown		= false, isInput = false;

	if ( IS_TRG_UP		) { isUp	= true; }
	if ( IS_TRG_DOWN	) { isDown	= true; }
	if ( isUp	&& !isDown	) { pos.y -= RESPONSE_MOVE_AMOUNT; isInput = true; }
	if ( isDown	&& !isUp	) { pos.y += RESPONSE_MOVE_AMOUNT; isInput = true; }

	if ( isInput )
	{
		oldStageNumber = nowStageNumber;
		isDoneMove = false;

		int moveAmount = SelectStage::GetMaxRow();

		if ( isUp	&& moveAmount < nowStageNumber )
		{
			nowStageNumber -= moveAmount;
		}
		if ( isDown	&& nowStageNumber <= ( ( SelectStage::CalcMaxColumn() - 1 ) * SelectStage::GetMaxRow() ) )
		{
			nowStageNumber += moveAmount;
		}

		return;
	}
	// else

	isInput = false;
	bool isLeft = false, isRight = false;

	if ( IS_TRG_LEFT	) { isLeft	= true; }
	if ( IS_TRG_RIGHT	) { isRight	= true; }
	if ( isLeft		&& !isRight	) { pos.x -= RESPONSE_MOVE_AMOUNT; isInput = true; }
	if ( isRight	&& !isLeft	) { pos.x += RESPONSE_MOVE_AMOUNT; isInput = true; }

	if ( isInput )
	{
		oldStageNumber = nowStageNumber;
		isDoneMove = false;

		int moveAmount = 1;

		if ( isLeft		&& moveAmount < nowStageNumber )
		{
			nowStageNumber -= moveAmount;
		}
		if ( isRight	&& nowStageNumber < FileIO::GetMaxStageNumber() )
		{
			nowStageNumber += moveAmount;
		}
	}
}

bool Cursor::Interpolate()
{
	int row  = nowStageNumber % SelectStage::GetMaxRow();
	int clum = nowStageNumber / SelectStage::GetMaxRow();;
	Vector2 targetPos;

	return isDoneMove;
}

void Cursor::Draw( Vector2 shake ) const
{

}