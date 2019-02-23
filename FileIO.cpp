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
	static std::vector<std::vector<int>>  numMoves;

	// TODO:各要素で読み込みが別なので，フォルダの数を不揃いにできてしまう
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

		maxStageNumber = stageNumber - 1;	// 見つからなかったから抜けている -> 最後の１つ後ろを指している
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

		maxStageNumber = stageNumber - 1;	// 見つからなかったから抜けている -> 最後の１つ後ろを指している
	}
	void ReadAllNumMoves()
	{
		if ( !numMoves.empty() )
		{
			numMoves.clear();
		}

		// ステージが見つからなくなるまで，つまりステージ数ぶんループ
		int stageNumber = 1;
		for ( ; true; stageNumber++ )
		{
			// ファイルの用意
			std::string filename = "./Data/Stages/Stage" + std::to_string( stageNumber ) + "/NumberOfMoves.txt";

			std::ifstream ifs( filename, std::ios::in );
			if ( !ifs )
			{
				break;
			}
			// else


			// データの用意
			std::vector<int> numbers;	// １ステージの評価基準たち

			// 読み込み
			for ( ; ; )
			{
				std::string buf;
				std::getline( ifs, buf, '\n' );

				if ( ifs.eof() )
				{
					break;
				}
				// else

				numbers.push_back( atoi( buf.c_str() ) );
			}

			numMoves.push_back( numbers );

			ifs.close();
		}

		maxStageNumber = stageNumber - 1;	// 見つからなかったから抜けている -> 最後の１つ後ろを指している
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
		assert( stageNumber <= scast<int>( galaxy.size() ) );

		return scast<int>( ( galaxy.at( stageNumber - 1 ) ).size() );
	}
	std::vector<Star> FetchStarsInfo( int stageNumber )
	{
		assert( stageNumber <= scast<int>( galaxy.size() ) );

		return  galaxy.at( stageNumber - 1 );
	}

	void WriteNumMoves( int stageNumber, const std::vector<int> *data )
	{
		// フォルダ・ファイルの準備
		std::string filename = "./Data/Stages/Stage" + std::to_string( stageNumber );
		_mkdir( filename.c_str() );

		filename += "/NumberOfMoves.txt";

		std::ofstream ofs( filename, std::ios::out );
		if ( !ofs )
		{
			assert( !"ファイルオープンに失敗しました：WriteNumMoves()" );
			exit( EXIT_FAILURE );
			return;
		}
		// else


		// データの用意・書き込み
		for ( int i = 0; i < scast<int>( data->size() ); i++ )
		{
			ofs << data->at( i ) << std::endl;
		}

		ofs.close();
	}
	int  GetNumMovesArraySize( int stageNumber )
	{
		assert( stageNumber <= scast<int>( numMoves.size() ) );

		return scast<int>( ( numMoves.at( stageNumber - 1 ) ).size() );
	}
	std::vector<int> FetchNumMovesInfo( int stageNumber )
	{
		assert( stageNumber <= scast<int>( numMoves.size() ) );

		return numMoves.at( stageNumber - 1 );
	}

	void ReleaseCameraData()
	{
		std::vector<Camera>().swap( cameras );
	}
	void ReleaseStarsData()
	{
		std::vector<std::vector<Star>>().swap( galaxy );
	}
	void ReleaseNumMovesData()
	{
		std::vector<std::vector<int>>().swap( numMoves );
	}

	int  GetMaxStageNumber()
	{
		return maxStageNumber;
	}

#if USE_IMGUI

	static int  nowStageNumber	 = 1;	// 1始まり
	static bool isCreateNewStage = false;

	void UpdateNowStageNumberByImGui()
	{
		isCreateNewStage = false;

		ImGui::Begin( "Change_Save&Load_StageNumber" );

		ImGui::Text( "IO_StageNumber : %d", nowStageNumber );

		ImGui::BeginChild( ImGui::GetID( (void*)0 ), ImVec2( 250, 100 ), ImGuiWindowFlags_NoTitleBar );
		int end = maxStageNumber + 1;
		for ( int i = 1; i <= end; i++ )
		{
			std::string stageName = "Stage[ " + std::to_string( i ) + " ]";

			if ( ImGui::Button( stageName.c_str() ) )
			{
				nowStageNumber = i;
			}
		}
		ImGui::EndChild();

		assert( 0 < nowStageNumber );

		if ( maxStageNumber < nowStageNumber )
		{
			if ( ImGui::Button( "Create_NewStage" ) )
			{
				isCreateNewStage = true;
			}
		}

		ImGui::End();
	}
	int  GetNowStageNumber()
	{
		return nowStageNumber;
	}
	bool IsCreateNewStage()
	{
		return isCreateNewStage;
	}

#endif // USE_IMGUI
}