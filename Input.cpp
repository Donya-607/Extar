#include "Common.h"
#include "Input.h"

// XInputのコントローラにのみ対応。DirectInputバージョンもあるにはあるが，ラップしていない
#define VER_CONTROLLER ( false );

namespace InputImpl
{
	/*
	キーボード
	１ページ送り	：非対応（LB, RBに相当する適当なボタンが無いため）
	カメラ回転	：Ｃキー
	露光			：Ｚキー
	アンドゥ		：Ｘキー

	コントローラ（XInput，０番のみ）
	１ページ送り	：LB, RB
	カメラ回転	：RB
	露光			：X, A
	アンドゥ		：Y, B
	*/

	bool IsTrgKeyboard( InputTrigger input )
	{
		switch ( input )
		{
		case InputTrigger::Up:
			return TRG( KEY_INPUT_UP );
		case InputTrigger::Down:
			return TRG( KEY_INPUT_DOWN );
		case InputTrigger::Left:
			return TRG( KEY_INPUT_LEFT );
		case InputTrigger::Right:
			return TRG( KEY_INPUT_RIGHT );
		case InputTrigger::LB:
			return false;
		case InputTrigger::RB:
			return TRG( KEY_INPUT_C );
		case InputTrigger::Select:
			return TRG( KEY_INPUT_SPACE );
		case InputTrigger::Exposure:
			return TRG( KEY_INPUT_Z );
		case InputTrigger::Undo:
			return TRG( KEY_INPUT_X );
		default: return false;
		}
		return false;
	}
	bool IsPressKeyboard( InputPress input )
	{
		switch ( input )
		{
		case InputPress::Up:
			return PRESS( KEY_INPUT_UP );
		case InputPress::Down:
			return PRESS( KEY_INPUT_DOWN );
		case InputPress::Left:
			return PRESS( KEY_INPUT_LEFT );
		case InputPress::Right:
			return PRESS( KEY_INPUT_RIGHT );
		default: return false;
		}
		return false;
	}
	
	bool IsTrgXInput( InputTrigger input )
	{
		switch ( input )
		{
		case InputTrigger::Up:
			return TRG_J_STICK_X_UP( 0 );
		case InputTrigger::Down:
			return TRG_J( KEY_INPUT_DOWN );
		case InputTrigger::Left:
			return TRG_J( KEY_INPUT_LEFT );
		case InputTrigger::Right:
			return TRG_J( KEY_INPUT_RIGHT );
		case InputTrigger::LB:
			return false;
		case InputTrigger::RB:
			return TRG_J( KEY_INPUT_C );
		case InputTrigger::Select:
			return TRG_J( KEY_INPUT_SPACE );
		case InputTrigger::Exposure:
			return TRG_J( KEY_INPUT_Z );
		case InputTrigger::Undo:
			return TRG_J( KEY_INPUT_X );
		default: return false;
		}
		return false;
	}
}

bool IsTrigger( InputTrigger input )
{
	switch ( input )
	{
	case InputTrigger::Up:
		break;
	case InputTrigger::Down:
		break;
	case InputTrigger::Left:
		break;
	case InputTrigger::Right:
		break;
	case InputTrigger::LB:
		break;
	case InputTrigger::RB:
		break;
	case InputTrigger::Select:
		break;
	case InputTrigger::Exposure:
		break;
	case InputTrigger::Undo:
		break;
	default: return false;
	}

	return false;
}
int  IsPress( InputPress input )
{
	return 0;
}

bool IsTriggerPauseButton()
{
	// if ( TRG_J( 0, Input::START ) ) { return true; }
	if ( TRG( KEY_INPUT_P ) ) { return true; }
	// else
	return false;
}

bool IsControllerVersion()
{
	return VER_CONTROLLER;
}

#undef VER_CONTROLLER
