#include <direct.h>	// _mkdir()���g�p

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

	// TODO:�J�����Ɛ��œǂݍ��݂��ʂȂ̂ŁC�t�H���_�̐���s�����ɂł��Ă��܂�
	static int maxStageNumber = 1;

	void ReadAllCamera()
	{
		if ( !cameras.empty() )
		{
			cameras.clear();
		}

		// �X�e�[�W��������Ȃ��Ȃ�܂ŁC�܂�X�e�[�W���Ԃ񃋁[�v
		int stageNumber = 1;
		for ( ; true; stageNumber++ )
		{
			// �t�@�C���̗p��
			std::string filename = "./Data/Stages/Stage" + std::to_string( stageNumber ) + "/Camera.txt";

			std::ifstream ifs( filename, std::ios::in );
			if ( !ifs )
			{
				break;
			}
			// else


			// �f�[�^�̗p��
			cameras.push_back( Camera() );
			Camera &camera = cameras.back();

			int width, height, moveAmount;
			std::array<int *, 3> data =
			{
				&width, &height, &moveAmount
			};


			// �ǂݍ���
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
		// �t�H���_�E�t�@�C���̏���
		std::string filename = "./Data/Stages/Stage" + std::to_string( stageNumber );
		_mkdir( filename.c_str() );

		filename += +"/Camera.txt";

		std::ofstream ofs( filename, std::ios::out );
		if ( !ofs )
		{
			assert( !"�t�@�C���I�[�v���Ɏ��s���܂����FWriteCamera()" );
			exit( EXIT_FAILURE );
			return;
		}
		// else


		// �f�[�^�̗p�ӁE��������
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