#ifndef INCLUDED_FILEIO_H_
#define INCLUDED_FILEIO_H_

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
	void ReadAllNumMoves();

	void WriteCamera( int stageNumber, const Camera *data );
	int  GetCameraArraySize();
	Camera FetchCameraInfo( int stageNumber );

	void WriteStars( int stageNumber, const std::vector<Star> *data );
	int  GetStarsArraySize( int stageNumber );
	std::vector<Star> FetchStarsInfo( int stageNumber );

	void WriteNumMoves( int stageNumber, const std::vector<int> *data );
	int  GetNumMovesArraySize( int stageNumber );
	std::vector<int> FetchNumMovesInfo( int stageNumber );

	void ReleaseCameraData();
	void ReleaseStarsData();
	void ReleaseNumMovesData();

	int  GetMaxStageNumber();

#if USE_IMGUI

	void UpdateShowWIndowState();
	bool IsShowImGuiWindow();

	void UpdateNowStageNumberByImGui();
	int  GetNowStageNumber();

	bool IsCreateNewStage();

#endif // USE_IMGUI

}

#endif //INCLUDED_FILEIO_H_
