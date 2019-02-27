#include <string>

#include "DxLib.h"
#include "Common.h"
#include "Music.h"
#include "Input.h"

#include "Cursor.h"

#include "FileIO.h"

#include "Star.h"
#include "Number.h"

namespace CursorImage
{
	const Vector2 SIZE{ 512.0f, 378.0f };

	static int hCursor;
	static int hNotChoice;

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( 0 != hCursor )
		{
			return;
		}
		// else
		
		hCursor = LoadGraph( "./Data/Images/Camera/CameraFrame.png" );
		hNotChoice = LoadGraph( "./Data/Images/Camera/NotChoice.png" );
	}
	void Release()
	{
		DeleteGraph( hCursor );
		DeleteGraph( hNotChoice );
		hCursor = 0;
		hNotChoice = 0;
	}

	int  GetHandle( bool isChoice )
	{
		return ( isChoice ) ? hCursor : hNotChoice;
	}
	Vector2 GetSize()
	{
		return SIZE;
	}
}
namespace StageImage
{
	constexpr int NUM_BACK		= 2;
	constexpr int SIZE_BACK_X	= 96;
	constexpr int SIZE_BACK_Y	= 96;

	constexpr int NUM_LR_ROW	= 2;
	constexpr int SIZE_LR_X		= 96;
	constexpr int SIZE_LR_Y		= 128;

	static std::vector<int> hThumbnails;
	static int hBack[2];
	static int hLR[NUM_LR_ROW * 2];

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

		LoadDivGraph
		(
			"./Data/Images/UI/Back.png",
			NUM_BACK,
			NUM_BACK, 1,
			SIZE_BACK_X, SIZE_BACK_Y,
			hBack
		);

