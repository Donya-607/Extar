#ifndef INCLUDED_LOADFILE_H_
#define INCLUDED_LOADFILE_H_

#include <vector>

#include "Common.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

class Camera;
class Star;

namespace FileIO
{
	void ReadAllCamera();
	void ReadAllStars();

	void WriteCamera( int stageNumber, const Camera *data );
	int  GetCameraArraySize();
	Camera FetchCameraInfo( int stageNumber );

	void WriteStars( int stageNumber, const std::vector<Star> *data );
	int  GetStarsArraySize( int stageNumber );
	std::vector<Star> FetchStarsInfo( int stageNumber );

	void ReleaseCameraData();
	void ReleaseStarsData();

	int GetMaxStageNumber();
}

#endif //INCLUDED_LOADFILE_H_
