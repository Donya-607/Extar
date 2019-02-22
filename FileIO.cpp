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
			for ( int i = 0; i < scast<int>( data.size() ); i++ )
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
		if ( !galaxy.empty() )
		{
			galaxy.clear();
		}

		// ステージが見つからなくなるまで，つまりステージ数ぶんループ
		int stageNumber = 1;
		for ( ; true; stageNumber++ )
		{
			// ファイルの用意
			std::string filename = "./Data/Stages/Stage" + std::to_string( stageNumber ) + "/Stars.txt";

			std::ifstream ifs( filename, std::ios::in );
			if ( !ifs )
			{
				break;
			}
			// else


			// データの用意
			int row, column, width, height, level;
			std::array<int *, 5> data =
			{
				&row, &column, &width, &height, &level
			};

			std::vector<Star> stars;	// １ステージの星たち

			// 読み込み
			for ( ; ; )
			{
				for ( int i = 0; i < scast<int>( data.size() ); i++ )
				{
					std::string buf;

					// 最後のみ区切り文字を変えないと，改行して次の数字まで読んでしまう
					if ( i == scast<int>( data.size() ) - 1 )
					{
						std::getline( ifs, buf, '\n' );
					}
					else
					{
						std::getline( ifs, buf, ',' );
					}

					*data[i] = atoi( buf.c_str() );
				}

				if ( ifs.eof() )
				{
					break;
				}
				// else

				stars.push_back( Star() );
				stars.back().SetData( row, column, width, height, level );
			}

			galaxy.push_back( stars );

			ifs.close();
		}

		maxStageNumber = stageNumber;
	}

	void WriteCamera( int stageNumber, const Camera *data )
	{
		// フォルダ・ファイルの準備
		std::string filename = "./Data/Stages/Stage" + std::to_string( stageNumber );
		_mkdir( filename.c_str() );

		filename += "/Camera.txt";

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
		// フォルダ・ファイルの準備
		std::string filename = "./Data/Stages/Stage" + std::to_string( stageNumber );
		_mkdir( filename.c_str() );

		filename += "/Stars.txt";

		std::ofstream ofs( filename, std::ios::out );
		if ( !ofs )
		{
			assert( !"ファイルオープンに失敗しました：WriteStars()" );
			exit( EXIT_FAILURE );
			return;
		}
		// else


		// データの用意・書き込み
		for ( int i = 0; i < scast<int>( data->size() ); i++ )
		{
			int row, column, width, height, level;
			( data->at( i ) ).AcquireData( &row, &column, &width, &height, &level );

			ofs << row		<< ',' << column << ','
				<< width	<< ',' << height << ','
				<< level	<< std::endl;
		}

		ofs.close();
	}

	int  GetStarsArraySize( int stageNumber )
	{
		return scast<int>( ( galaxy.at( stageNumber ) ).size() );
	}
	std::vector<Star> FetchStarsInfo( int stageNumber )
	{
		assert( stageNumber <= scast<int>( galaxy.size() ) );

		return  galaxy.at( stageNumber - 1 );
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
		return maxStageNumber;
	}
}