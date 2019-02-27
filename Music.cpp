#include <assert.h>
#include "DxLib.h"
#include "Music.h"

int BGMhandles[BGM_END]	= { 0 };
int SEhandles[SE_END]	= { 0 };

void MusicInit()
{
	// ----------------------------- Load -----------------------------------

	BGMhandles[M_Game]			= LoadSoundMem( "./Data/Sounds/BGM/n82.mp3" );
	BGMhandles[M_Title_Select]	= LoadSoundMem( "./Data/Sounds/BGM/Title_Select.mp3" );
	//BGMhandles[]				= LoadSoundMem( "./Data/Sounds/BGM/.wav" );

	SEhandles[M_EXPOSURE]		= LoadSoundMem( "./Data/Sounds/SE/Camera/Exposure.wav" );
	SEhandles[M_FAILURE]		= LoadSoundMem( "./Data/Sounds/SE/Camera/Failure.wav" );

	SEhandles[M_SELECT]			= LoadSoundMem( "./Data/Sounds/SE/Select/Move.wav" );

	SEhandles[M_RECORD_STAR]	= LoadSoundMem( "./Data/Sounds/SE/Result/Reward.wav" );

	SEhandles[M_E_NEXT]			= LoadSoundMem( "./Data/Sounds/SE/Editor/Next.wav" );
	SEhandles[M_E_BACK]			= LoadSoundMem( "./Data/Sounds/SE/Editor/Back.wav" );
	//SEhandles[]				= LoadSoundMem("./Data/Sounds/SE/.wav");

	// ----------------------------------------------------------------------
}

void MusicUninit()
{
	//音楽ファイルの全開放
	InitSoundMem();
}

void PlayBGM( BGM type )
{
	// すでに鳴っているものがあったら，止める
	StopBGM( type );

	// エラーチェック（正しい引数の場合のみ，鳴らす）
	for ( int i = 0; i < BGM_END; i++ )
	{
		if ( type == i )
		{
			PlaySoundMem( BGMhandles[type], DX_PLAYTYPE_LOOP );
			return;
		}
	}

	assert( !"引数エラー：PlayBGM" );
}
void PlaySE( SE type )
{
	// エラーチェック（正しい引数の場合のみ，鳴らす）
	for ( int i = 0; i < SE_END; i++ )
	{
		if ( type == i )
		{
			PlaySoundMem( SEhandles[type], DX_PLAYTYPE_BACK );
			return;
		}
	}

	assert( !"引数エラー：PlaySE" );
}

void StopBGM( BGM type )
{
	for ( int i = 0; i < BGM_END; i++ )
	{
		if ( CheckSoundMem( BGMhandles[i] ) )
		{
			// 同じものだったら，そのまま鳴らし続けるように
			if ( i == type )
			{
				continue;
			}
			// else

			StopSoundMem( BGMhandles[i] );
		}
	}
}