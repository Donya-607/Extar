#ifndef INCLUDED_INPUT_H_
#define INCLUDED_INPUT_H_

#include "Keyboard.h"
#include "Joypad.h"

#if 0 // USE_DIRECT_INPUT

#define IS_TRG_J_D_OPEN		( TRG_J_D( DB_L1 ) || TRG_J_D( DB_R1 ) || TRG_J_D( DB_L2 ) || TRG_J_D( DB_R2 ) )
#define IS_TRG_J_D_SHUTTER	( TRG_J_D( DB_A )  || TRG_J_D( DB_B )  || TRG_J_D( DB_X )  || TRG_J_D( DB_Y )  )

#define IS_TRG_UP			( TRG( KEY_INPUT_UP )    || TRG( KEY_INPUT_W ) || TRG_J_D( DB_UP    ) || TRG_J_STICK_D_UP     )
#define IS_TRG_DOWN			( TRG( KEY_INPUT_DOWN )  || TRG( KEY_INPUT_S ) || TRG_J_D( DB_DOWN  ) || TRG_J_STICK_D_DOWN   )
#define IS_TRG_LEFT			( TRG( KEY_INPUT_LEFT )  || TRG( KEY_INPUT_A ) || TRG_J_D( DB_LEFT  ) || TRG_J_STICK_D_LEFT   )
#define IS_TRG_RIGHT		( TRG( KEY_INPUT_RIGHT ) || TRG( KEY_INPUT_D ) || TRG_J_D( DB_RIGHT ) || TRG_J_STICK_D_RIGHT  )

#define IS_PRESS_UP			( PRESS( KEY_INPUT_UP )    || PRESS( KEY_INPUT_W ) || TILT_STICK_D_UP    )
#define IS_PRESS_DOWN		( PRESS( KEY_INPUT_DOWN )  || PRESS( KEY_INPUT_S ) || TILT_STICK_D_DOWN  )
#define IS_PRESS_LEFT		( PRESS( KEY_INPUT_LEFT )  || PRESS( KEY_INPUT_A ) || TILT_STICK_D_LEFT  )
#define IS_PRESS_RIGHT		( PRESS( KEY_INPUT_RIGHT ) || PRESS( KEY_INPUT_D ) || TILT_STICK_D_RIGHT )

#else

// #define IS_TRG_SELECT		( TRG( KEY_INPUT_SPACE ) )
// 
// #define IS_TRG_EXPOSURE		( TRG( KEY_INPUT_Z ) )
// #define IS_TRG_UNDO			( TRG( KEY_INPUT_X ) )
// 
// #define IS_TRG_L			( PRESS( KEY_INPUT_LSHIFT ) && TRG( KEY_INPUT_LEFT  ) )
// #define IS_TRG_R			( PRESS( KEY_INPUT_LSHIFT ) && TRG( KEY_INPUT_RIGHT ) )
// 
// #define IS_TRG_UP			( TRG( KEY_INPUT_UP			) )
// #define IS_TRG_DOWN			( TRG( KEY_INPUT_DOWN		) )
// #define IS_TRG_LEFT			( TRG( KEY_INPUT_LEFT		) )
// #define IS_TRG_RIGHT		( TRG( KEY_INPUT_RIGHT		) )
// 
// #define IS_PRESS_UP			( PRESS( KEY_INPUT_UP		) )
// #define IS_PRESS_DOWN		( PRESS( KEY_INPUT_DOWN		) )
// #define IS_PRESS_LEFT		( PRESS( KEY_INPUT_LEFT		) )
// #define IS_PRESS_RIGHT		( PRESS( KEY_INPUT_RIGHT	) )
// 
// #define IS_TRG_J_X_EXPOSURE	( TRG( KEY_INPUT_Z ) )

#define CHANGE_SHOW_IMGUI_KEY ( KEY_INPUT_V )

#endif // USE_DIRECT_INPUT

enum class InputTrigger
{
	Up,
	Down,
	Left,
	Right,
	LB,
	RB,
	Select,		// チュートリアルのリセットに使用する

	ToggleCamera,
	Exposure,
	Undo,
	Pause,
};
enum class InputPress
{
	Up,
	Down,
	Left,
	Right,
};
// 押された瞬間のみ真を返す
bool IsTrigger( InputTrigger input );
// 押されているフレーム数を返す
int  IsPress( InputPress input );

// bool IsTriggerPauseButton();

// XInputコントローラを使用するバージョンなら真を返す
bool IsControllerVersion();

#endif //INCLUDED_INPUT_H_