		LoadDivGraph
		(
			"./Data/Images/UI/UseLR.png",
			NUM_LR_ROW * 2,
			NUM_LR_ROW, 2,
			SIZE_LR_X, SIZE_LR_Y,
			hLR
		);
	}
	void Release()
	{
		for ( ; !hThumbnails.empty(); )
		{
			DeleteGraph( hThumbnails.back() );
			hThumbnails.pop_back();
		}

		for ( int i = 0; i < NUM_BACK; i++ )
		{
			DeleteGraph( hBack[i] );
			hBack[i] = 0;
		}

		for ( int i = 0; i < ( NUM_LR_ROW * 2 ); i++ )
		{
			DeleteGraph( hLR[i] );
			hLR[i] = 0;
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
	int  GetBackHandle( bool isGlow )
	{
		return hBack[( isGlow ) ? 1 : 0];
	}

	int GetLRHandle( bool isL, bool isGlow )
	{
		int index = ( isL ) ? 0 : 2;
		if ( isGlow )
		{
			index += 1;
		}

		return hLR[index];
	}
}
namespace StageSelect
{
	const Vector2 LEFT_TOP_POS{ 128.0f, 144.0f };
	const Vector2 SIZE{ 512.0f, 378.0f };
	const Vector2 MARGIN{ 64.0f, 32.0f };

	const Vector2 BACK_CENTER_POS{ 200.0f, 72.0f };
	const Vector2 BACK_SIZE{ 96.0f, 96.0f };

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

	Vector2 GetBackPosCenter()
	{
		return BACK_CENTER_POS;
	}
	Vector2 GetBackSize( bool isGlow )
	{
		float magni = ( isGlow ) ? 1.5f : 1.0f;
		return BACK_SIZE * magni;
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

	static bool isGlowBack = false;
	void SetBackGlow( bool isGlow )
	{
		isGlowBack = isGlow;
	}

	static bool isGlowLR[2] = { false, false };
	void SetLRGlow( bool isL, bool isGlow )
	{
		isGlowLR[( isL ) ? 0 : 1] = isGlow;
	}

	void Draw( int nowStageNumber )
	{
		int pageNum = ( ( nowStageNumber - 1 ) / StageSelect::GetMaxDisplayNumber() ); // 0始まり
		int end = ( pageNum * StageSelect::GetMaxDisplayNumber() ) + StageSelect::GetMaxDisplayNumber() + 1;
		for (
			int stageNumber = 1 + ( pageNum * StageSelect::GetMaxDisplayNumber() );
				stageNumber < end;
				stageNumber++
			)
		{
			int slidePos = ( ( stageNumber - 1 ) % StageSelect::GetMaxRow() ) * scast<int>( StageSelect::GetSize().x + StageSelect::GetMargin().x );
			int dropPos  = 0;
			if ( ( pageNum * StageSelect::GetMaxDisplayNumber() ) + GetMaxRow() < stageNumber )
			{
				dropPos = scast<int>( StageSelect::GetSize().y + StageSelect::GetMargin().y );
			}

			int posX = scast<int>( StageSelect::GetPosLeftTop().x ) + slidePos;
			int posY = scast<int>( StageSelect::GetPosLeftTop().y ) + dropPos;

			// Thumbnail
			DrawExtendGraph
			(
				posX,
				posY,
				posX + scast<int>( StageSelect::GetSize().x ),
				posY + scast<int>( StageSelect::GetSize().y ),
				StageImage::GetHandle( stageNumber ),
				TRUE
			);
			// Cursor
			DrawExtendGraph
			(
				posX,
				posY,
				posX + scast<int>( StageSelect::GetSize().x ),
				posY + scast<int>( StageSelect::GetSize().y ),
				CursorImage::GetHandle( false ),
				TRUE
			);
			// StageNumber
			{
				const int HALF_SIZE_X = scast<int>( StageSelect::GetSize().x * 0.5f );
				const int HALF_SIZE_Y = scast<int>( StageSelect::GetSize().y * 0.5f );
				const int MAGNI_X = 2;
				const int MAGNI_Y = 2;

				int num = stageNumber;
				// HACK:ステージ数は２ケタにおさまる想定である
				for ( int digit = 0; digit < 2; digit++ )
				{
					bool isGlow = ( nowStageNumber == stageNumber ) ? true : false;

					if ( stageNumber < 10 )
					{
						DrawExtendGraph
						(
							posX + HALF_SIZE_X - Number::SIZE_X,
							posY + ( HALF_SIZE_Y >> 1 ),
							posX + HALF_SIZE_X - Number::SIZE_X + ( Number::SIZE_X * MAGNI_X ),
							posY + ( HALF_SIZE_Y >> 1 ) + ( Number::SIZE_Y * MAGNI_Y ),
							Number::GetHandle( num, isGlow ),
							TRUE
						);

						break;
					}
					// else
					
					DrawExtendGraph
					(
						posX + HALF_SIZE_X - ( Number::SIZE_X >> 1 ) - ( Number::SIZE_X * digit ),
						posY + ( HALF_SIZE_Y >> 1 ),
						posX + HALF_SIZE_X - ( Number::SIZE_X >> 1 ) - ( Number::SIZE_X * digit ) + ( Number::SIZE_X * MAGNI_X ),
						posY + ( HALF_SIZE_Y >> 1 ) + ( Number::SIZE_Y * MAGNI_Y ),
						Number::GetHandle( num % 10, isGlow ),
						TRUE
					);

					num /= 10;
				}
			}
		}

		// Back
		{
			int tweakX = 0;
			int tweakY = 0;

			int baseX  = scast<int>( StageSelect::GetBackPosCenter().x ) + tweakX;
			int baseY  = scast<int>( StageSelect::GetBackPosCenter().y ) + tweakY;

			double scale = ( isGlowBack ) ? 1.5 : 1.0;
			DrawRotaGraph
			(
				baseX,
				baseY,
				scale,
				ToRadian( 0 ),
				StageImage::GetBackHandle( isGlowBack ),
				TRUE
			);
		}

		// ページ数アイコン表示
		{
			const int CHOICE_STAR_LEVEL = 1;
			const int NOT_CHOICE_STAR_LEVEL = 5;

			int maxPageNum = ( ( FileIO::GetMaxStageNumber() - 1 ) / StageSelect::GetMaxDisplayNumber() ) + 1;

			Vector2 base{ scast<float>( SCREEN_WIDTH >> 1 ), 960.0f };
			base.x -= StarImage::SIZE * ( maxPageNum >> 1 );

			for ( int i = 0; i < maxPageNum; i++ )
			{
				bool isChoicePage = false;
				if ( i == pageNum )
				{
					isChoicePage = true;
				}

				int level =
					( isChoicePage )
					? CHOICE_STAR_LEVEL
					: NOT_CHOICE_STAR_LEVEL;

				SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
				DrawGraph
				(
					scast<int>( base.x + ( i * StarImage::SIZE ) ),
					scast<int>( base.y ),
					StarImage::GetHandle( level, 0 ),
					TRUE
				);
				SetDrawBlendMode( DX_BLENDMODE_ADD, 255 );
				DrawGraph
				(
					scast<int>( base.x + ( i * StarImage::SIZE ) ),
					scast<int>( base.y ),
					StarImage::GetHandle( level, 0 ),
					TRUE
				);
			}
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
		}

		// LR表示
		{
			int baseX = 16 + ( StageImage::SIZE_LR_X >> 1 );
			int baseY = SCREEN_HEIGHT >> 1;

			// L
			{
				double scale = ( isGlowLR[0] ) ? 1.5 : 1.0;
				DrawRotaGraph
				(
					baseX,
					baseY,
					scale,
					ToRadian( 0 ),
					StageImage::GetLRHandle( /* isL = */true, isGlowLR[0] ),
					TRUE
				);
			}
			// R
			{
				double scale = ( isGlowLR[1] ) ? 1.5 : 1.0;
				DrawRotaGraph
				(
					SCREEN_WIDTH - baseX,
					baseY,
					scale,
					ToRadian( 0 ),
					StageImage::GetLRHandle( /* isL = */false, isGlowLR[1] ),
					TRUE
				);
			}
		}
	}
}

void Cursor::Init()
{
	pos = StageSelect::GetPosLeftTop();
}
void Cursor::Uninit()
{

}

void Cursor::Update()
{
	if ( isDecision )
	{
		return;
	}
	// else

	Move();
	if ( !isDoneMove )
	{
		Interpolate();
	}

	StageSelect::SetBackGlow( isChooseBack );
	GlowUpdate();

	if ( IS_TRG_EXPOSURE )
	{
		isDecision = true;
		PlaySE( M_E_NEXT );
	}
}

void Cursor::Move()
{
	constexpr float RESPONSE_MOVE_AMOUNT = 48.0f;	// 押した瞬間に，かならず移動させる量

	bool isLB = false, isRB = false, isInput = false;

	if ( IS_TRG_L ) { isLB = true; }
	if ( IS_TRG_R ) { isRB = true; }
	if ( !isChooseBack && isLB && !isRB ) { pos.y -= RESPONSE_MOVE_AMOUNT; isInput = true; }
	if ( !isChooseBack && isRB && !isLB ) { pos.y -= RESPONSE_MOVE_AMOUNT; isInput = true; }

	if ( isInput )
	{
		// Reaction
		{
			constexpr int GLOW_TIME = 10;

			StageSelect::SetLRGlow( isLB, /* isGlow = */true );
			glowTimer[( isLB ) ? 0 : 1] = GLOW_TIME;
			PlaySE( M_SELECT );
		}

		isDoneMove = false;

		int moveAmount = StageSelect::GetMaxDisplayNumber();
		int pageNum = ( ( nowStageNumber - 1 ) / StageSelect::GetMaxDisplayNumber() ); // 0始まり

		if ( isLB )
		{
			if ( 0 == pageNum )
			{
				// 最初のステージにカーソルが合っている場合のみ，最後に合わせる
				nowStageNumber =
					( 1 == nowStageNumber )
					? FileIO::GetMaxStageNumber()
					: 1;
			}
			else
			{
				nowStageNumber -= moveAmount;
			}
		}
		if ( isRB )
		{
			// 開いているのが最後のページだったら
			if ( FileIO::GetMaxStageNumber() - 1 < ( pageNum + 1 ) * StageSelect::GetMaxDisplayNumber() )
			{
				// 最後のステージにカーソルが合っている場合のみ，最初に合わせる
				nowStageNumber =
					( FileIO::GetMaxStageNumber()  == nowStageNumber )
					? 1
					: FileIO::GetMaxStageNumber();
			}
			else
			{
				nowStageNumber += moveAmount;
				if ( FileIO::GetMaxStageNumber() < nowStageNumber )
				{
					nowStageNumber = FileIO::GetMaxStageNumber();
				}
			}
		}

		return;
	}
	// else

	isInput = false;

	bool isUp	= false, isDown		= false;

	if ( IS_TRG_UP		) { isUp	= true; }
	if ( IS_TRG_DOWN	) { isDown	= true; }
	if ( !isChooseBack	&& isUp	&& !isDown	) { pos.y -= RESPONSE_MOVE_AMOUNT; isInput = true; }
	if (				isDown	&& !isUp	) { pos.y += RESPONSE_MOVE_AMOUNT; isInput = true; }

	if ( isInput )
	{
		PlaySE( M_SELECT );

		isDoneMove = false;

		int moveAmount = StageSelect::GetMaxRow();

		int remStageNumber = ( nowStageNumber - 1 ) % StageSelect::GetMaxDisplayNumber();	// 0始まり
		int clum = remStageNumber / StageSelect::GetMaxRow();	// 0始まり

		if ( isUp )
		{
			if ( !isChooseBack && 0 == clum )
			{
				isChooseBack = true;
				nowStageNumber -= remStageNumber % StageSelect::GetMaxRow();	// Backから下入力した際に，左上にカーソルを合わせるため
			}
			if ( 0 < clum )
			{
				nowStageNumber -= moveAmount;
			}
		}
		if ( isDown )
		{
			if ( isChooseBack )
			{
				isChooseBack = false;
			}
			else if ( nowStageNumber <= FileIO::GetMaxStageNumber() - moveAmount && clum != ( StageSelect::GetMaxColumn() - 1 ) )
			{
				nowStageNumber += moveAmount;
			}
		}

		return;
	}
	// else

	isInput = false;
	bool isLeft = false, isRight = false;

	if ( IS_TRG_LEFT	) { isLeft	= true; }
	if ( IS_TRG_RIGHT	) { isRight	= true; }
	if ( !isChooseBack && isLeft	&& !isRight	) { pos.x -= RESPONSE_MOVE_AMOUNT; isInput = true; }
	if ( !isChooseBack && isRight	&& !isLeft	) { pos.x += RESPONSE_MOVE_AMOUNT; isInput = true; }

	if ( isInput )
	{
		PlaySE( M_SELECT );

		isDoneMove = false;

		int moveAmount = 1;

		if ( isLeft )
		{
			nowStageNumber -= moveAmount;
			if ( !nowStageNumber )
			{
				nowStageNumber = FileIO::GetMaxStageNumber();
			}
		}
		if ( isRight )
		{
			nowStageNumber += moveAmount;
			if ( FileIO::GetMaxStageNumber() < nowStageNumber )
			{
				nowStageNumber = 1;
			}
		}
	}
}

void Cursor::Interpolate()
{
	constexpr float RESISTANCE		= 0.3f;
	constexpr float LOWER_DISTANCE	= 8.0f;

	Vector2 targetPos;

	if ( isChooseBack )
	{
		pos = StageSelect::GetBackPosCenter() - ( StageSelect::GetBackSize(/* isGlow = */true ) * 0.5f );
		return;
	}
	// else

	{
		int row  = ( ( nowStageNumber - 1 ) % StageSelect::GetMaxDisplayNumber() ) % StageSelect::GetMaxRow();
		int clum = ( ( nowStageNumber - 1 ) % StageSelect::GetMaxDisplayNumber() ) / StageSelect::GetMaxRow();
		targetPos.x = StageSelect::GetPosLeftTop().x + ( row  * ( StageSelect::GetSize().x + StageSelect::GetMargin().x ) );
		targetPos.y = StageSelect::GetPosLeftTop().y + ( clum * ( StageSelect::GetSize().y + StageSelect::GetMargin().y ) );
	}

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

void Cursor::GlowUpdate()
{
	for ( int i = 0; i < 2; i++ )
	{
		if ( glowTimer[i] )
		{
			if ( !( --glowTimer[i] ) )
			{
				bool isL = ( 0 == i ) ? true : false;
				StageSelect::SetLRGlow( isL, false );
			}
		}
	}
}

void Cursor::Draw( Vector2 shake ) const
{
	const Vector2 SIZE =
		( isChooseBack )
		? StageSelect::GetBackSize(/* isGlow = */true )
		: StageSelect::GetSize();

	DrawExtendGraph
	(
		scast<int>( pos.x ),
		scast<int>( pos.y ),
		scast<int>( pos.x + SIZE.x ),
		scast<int>( pos.y + SIZE.y ),
		CursorImage::GetHandle( true ),
		TRUE
	);
}