#include "DxLib.h"
#include "Common.h"
#include "Music.h"
#include "Input.h"

#include "Camera.h"
#include "Grid.h"

#include "FileIO.h"

#include <algorithm>	// Clamp�Ŏg�p
#undef min				// Clamp�Ŏg�p
#undef max				// Clamp�Ŏg�p

namespace CameraImage
{
	static int hCamera = 0;

	void Load()
	{
		// ���łɒl�������Ă�����C�ǂݍ��񂾂��̂Ƃ݂Ȃ��Ĕ�΂�
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
	// ����ɐݒ�
	row		= 0;
	column	= 0;

	// HACK:*this = �Œ��ڑ������̂͂n�j���H�댯���H
	*this = FileIO::FetchCameraInfo( stageNumber );

	// �O���b�h�T�C�Y�́C���̎��_�Ő��������̂ɐݒ肳��Ă���Ƃ���
	size.x	= Grid::GetSize().x * width;
	size.y	= Grid::GetSize().y * height;

	pos.x	= row		* size.x;
	pos.y	= column	* size.y;

#if USE_IMGUI

	this->stageNumber = stageNumber;

#endif // USE_IMGUI

}
void Camera::Uninit()
{

}

void Camera::Update()
{
	Move();

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
	// ���̓����Ƃ��āCpos�𒼐ڏ��������Ă���

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

void Camera::Exposure()
{
	if ( !IS_TRG_J_X_EXPOSURE )
	{
		isExposure = false;

		return;
	}
	// else

	isExposure = true;
}

Box Camera::FetchColWorldPos() const
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

#if USE_IMGUI

void Camera::ChangeParametersByImGui()
{
	ImGui::Begin( "Camera_Parameters", nullptr, ImGuiWindowFlags_MenuBar );

	if ( ImGui::BeginMenuBar() )
	{
		if ( ImGui::BeginMenu( "File" ) )
		{
			if ( ImGui::MenuItem( "Save" ) )
			{
				FileIO::WriteCamera( stageNumber, this );

				// �t�@�C���ɕۑ����邾���œK�p���Ȃ����߁C���łɂ܂Ƃ߂ēǂݍ��݂Ȃ���
				FileIO::ReadAllCamera();
				FileIO::ReadAllStars();

				PlaySE( M_E_NEXT );
			}
			if ( ImGui::MenuItem( "Load" ) )
			{
				// HACK:*this = �Œ��ڑ������̂͂n�j���H�댯���H
				*this = FileIO::FetchCameraInfo( stageNumber );

				PlaySE( M_E_BACK );
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::SliderInt( "IO_StageNumber", &stageNumber, 1, 30 );

	ImGui::SliderInt( "Width",  &width,  1, Grid::GetRowMax() - 1 );
	ImGui::SliderInt( "Height", &height, 1, Grid::GetColumnMax() - 1 );

	ImGui::SliderInt( "MovementAmount", &moveAmount, 1, 12 );

	ImGui::End();

	ClampPos();
	ClampMatrix();

	size.x = Grid::GetSize().x * width;
	size.y = Grid::GetSize().y * height;
}

#endif // USE_IMGUI