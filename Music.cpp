#include <assert.h>
#include "DxLib.h"
#include "Music.h"

int BGMhandles[BGM_END]	= { 0 };
int SEhandles[SE_END]	= { 0 };

void MusicInit()
{
	// ----------------------------- Load -----------------------------------

	BGMhandles[M_Game]			= LoadSoundMem( "./Data/Sounds/BGM/n82.mp3" );
	BGMhandles[M_Title]			= LoadSoundMem( "./Data/Sounds/BGM/n69.mp3" );
	//BGMhandles[]				= LoadSoundMem( "./Data/Sounds/BGM/.wav" );

	SEhandles[M_OPEN]			= LoadSoundMem( "./Data/Sounds/SE/Player/Open.wav" );

	SEhandles[M_E_NEXT]			= LoadSoundMem( "./Data/Sounds/SE/Editor/Next.wav" );
	SEhandles[M_E_BACK]			= LoadSoundMem( "./Data/Sounds/SE/Editor/Back.wav" );
	//SEhandles[]				= LoadSoundMem("./Data/Sounds/SE/.wav");

	// ----------------------------------------------------------------------
}

void MusicUninit()
{
	//���y�t�@�C���̑S�J��
	InitSoundMem();
}

void PlayBGM( BGM type )
{
	// ���łɖ��Ă�����̂���������C�~�߂�
	StopBGM();

	// �G���[�`�F�b�N�i�����������̏ꍇ�̂݁C�炷�j
	for ( int i = 0; i < BGM_END; i++ )
	{
		if ( type == i )
		{
			PlaySoundMem( BGMhandles[type], DX_PLAYTYPE_LOOP );
			return;
		}
	}

	assert( !"�����G���[�FPlayBGM" );
}
void PlaySE( SE type )
{
	// �G���[�`�F�b�N�i�����������̏ꍇ�̂݁C�炷�j
	for ( int i = 0; i < SE_END; i++ )
	{
		if ( type == i )
		{
			PlaySoundMem( SEhandles[type], DX_PLAYTYPE_BACK );
			return;
		}
	}

	assert( !"�����G���[�FPlaySE" );
}

void StopBGM()
{
	for ( int i = 0; i < BGM_END; i++ )
	{
		if ( CheckSoundMem( BGMhandles[i] ) )
		{
			StopSoundMem( BGMhandles[i] );
		}
	}
}