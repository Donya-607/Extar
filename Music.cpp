#include <assert.h>
#include "DxLib.h"
#include "Music.h"

int BGMhandles[BGM_END]	= { 0 };
int SEhandles[SE_END]	= { 0 };

void MusicInit()
{
	// ----------------------------- Load -----------------------------------

	BGMhandles[M_Game]			= LoadSoundMem( "./Data/Sounds/BGM/Game.wav" );
	BGMhandles[M_Title_Select]	= LoadSoundMem( "./Data/Sounds/BGM/Title_Select.wav" );
	//BGMhandles[]				= LoadSoundMem( "./Data/Sounds/BGM/.wav" );

	SEhandles[M_DECISION]		= LoadSoundMem( "./Data/Sounds/SE/Decision.wav" );
	SEhandles[M_EXPOSURE]		= LoadSoundMem( "./Data/Sounds/SE/Camera/Exposure.wav" );
	SEhandles[M_FAILURE]		= LoadSoundMem( "./Data/Sounds/SE/Camera/Failure.wav" );
	SEhandles[M_GOTO_TITLE]		= LoadSoundMem( "./Data/Sounds/SE/Select/Back.wav" );
	SEhandles[M_PAUSE]			= LoadSoundMem( "./Data/Sounds/SE/Pause/Pause.wav" );
	SEhandles[M_SELECT]			= LoadSoundMem( "./Data/Sounds/SE/Select/Move.wav" );
	SEhandles[M_RECORD_STAR]	= LoadSoundMem( "./Data/Sounds/SE/Result/Reward.wav" );
	SEhandles[M_VOICE]			= LoadSoundMem( "./Data/Sounds/SE/Human/Voice.wav" );

	// DEBUG:���C�|�[�Y���ɖ炵�Ă���

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
	StopBGM( type );

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

void StopBGM( BGM type )
{
	for ( int i = 0; i < BGM_END; i++ )
	{
		if ( CheckSoundMem( BGMhandles[i] ) )
		{
			// �������̂�������C���̂܂ܖ炵������悤��
			if ( i == type )
			{
				//continue;
			}
			// else

			StopSoundMem( BGMhandles[i] );
		}
	}
}