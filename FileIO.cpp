#include <direct.h>	// _mkdir()を使用

#include <array>
#include <string>
#include <fstream>

#include "FileIO.h"

#include "Camera.h"
#include "Star.h"

namespace FileIO
{
	static std::vector<Camera> cameras;
	static std::vector<std::vector<Star>> galaxy;

	// TODO:カメラと星で読み込みが別なので，フォルダの数を不揃いにできてしまう
	static int maxStageNumber = 1;

	void ReadAllCamera()
	{
		if ( !cameras.empty() )
		{
			cameras.clear();
		}

		// ステージが見つからなくなるまで，つまりステージ数ぶんループ
		int stageNumber = 1;
		for ( ; true; stageNumber++ )
		{
			// ファイルの用意
			std::string filename = "./Data/Stages/Stage" + std::to_string( stageNumber ) + "/Camera.txt";

			std::ifstream ifs( filename, std::ios::in );
			if ( !ifs )
			{
				break;
			}
			// else


			// データの用意
			cameras.push_back( Camera() );
			Camera &camera = cameras.back();

			int width, height, moveAmount;
			std::array<int *, 3> data =
			{
				&width, &height, &moveAmount
			};


			// 読み込み
			for ( int i = 0; i < data.size(); i++ )
			{
				std::string buf;
				std::getline( ifs, buf, ',' );

				*data[i] = atoi( buf.c_str() );
			}
			camera.SetData( width, height, moveAmount );


			ifs.close();
		}

		maxStageNumber = stageNumber;
	}
	void ReadAllStars()
	{

	}

	void WriteCamera( int stageNumber, const Camera *data )
	{
		// フォルダ・ファイルの準備
		std::string filename = "./Data/Stages/Stage" + std::to_string( stageNumber );
		_mkdir( filename.c_str() );

		filename += +"/Camera.txt";

		std::ofstream ofs( filename, std::ios::out );
		if ( !ofs )
		{
			assert( !"ファイルオープンに失敗しました：WriteCamera()" );
			exit( EXIT_FAILURE );
			return;
		}
		// else


		// データの用意・書き込み
		int width, height, moveAmount;
		data->AcquireData( &width, &height, &moveAmount );

		ofs << width		<< ',' << height << ','
			<< moveAmount	<< std::endl;

		ofs.close();
	}
	int  GetCameraArraySize()
	{
		return scast<int>( cameras.size() );
	}
	Camera FetchCameraInfo( int stageNumber )
	{
		assert( stageNumber <= scast<int>( cameras.size() ) );

		return cameras.at( stageNumber - 1 );
	}

	void WriteStars( int stageNumber, const std::vector<Star> *data )
	{

	}

	int  GetStarsArraySize( int stageNumber )
	{
		return NULL;
	}
	Star FetchStarsInfo( int stageNumber, int index )
	{
		Star temp;
		return temp;
	}

	void ReleaseCameraData()
	{
		std::vector<Camera>().swap( cameras );
	}
	void ReleaseStarsData()
	{
		std::vector<std::vector<Star>>().swap( galaxy );
	}

	int GetMaxStageNumber()
	{

	}
}