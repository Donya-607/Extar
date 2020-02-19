#include "UnlockAnnouncer.h"

#include <algorithm> // std::min().

#include "DxLib.h"
#include "Common.h"
#include "FileIO.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

#undef max
#undef min

namespace
{
	static float	amplAlpha			= 7.0f;
	static float	amplBright			= 9.0f;

	static int		borderBright		= 64;

	static int		waitForTransition	= 100;

	static int		beginShowImageTime	= 40 + waitForTransition;
	static int		slideImageTime		= 24;

	static int		beginFadeoutTime	= 160 + waitForTransition;
	static int		takeFadeoutTime		= 20;

	static Vector2	imagePosStart{ 560.0f, 572.0f }; // ScreenSize/2 - ImageSize/2
	static Vector2	imagePosDest { 560.0f, 508.0f }; // ScreenSize/2 - ImageSize/2

	static int hImage = 0;
	void Load()
	{
		if ( hImage != 0 ) { return; }
		// else

		hImage = LoadGraph( "./Data/Images/Select/Unlock.png" );
	}
	void Release()
	{
		DeleteGraph( hImage );
		hImage = 0;
	}
}

void UnlockAnnouncer::LoadImages()
{
	Load();
}
void UnlockAnnouncer::ReleaseImages()
{
	Release();
}

void UnlockAnnouncer::Update()
{
	UseImGui();

	timer++;

	AssignAlpha();
	AssignBright();

	AssignImagePos();
}

void UnlockAnnouncer::Draw()
{
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, alpha );
	DrawGraphF
	(
		pos.x, pos.y,
		hImage, TRUE
	);
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 );
}

bool UnlockAnnouncer::ShouldRemove() const
{
	return ( beginFadeoutTime + takeFadeoutTime < timer ) ? true : false;
}

int UnlockAnnouncer::GetDrawAlpha()  const
{
	return std::min( 255, alpha );
}
int UnlockAnnouncer::GetDrawBright() const
{
	return std::min( 255, bright );
}

void UnlockAnnouncer::AssignAlpha()
{
	if ( timer <= beginShowImageTime ) { alpha = 0; }
	// else

	if ( beginFadeoutTime <= timer )
	{
		if ( beginFadeoutTime + takeFadeoutTime <= timer )
		{
			alpha = 0;
		}
		else
		{
			alpha = 255 - scast<int>( 255.0f * scast<float>( timer - beginFadeoutTime ) / scast<float>( takeFadeoutTime ) );
		}

		return;
	}
	// else

	alpha = scast<int>( scast<float>( timer - beginShowImageTime ) * amplAlpha );
	alpha = std::max( 0, std::min( 255, alpha ) );
}
void UnlockAnnouncer::AssignBright()
{
	if ( beginFadeoutTime <= timer )
	{
		if ( beginFadeoutTime + takeFadeoutTime <= timer )
		{
			bright = 255;
		}
		else
		{
			bright = scast<int>( 255.0f * scast<float>( timer - beginFadeoutTime ) / scast<float>( takeFadeoutTime ) );
			bright = std::max( borderBright, std::min( 255, bright ) );
		}

		return;
	}
	// else

	bright = 255 - scast<int>( scast<float>( std::max( 0, timer - waitForTransition ) ) * amplBright );
	bright = std::max( borderBright, std::min( 255, bright ) );
}

void UnlockAnnouncer::AssignImagePos()
{
	const Vector2 screenSize
	{
		scast<float>( SCREEN_WIDTH  ),
		scast<float>( SCREEN_HEIGHT )
	};

	if ( timer <= beginShowImageTime )
	{
		pos = screenSize;
		return;
	}
	// else
	if ( beginFadeoutTime <= timer )
	{
		return;
	}
	// else

	const Vector2 diff = imagePosDest - imagePosStart;
	float percent = std::min( 1.0f, scast<float>( timer - beginShowImageTime ) / scast<float>( slideImageTime ) );
	percent = std::max( 0.0f, std::min( 1.0f, percent ) );

	pos = imagePosStart + ( diff * percent );
}

void UnlockAnnouncer::UseImGui()
{
#if USE_IMGUI
	if ( !FileIO::IsShowImGuiWindow() ) { return; }
	// else

	if ( !ImGui::Begin( "UnlockAnnouncer" ) ) { return; }
	// else

	ImGui::Text( "Timer:%d",  timer  );
	ImGui::Text( "Alpha:%d",  alpha  );
	ImGui::Text( "Bright:%d", bright );
	if ( ImGui::Button( "Timer Reset" ) )
	{
		timer = 0;
	}

	ImGui::DragFloat( "Ampl:Alpha",  &amplAlpha,  0.01f, 0.0f );
	ImGui::DragFloat( "Ampl:Bright", &amplBright, 0.01f, 0.0f );
	ImGui::DragInt( "Border:Bright", &borderBright, 1.0f, 0, 255 );

	ImGui::DragInt( "Frame:Wait Transition",      &waitForTransition,  1.0f, 1 );

	ImGui::DragInt( "Frame:Begin ShowImage",      &beginShowImageTime, 1.0f, 1 );
	ImGui::DragInt( "Frame:Taking SlidingImage",  &slideImageTime,     1.0f, 1 );

	ImGui::DragFloat2( "Pos:Start Image", &imagePosStart.x, 1.0f, 0.0f, 1920.0f );
	ImGui::DragFloat2( "Pos:Dest  Image", &imagePosDest.x,   1.0f, 0.0f, 1920.0f );

	ImGui::DragInt( "Frame:Begin Fade-out",   &beginFadeoutTime, 1.0f, 1 );
	ImGui::DragInt( "Frame:Taking Fade-out",  &takeFadeoutTime,  1.0f, 1 );

	ImGui::End();
#endif // USE_IMGUI
}
