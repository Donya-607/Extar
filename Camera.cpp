#include "DxLib.h"
#include "Common.h"
#include "Music.h"
#include "Input.h"

#include "Camera.h"
#include "Grid.h"

#include "FileIO.h"

#include "Easing.h"

#include <algorithm>	// Clampで使用
#undef min				// Clampで使用
#undef max				// Clampで使用

namespace CameraImage
{
	constexpr int SIZE_X = 192;
	constexpr int SIZE_Y = 128;

	static int hCamera[2];	// 0:Dark, 1:Glow

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( 0 != hCamera[0] )
		{
			return;
		}
		// else

		LoadDivGraph
		(
			"./Data/Images/Camera/CameraFrame.png",
			2,
			2, 1,
			SIZE_X, SIZE_Y,
			hCamera
		);
	}
	void Release()
	{
		for ( int i = 0; i < 2; i++ )
		{
			DeleteGraph( hCamera[i] );
			hCamera[i] = 0;
		}
	}

	int  GetHandle( int index )
	{
		assert( 0 <= index && index < 2 );

		return hCamera[index];
	}
}

void Camera::Init( int stageNumber )
{
	// 左上に設定
	row		= 0;
	column	= 0;

	// HACK:*this = で直接代入するのはＯＫか？危険か？
	*this = FileIO::FetchCameraInfo( stageNumber );

	// グリッドサイズは，この時点で正しいものに設定されているとする
	size.x	= Grid::GetSize().x * width;
	size.y	= Grid::GetSize().y * height;

	pos.x	= row		* size.x;
	pos.y	= column	* size.y;
}
void Camera::Uninit()
{

}

void Camera::Update()
{
	if ( IsTrigger( InputTrigger::ToggleCamera ) )
	{
		ToggleAspectRatio();
		PlaySE( M_TOGGLE_CAMERA );
	}

	Move();
	Interpolate();

	Shake();

	Exposure();

	if ( isGlow )
	{
		if ( !( glowTimer-- ) )
		{
			isGlow = false;
		}
	}

#if USE_IMGUI

	ChangeParametersByImGui();

#endif // USE_IMGUI
}

void Camera::Draw( Vector2 shake ) const
{
	// 回転対応前のもの
	//DrawExtendGraph
	//(
	//	FRAME_POS_X + scast<int>( pos.x - denialShake.x - shake.x ),
	//	FRAME_POS_Y + scast<int>( pos.y - denialShake.y - shake.y ),
	//	FRAME_POS_X + scast<int>( pos.x + size.x - denialShake.x - shake.x ),
	//	FRAME_POS_Y + scast<int>( pos.y + size.y - denialShake.y - shake.y ),
	//	CameraImage::GetHandle( ( isGlow ) ? 1 : 0 ),
	//	TRUE
	//);

	bool	shouldRot	= ( width < height ) ? true : false;
	double	radian		= ( shouldRot ) ? ToRadian( 90.0 ) : 0.0;
	Vector2	extend
	{
		// 画像サイズとの比率を出す
		// min(), max()を使っているのは，縦横比が反対になっていても元の大きさ（横がでかい）のサイズをかけるため
		( Grid::GetSize().x * std::max( width, height ) ) / scast<float>( CameraImage::SIZE_X ),
		( Grid::GetSize().y * std::min( width, height ) ) / scast<float>( CameraImage::SIZE_Y ),
	};
	Vector2 halfSize{ size.x * 0.5f, size.y * 0.5f };

	DrawRotaGraph3
	(
		FRAME_POS_X + scast<int>( pos.x + ( halfSize.x ) - denialShake.x - shake.x ),
		FRAME_POS_Y + scast<int>( pos.y + ( halfSize.y ) - denialShake.y - shake.y ),
		CameraImage::SIZE_X >> 1,
		CameraImage::SIZE_Y >> 1,
		scast<double>( extend.x ),
		scast<double>( extend.y ),
		radian,
		CameraImage::GetHandle( ( isGlow ) ? 1 : 0 ),
		TRUE
	);
}

void Camera::ToggleAspectRatio()
{
	std::swap( width,  height );
	std::swap( size.x, size.y );
	ClampMatrix(); // 位置の補正
}

