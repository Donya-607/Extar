#include "Common.h"
#include "Input.h"

bool IsTriggerPauseButton()
{
	/*
#if USE_DIRECT_INPUT

	if ( TRG_J( 0, Input::START ) || TRG_J( 0, Input::SELECT ) )
	{
		return true;
	}

#else

	if ( TRG_J( 0, Input::START ) )
	{
		return true;
	}

#endif // USE_DIRECT_INPUT
	*/

	if ( TRG( KEY_INPUT_P ) ) { return true; }

	return false;
}