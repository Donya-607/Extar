#include "DxLib.h"
#include "Common.h"
#include "Music.h"
#include "Input.h"

#include "Camera.h"
#include "Grid.h"

#include "FileIO.h"

#include <algorithm>	// Clampで使用
#undef min				// Clampで使用
#undef max				// Clampで使用

namespace CameraImage
{
	static int hCamera = 0;

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
		if ( 0 != hCamera )
		{
			return;
		}
		// else

		hCamera = LoadGraph( "./Data/Images/Camera/CameraFrame.png" );
	}
	void Release()
	{
		DeleteGraph( hCamera );
		hCamera = 0;
	}

	int  GetHandle()
	{
		return hCamera;
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
	Move();

	Shake();

	Exposure();

#if USE_IMGUI

	ChangeParametersByImGui();

#endif // USE_IMGUI
}

void Camera::Draw( Vector2 shake ) const
{
	DrawExtendGraph
	(
		FRAME_POS_X + scast<int>( pos.x - shake.x ),
		FRAME_POS_Y + scast<int>( pos.y - shake.y ),
		FRAME_POS_X + scast<int>( pos.x + size.x - shake.x ),
		FRAME_POS_Y + scast<int>( pos.y + size.y - shake.y ),
		CameraImage::GetHandle(),
		TRUE
	);
}

void Camera::Move()
{
	// 仮の動きとして，posを直接書き換えている

	bool isUp = false, isDown = false, isLeft = false, isRight = false;

	if ( IS_TRG_UP		) { isUp	= true;	}
	if ( IS_TRG_DOWN	) { isDown	= true;	}
	if ( IS_TRG_LEFT	) { isLeft	= true;	}
	if ( IS_TRG_RIGHT	) { isRight	= true;	}

	if ( isUp		&& !isDown	)	{ pos.y -= Grid::GetSize().y * moveAmount; column	-= moveAmount; }
	if ( isDown		&& !isUp	)	{ pos.y += Grid::GetSize().y * moveAmount; column	+= moveAmount; }
	if ( isLeft		&& !isRight	)	{ pos.x -= Grid::GetSize().x * moveAmount; row		-= moveAmount; }
	if ( isRight	&& !isLeft	)	{ pos.x += Grid::GetSize().x * moveAmount; row		+= moveAmount; }

	if ( isUp || isDown | isLeft || isRight )
	{
		ClampPos();
		if ( !ClampMatrix() )
		{
			PlaySE( M_CAMERA_MOVE );
		}
	}
}
void Camera::ClampPos()
{
	if ( !isShake )
	{
		pos.x = std::min( pos.x, scast<float>( FRAME_WIDTH  ) - ( Grid::GetSize().x * width  ) );
		pos.x = std::max( pos.x, 0.0f );
	}

	pos.y = std::min( pos.y, scast<float>( FRAME_HEIGHT ) - ( Grid::GetSize().y * height ) );
	pos.y = std::max( pos.y, 0.0f );
}
bool Camera::ClampMatrix()
{
	int oldRow		= row;
	int oldColumn	= column;

	row		= std::min( row,	Grid::GetRowMax()		- width  );
	row		= std::max( row,	0 );

	column	= std::min( column,	Grid::GetColumnMax()	- height );
	column	= std::max( column,	0 );

	if ( oldRow != row || oldColumn != column )
	{
		return true;
	}
	// else
	return false;
}

void Camera::Shake()
{
	if ( !isShake )
	{
		return;
	}
	// else

	pos -= velo;	// もどす

	constexpr float LOWER = 4.0f;
	if ( fabsf( velo.x ) < LOWER )
	{
		velo.x = 0;
		isShake = false;

		return;
	}
	// else

	constexpr float DECREASE = 0.7f;
	velo.x *= DECREASE;
	velo.x *= -1;

	pos += velo;	// ずらす
}

void Camera::Exposure()
{
	if ( !IS_TRG_EXPOSURE )
	{
		isExposure = false;

		return;
	}
	// else

	isExposure = true;
}

void Camera::SetShake()
{
	isShake = true;
	pos -= velo;	// もどしておく

	const Vector2 INIT_SHAKE{ 48.0f, 0 };
	velo = INIT_SHAKE;

	pos += velo;
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

	MoveBySelfMatrix();

	return true;
}

void Camera::MoveBySelfMatrix()
{
	pos.x = ( row		* Grid::GetSize().x );
	pos.y = ( column	* Grid::GetSize().y );
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