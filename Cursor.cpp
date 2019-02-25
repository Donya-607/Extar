#include <string>

#include "DxLib.h"
#include "Common.h"
#include "Music.h"
#include "Input.h"

#include "Cursor.h"

#include "FileIO.h"

namespace CursorImage
{
	const Vector2 SIZE{ 512.0f, 378.0f };

	static int hCursor;

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
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
	Vector2 GetSize()
	{
		return SIZE;
	}
}
namespace SelectImage
{
	static std::vector<int> hThumbnails;

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( scast<int>( hThumbnails.size() ) )
		{
			return;
		}
		// else

		hThumbnails.push_back( LoadGraph( "./Data/Images/Thumbnails/NoImage.png" ) );

		for ( int i = 1; true; i++ )
		{
			std::string filePath = "./Data/Images/Thumbnails/Stage" + std::to_string( i ) + ".png";
			int result = LoadGraph( filePath.c_str() );

			// HACK:エラーが起きたものは，DeleteGraphしなくて大丈夫なのかどうか？
			if ( result == -1 )
			{
				break;
			}
			// else

			hThumbnails.push_back( result );
		}
	}
	void Release()
	{
		for ( ; !hThumbnails.empty(); )
		{
			DeleteGraph( hThumbnails.back() );
			hThumbnails.pop_back();
		}
	}

	int  GetHandle( int stageNumber )
	{
		// stageNumberは１始まりの想定

		if ( stageNumber <= 0 )
		{
			assert( !"Error : SelectStage::GetHandle()" );
			return NULL;
		}
		// else
		if( scast<int>( hThumbnails.size() ) <= stageNumber )
		{
			return hThumbnails.at( 0 );
		}
		// else
		return hThumbnails.at( stageNumber );
	}
}
namespace SelectStage
{
	const Vector2 LEFT_TOP_POS{ 128.0f, 160.0f };
	const Vector2 SIZE{ 512.0f, 378.0f };
	const Vector2 MARGIN{ 64.0f, 64.0f };

	static const int maxRow  = 3;
	static const int maxClum = 2;

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
	int  GetMaxColumn()
	{
		return maxClum;
	}
	int  GetMaxDisplayNumber()
	{
		return GetMaxRow() * GetMaxColumn();
	}

	void Draw( int nowStageNumber )
	{
		int pageNum = ( ( nowStageNumber - 1 ) / SelectStage::GetMaxDisplayNumber() ); // 0始まり
		int end = ( pageNum * SelectStage::GetMaxDisplayNumber() ) + SelectStage::GetMaxDisplayNumber() + 1;
		for
			(
			int stageNumber = 1 + ( pageNum * SelectStage::GetMaxDisplayNumber() );
				stageNumber < end;
				stageNumber++
			)
		{
			int slidePos = ( ( stageNumber - 1 ) % SelectStage::GetMaxRow() ) * scast<int>( SelectStage::GetSize().x + SelectStage::GetMargin().x );
			int dropPos  = 0;
			if ( ( pageNum * SelectStage::GetMaxDisplayNumber() ) + GetMaxRow() < stageNumber )
			{
				dropPos = scast<int>( SelectStage::GetSize().y + SelectStage::GetMargin().y );
			}

			DrawExtendGraph
			(
				scast<int>( SelectStage::GetPosLeftTop().x ) + slidePos,
				scast<int>( SelectStage::GetPosLeftTop().y ) + dropPos,
				scast<int>( SelectStage::GetPosLeftTop().x + SelectStage::GetSize().x ) + slidePos,
				scast<int>( SelectStage::GetPosLeftTop().y + SelectStage::GetSize().y ) + dropPos,
				SelectImage::GetHandle( stageNumber ),
				TRUE
			);
		}
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
	constexpr float RESPONSE_MOVE_AMOUNT = 16.0f;	// 押した瞬間に，かならず移動させる量

	bool isUp	= false, isDown		= false, isInput = false;

	if ( IS_TRG_UP		) { isUp	= true; }
	if ( IS_TRG_DOWN	) { isDown	= true; }
	if ( isUp	&& !isDown	) { pos.y -= RESPONSE_MOVE_AMOUNT; isInput = true; }
	if ( isDown	&& !isUp	) { pos.y += RESPONSE_MOVE_AMOUNT; isInput = true; }

	if ( isInput )
	{
		isDoneMove = false;

		int moveAmount = SelectStage::GetMaxRow();

		// ２列のうち，カーソルの上下によって条件している（？），列が増えるとおじゃんになる

		if ( isUp	&& ( nowStageNumber % SelectStage::GetMaxColumn() ) >= SelectStage::GetMaxRow() )
		{
			nowStageNumber -= moveAmount;
		}
		if ( isDown	&& ( nowStageNumber % SelectStage::GetMaxColumn() ) <= SelectStage::GetMaxRow() )
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

void Cursor::Interpolate()
{
	constexpr float RESISTANCE		= 0.3f;
	constexpr float LOWER_DISTANCE	= 8.0f;

	int row  = ( ( nowStageNumber - 1 ) % SelectStage::GetMaxDisplayNumber() ) % SelectStage::GetMaxRow();
	int clum = ( ( nowStageNumber - 1 ) % SelectStage::GetMaxDisplayNumber() ) / SelectStage::GetMaxRow();
	Vector2 targetPos;
	targetPos.x = SelectStage::GetPosLeftTop().x + ( row  * ( SelectStage::GetSize().x + SelectStage::GetMargin().x ) );
	targetPos.y = SelectStage::GetPosLeftTop().y + ( clum * ( SelectStage::GetSize().y + SelectStage::GetMargin().y ) );

	Vector2 dir = targetPos - pos;
	float dist = fabsf( dir.Length() );

	if ( dist < LOWER_DISTANCE )
	{
		pos = targetPos;
		isDoneMove = true;

		return;
	}
	// else

	dir *= RESISTANCE;

	pos += dir;
}

void Cursor::Draw( Vector2 shake ) const
{
	DrawExtendGraph
	(
		scast<int>( pos.x ),
		scast<int>( pos.y ),
		scast<int>( pos.x + SelectStage::GetSize().x ),
		scast<int>( pos.y + SelectStage::GetSize().y ),
		CursorImage::GetHandle(),
		TRUE
	);
}