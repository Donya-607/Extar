 #ifndef INCLUDED_MUSIC_H_
#define INCLUDED_MUSIC_H_

//--------------------
//
//		Music.h
//
//--------------------

enum BGM
{
	M_Game = 0,
	M_Result,
	M_Title_Select,

	BGM_END
};
enum SE
{
	M_ALIGN_STARS,
	M_CAMERA_MOVE,
	M_CANT_PUSH,
	M_DECISION,
	M_EXPOSURE,
	M_FAILURE,
	M_GOTO_TITLE,
	M_PAUSE,
	M_RECORD_STAR,
	M_UNDO,
	M_UNLOCK_STAGE,
	M_UNRECORD_STAR,
	M_SELECT,
	M_SHUTTER,
	M_TOGGLE_CAMERA,
	M_VOICE,

	// DEBUG:仮，ポーズ時に鳴らしている

	M_E_NEXT,
	M_E_BACK,

	SE_END
};

void MusicInit();
void MusicUninit();

void PlayBGM( BGM type );
void PlaySE(  SE  type );
void StopBGM();

#endif //INCLUDED_MUSIC_H_
