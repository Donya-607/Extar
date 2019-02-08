#ifndef INCLUDED_INPUT_H_
#define INCLUDED_INPUT_H_

#include "Keyboard.h"
#include "Joypad.h"

#if USE_DIRECT_INPUT

#define IS_TRG_J_D_ACCEL	( TRG_J_D( D_B_L1 ) || TRG_J_D( D_B_R1 ) || TRG_J_D( D_B_L2 ) || TRG_J_D( D_B_R2 ) )
#define IS_TRG_J_D_ATTACK	( TRG_J_D( D_B_A )  || TRG_J_D( D_B_B )  || TRG_J_D( D_B_X )  || TRG_J_D( D_B_Y )  )

#define IS_TRG_UP			( TRG( KEY_INPUT_UP )    || STICK_D_UP( D_STICK_L_Y )    )
#define IS_TRG_DOWN			( TRG( KEY_INPUT_DOWN )  || STICK_D_DOWN( D_STICK_L_Y )  )
#define IS_TRG_LEFT			( TRG( KEY_INPUT_LEFT )  || STICK_D_LEFT( D_STICK_L_X )  )
#define IS_TRG_RIGHT		( TRG( KEY_INPUT_RIGHT ) || STICK_D_RIGHT( D_STICK_L_X ) )

#else

#define IS_TRG_J_X_ACCEL	( TRG_J_X( XB_L ) || TRG_J_X( XB_R ) || TRG_J_TRIGGER_X( XT_LEFT ) || TRG_J_TRIGGER_X( XT_RIGHT ) )
#define IS_TRG_J_X_ATTACK	( TRG_J_X( XB_A ) || TRG_J_X( XB_B ) || TRG_J_X( XB_X ) || TRG_J_X( XB_Y ) )

#define IS_TRG_UP			( TRG( KEY_INPUT_UP )    || STICK_UP( X_STICK_L_Y )    )
#define IS_TRG_DOWN			( TRG( KEY_INPUT_DOWN )  || STICK_DOWN( X_STICK_L_Y )  )
#define IS_TRG_LEFT			( TRG( KEY_INPUT_LEFT )  || STICK_LEFT( X_STICK_L_X )  )
#define IS_TRG_RIGHT		( TRG( KEY_INPUT_RIGHT ) || STICK_RIGHT( X_STICK_L_X ) )

#endif // USE_DIRECT_INPUT

bool IsTriggerPauseButton();

/*
#define IS_PRESS_LEFT	( PRESS( KEY_INPUT_LEFT )  || PRESS_J_X( XB_LEFT )  || STICK_LEFT( X_STICK_L_X )  )
#define IS_PRESS_RIGHT	( PRESS( KEY_INPUT_RIGHT ) || PRESS_J_X( XB_RIGHT ) || STICK_RIGHT( X_STICK_L_X ) )
*/

#endif //INCLUDED_INPUT_H_
