#include "Common.h"
#include "Input.h"

bool IsTriggerPauseButton()
{
	// if ( TRG_J( 0, Input::START ) ) { return true; }
	if ( TRG( KEY_INPUT_P ) ) { return true; }
	// else
	return false;
}