void Camera::Move()
{
	constexpr float RESPONSE_MOVE_AMOUNT = 12.0f;	// 押した瞬間に，かならず移動させる量

	bool isUp = false, isDown = false, isLeft = false, isRight = false;

	if ( IsTrigger( InputTrigger::Up	) ) { isUp		= true; }
	if ( IsTrigger( InputTrigger::Down	) ) { isDown	= true;	}
	if ( IsTrigger( InputTrigger::Left	) ) { isLeft	= true;	}
	if ( IsTrigger( InputTrigger::Right	) ) { isRight	= true;	}

	if ( isUp		&& !isDown	)	{ pos.y -= RESPONSE_MOVE_AMOUNT * moveAmount; column	-= moveAmount; PlaySE( M_CAMERA_MOVE ); }
	if ( isDown		&& !isUp	)	{ pos.y += RESPONSE_MOVE_AMOUNT * moveAmount; column	+= moveAmount; PlaySE( M_CAMERA_MOVE ); }
	if ( isLeft		&& !isRight	)	{ pos.x -= RESPONSE_MOVE_AMOUNT * moveAmount; row		-= moveAmount; PlaySE( M_CAMERA_MOVE ); }
	if ( isRight	&& !isLeft	)	{ pos.x += RESPONSE_MOVE_AMOUNT * moveAmount; row		+= moveAmount; PlaySE( M_CAMERA_MOVE ); }

	if ( isUp || isDown | isLeft || isRight )
	{
		// ClampPos(); 移動方法を変えたため，不要になった
		ClampMatrix();
	}
}
void Camera::Interpolate()
{
	Vector2 destination{};
	destination.x = ( row		* Grid::GetSize().x );
	destination.y = ( column	* Grid::GetSize().y );

	constexpr float LOWER_DISTANCE = 3.0f;
	if ( fabsf( ( destination - pos ).Length() ) < LOWER_DISTANCE )
	{
		pos = destination;
		return;
	}
	// else

	pos += Easing::Interpolate( pos, destination, 0.4f );
}

void Camera::ClampPos()
{
	pos.x = std::min( pos.x, scast<float>( FRAME_WIDTH  ) - ( Grid::GetSize().x * width  ) );
	pos.x = std::max( pos.x, 0.0f );

	pos.y = std::min( pos.y, scast<float>( FRAME_HEIGHT ) - ( Grid::GetSize().y * height ) );
	pos.y = std::max( pos.y, 0.0f );
}
void Camera::ClampMatrix()
{
	row		= std::min( row,	Grid::GetRowMax()		- width  );
	row		= std::max( row,	0 );

	column	= std::min( column,	Grid::GetColumnMax()	- height );
	column	= std::max( column,	0 );
}

void Camera::SetGlow()
{
	isGlow = true;

	constexpr int GLOW_TIME = 8;
	glowTimer = GLOW_TIME;
}

void Camera::Shake()
{
	if ( !isShake )
	{
		return;
	}
	// else

	constexpr float LOWER = 4.0f;
	if ( fabsf( denialShake.x ) < LOWER )
	{
		denialShake.x = 0;
		isShake = false;

		return;
	}
	// else

	constexpr float DECREASE = 0.7f;
	denialShake.x *= DECREASE;
	denialShake.x *= -1;
}

void Camera::Exposure()
{
	if ( !IsTrigger( InputTrigger::Exposure ) )
	{
		isExposure = false;

		return;
	}
	// else

	isExposure = true;
}

void Camera::SetShake()
{
	const Vector2 INIT_SHAKE{ 48.0f, 0 };
	denialShake = INIT_SHAKE;

	isShake = true;
}

Box  Camera::FetchColWorldPos() const
{
	Vector2 halfSize{ size.x * 0.5f, size.y * 0.5f };
	Vector2 base{ scast<float>( FRAME_POS_X ), scast<float>( FRAME_POS_Y ) };

	Box tmp =
	{
		base.x + ( row		* Grid::GetSize().x ) + halfSize.x,
		base.y + ( column	* Grid::GetSize().y ) + halfSize.y,
		halfSize.x,
		halfSize.y,
		true
	};

	return tmp;
}

void Camera::SaveLog()
{
	if ( !CanSaveLog() )
	{
		return;
	}
	// else

	rowStorage.push_back( row );
	clumStorage.push_back( column );
}

bool Camera::Undo()
{
	if ( !CanUndo() )
	{
		return false;
	}
	// else

	row = rowStorage.back();
	column = clumStorage.back();

	rowStorage.pop_back();
	clumStorage.pop_back();

	return true;
}

#if USE_IMGUI

void Camera::ChangeParametersByImGui()
{
	if ( !FileIO::IsShowImGuiWindow() )
	{
		return;
	}
	// else

	ImGui::Begin( "Camera_Parameters" );

	ImGui::SliderInt( "Width",  &width,  1, Grid::GetRowMax() - 1 );
	ImGui::SliderInt( "Height", &height, 1, Grid::GetColumnMax() - 1 );

	ImGui::SliderInt( "MovementAmount", &moveAmount, 1, 12 );

	if ( FileIO::GetNowStageNumber() <= FileIO::GetMaxStageNumber() )
	{
		if ( ImGui::Button( "Save" ) )
		{
			SaveData();

			// ファイルに保存するだけで適用しないため，ついでにまとめて読み込みなおす
			FileIO::ReadAllCamera();
			FileIO::ReadAllStars();
			FileIO::ReadAllNumMoves();

			PlaySE( M_E_NEXT );
		}
		if ( ImGui::Button( "Load" ) )
		{
			// HACK:*this = で直接代入するのはＯＫか？危険か？
			*this = FileIO::FetchCameraInfo( FileIO::GetNowStageNumber() );

			PlaySE( M_E_BACK );
		}
	}

	ImGui::End();

	ClampPos();
	ClampMatrix();

	size.x = Grid::GetSize().x * width;
	size.y = Grid::GetSize().y * height;
}
void Camera::SaveData()
{
	FileIO::WriteCamera( FileIO::GetNowStageNumber(), this );
}

#endif // USE_IMGUI