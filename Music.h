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
	M_Title,

	BGM_END
};
enum SE
{
	M_EXPOSURE = 0,
	M_FAILURE,

	M_SELECT,

	// 仮，ポーズ時に鳴らしている
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
