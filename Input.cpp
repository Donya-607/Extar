#include "Common.h"
#include "Input.h"

// XInput�̃R���g���[���ɂ̂ݑΉ��BDirectInput�o�[�W����������ɂ͂��邪�C���b�v���Ă��Ȃ�
#define VER_CONTROLLER ( false );

namespace InputImpl
{
	/*
	�L�[�{�[�h
	�P�y�[�W����	�F��Ή��iLB, RB�ɑ�������K���ȃ{�^�����������߁j
	�J������]	�F�b�L�[
	�I��			�F�y�L�[
	�A���h�D		�F�w�L�[

	�R���g���[���iXInput�C�O�Ԃ̂݁j
	�P�y�[�W����	�FLB, RB
	�J������]	�FRB
	�I��			�FX, A
	�A���h�D		�FY, B
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
