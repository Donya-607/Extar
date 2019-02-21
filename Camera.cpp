#include "DxLib.h"
#include "Common.h"
#include "Music.h"
#include "Input.h"

#include "Camera.h"
#include "Grid.h"

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

void Camera::Init( int sizeX, int sizeY, int movementAmount )
{
	row		= 0;
	column	= 0;

	width	= sizeX;
	height	= sizeY;

	moveAmount = movementAmount;

	// グリッドサイズは，この時点で正しいものに設定されているとする
	size.x	= Grid::GetSize().x * width;
	size.y	= Grid::GetSize().y * height;

	// 画面左上に設定
	pos.x	= row		* size.x;
	pos.y	= column	* size.y;
}
void Camera::Uninit()
{

}

void Camera::Update()
{
	Move();

#if USE_IMGUI

	ChangeParametersByImGui();

#endif // USE_IMGUI
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

	ClampPos();
	ClampMatrix();
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

#if USE_IMGUI

void Camera::ChangeParametersByImGui()
{
	ImGui::Begin( "Camera_Parameters" );

	ImGui::SliderInt( "Width",  &width,  1, Grid::GetRowMax() );
	ImGui::SliderInt( "Height", &height, 1, Grid::GetColumnMax() );

	ImGui::SliderInt( "MovementAmount", &moveAmount, 1, 12 );

	ImGui::End();

	ClampPos();
	ClampMatrix();

	size.x = Grid::GetSize().x * width;
	size.y = Grid::GetSize().y * height;
}

#endif // USE_IMGUI