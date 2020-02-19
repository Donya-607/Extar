#include <assert.h>
#include "DxLib.h"
#include "Music.h"

int BGMhandles[BGM_END]	= { 0 };
int SEhandles[SE_END]	= { 0 };

void MusicInit()
{
	// ----------------------------- Load -----------------------------------

	BGMhandles[M_Game]			= LoadSoundMem( "./Data/Sounds/BGM/Game.wav" );
	BGMhandles[M_Result]		= LoadSoundMem( "./Data/Sounds/BGM/Result.wav" );
	BGMhandles[M_Title_Select]	= LoadSoundMem( "./Data/Sounds/BGM/Title_Select.wav" );
	//BGMhandles[]				= LoadSoundMem( "./Data/Sounds/BGM/.wav" );

	SEhandles[M_ALIGN_STARS]	= LoadSoundMem( "./Data/Sounds/SE/Game/Align.wav" );
	SEhandles[M_CAMERA_MOVE]	= LoadSoundMem( "./Data/Sounds/SE/Camera/CameraFrameMove.wav" );
	SEhandles[M_CANT_PUSH]		= LoadSoundMem( "./Data/Sounds/SE/CantPush.wav" );
	SEhandles[M_DECISION]		= LoadSoundMem( "./Data/Sounds/SE/Decision.wav" );
	SEhandles[M_EXPOSURE]		= LoadSoundMem( "./Data/Sounds/SE/Camera/Exposure.wav" );
	SEhandles[M_FAILURE]		= LoadSoundMem( "./Data/Sounds/SE/Camera/Failure.wav" );
	SEhandles[M_GOTO_TITLE]		= LoadSoundMem( "./Data/Sounds/SE/Select/Back.wav" );
	SEhandles[M_PAUSE]			= LoadSoundMem( "./Data/Sounds/SE/Pause/Pause.wav" );
	SEhandles[M_RECORD_STAR]	= LoadSoundMem( "./Data/Sounds/SE/Result/Reward.wav" );
	SEhandles[M_UNDO]			= LoadSoundMem( "./Data/Sounds/SE/Game/Undo.wav" );
	SEhandles[M_UNLOCK_STAGE]	= LoadSoundMem( "./Data/Sounds/SE/Select/UnlockStage.wav" );
	SEhandles[M_UNRECORD_STAR]	= LoadSoundMem( "./Data/Sounds/SE/Result/FailureReward.wav" );
	SEhandles[M_SELECT]			= LoadSoundMem( "./Data/Sounds/SE/Select/Move.wav" );
	SEhandles[M_SHUTTER]		= LoadSoundMem( "./Data/Sounds/SE/Camera/Shutter.wav" );
	SEhandles[M_TOGGLE_CAMERA]	= LoadSoundMem( "./Data/Sounds/SE/Camera/ToggleCamera.wav" );
	SEhandles[M_VOICE]			= LoadSoundMem( "./Data/Sounds/SE/Human/Voice.wav" );


	// DEBUG:仮，ポーズ時に鳴らしている

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
	StopBGM();

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