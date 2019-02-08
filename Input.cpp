#include "Common.h"
#include "Input.h"

bool IsTriggerPauseButton()
{
#if USE_DIRECT_INPUT

	if ( TRG_J_D( D_B_START ) || TRG_J_D( D_B_SELECT ) )
	{
		return true;
	}

#else

	if ( TRG_J_X( XB_START ) || TRG_J_X( XB_SELECT ) )
	{
		return true;
	}

#endif // USE_DIRECT_INPUT

	if ( TRG( KEY_INPUT_P ) ) { return true; }

	return false;
}