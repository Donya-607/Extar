#include "Common.h"
#include "Input.h"

// XInputのコントローラにのみ対応。DirectInputバージョンもあるにはあるが，ラップしていない
#define VER_CONTROLLER ( false )

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
	カメラ回転	：RB, RT
	露光			：X, A
	アンドゥ		：Y, B
	*/

	/* 過去の遺産・DirectInputバージョン
	// USE_DIRECT_INPUT

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
			return false;
		case InputTrigger::Select:
			return TRG( KEY_INPUT_SPACE );
		case InputTrigger::ToggleCamera:
			return TRG( KEY_INPUT_C );
		case InputTrigger::Exposure:
			return TRG( KEY_INPUT_Z );
		case InputTrigger::Undo:
			return TRG( KEY_INPUT_X );
		case InputTrigger::Pause:
			return TRG( KEY_INPUT_P );
		default: return false;
		}
		return false;
	}
	int  IsPressKeyboard( InputPress input )
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
	
	static constexpr int PAD_NUM = 0;
	bool IsTrgXInput( InputTrigger input )
	{
		switch ( input )
		{
		case InputTrigger::Up: // 十字キーと，スティックの両方を見る
			return TRG_J( PAD_NUM, Input::UP ) || TRG_J_STICK_X_UP( PAD_NUM );
		case InputTrigger::Down:
			return TRG_J( PAD_NUM, Input::DOWN ) || TRG_J_STICK_X_DOWN( PAD_NUM );
		case InputTrigger::Left:
			return TRG_J( PAD_NUM, Input::LEFT ) || TRG_J_STICK_X_LEFT( PAD_NUM );
		case InputTrigger::Right:
			return TRG_J( PAD_NUM, Input::RIGHT ) || TRG_J_STICK_X_RIGHT( PAD_NUM );
		case InputTrigger::LB: // ボタンと，トリガーの両方を見る
			return TRG_J( PAD_NUM, Input::L1 ) || TRG_J( PAD_NUM, Input::L2 );
		case InputTrigger::RB:
			return TRG_J( PAD_NUM, Input::R1 ) || TRG_J( PAD_NUM, Input::R2 );
		case InputTrigger::Select:
			return TRG_J( PAD_NUM, Input::SELECT );
		case InputTrigger::ToggleCamera:
			return TRG_J( PAD_NUM, Input::R1 ) || TRG_J( PAD_NUM, Input::R2 );
		case InputTrigger::Exposure:
			return TRG_J( PAD_NUM, Input::A ) || TRG_J( PAD_NUM, Input::X );
		case InputTrigger::Undo:
			return TRG_J( PAD_NUM, Input::B ) || TRG_J( PAD_NUM, Input::Y );
		case InputTrigger::Pause:
			return TRG_J( PAD_NUM, Input::START );
		default: return false;
		}
		return false;
	}
	int  IsPressXInput( InputPress input )
	{
		switch ( input )
		{
		case InputPress::Up:
			return PRESS_J( PAD_NUM, Input::UP ) || TILT_STICK_X_UP( PAD_NUM );
		case InputPress::Down:
			return PRESS_J( PAD_NUM, Input::DOWN ) || TILT_STICK_X_DOWN( PAD_NUM );
		case InputPress::Left:
			return PRESS_J( PAD_NUM, Input::LEFT ) || TILT_STICK_X_LEFT( PAD_NUM );
		case InputPress::Right:
			return PRESS_J( PAD_NUM, Input::RIGHT ) || TILT_STICK_X_RIGHT( PAD_NUM );
		default: return false;
		}
		return false;
	}
}

bool IsTrigger( InputTrigger input )
{
#if VER_CONTROLLER
	return InputImpl::IsTrgXInput( input );
#else
	return InputImpl::IsTrgKeyboard( input );
#endif // VER_CONTROLLER
}
int  IsPress( InputPress input )
{
#if VER_CONTROLLER
	return InputImpl::IsPressXInput( input );
#else
	return InputImpl::IsPressKeyboard( input );
#endif // VER_CONTROLLER
}

bool IsTriggerPauseButton()
{
#if VER_CONTROLLER
	return InputImpl::IsTrgXInput( InputTrigger::Pause );
#else
	return InputImpl::IsTrgKeyboard( InputTrigger::Pause );
#endif // VER_CONTROLLER
}

bool IsControllerVersion()
{
	return VER_CONTROLLER;
}

#undef VER_CONTROLLER
