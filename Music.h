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
	M_Title_Select,

	BGM_END
};
enum SE
{
	M_EXPOSURE = 0,
	M_FAILURE,

	M_SELECT,

	M_RECORD_STAR,

	// 仮，ポーズ時に鳴らしている
	M_E_NEXT,
	M_E_BACK,

	SE_END
};

void MusicInit();
void MusicUninit();

void PlayBGM( BGM type );
void PlaySE(  SE  type );
void StopBGM( BGM type = BGM_END );

#endif //INCLUDED_MUSIC_H_
