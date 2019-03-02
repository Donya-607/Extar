#include <array>
#include <vector>
#include <memory>
#include <math.h>

#include "DxLib.h"
#include "Common.h"	// assert.hの読み込み, ToDegree定数, scast定数の使用
#include "Joypad.h"

constexpr std::array<int, 4> INPUT_TYPES =
{
	DX_INPUT_PAD1, DX_INPUT_PAD2,
	DX_INPUT_PAD3, DX_INPUT_PAD4
};

struct XInput
{
	int		keys[ButtonXEnd];				// ボタンの入力状態
	int		sticksInput[StickXEnd];			// スティックの傾きぐあい
	int		sticksFrame[StickXEnd];			// スティックの入力フレーム
	int		triggersFrame[LRTriggerXEnd];	// トリガーの入力フレーム
	float	angle[LRStickXEnd];				// Degree
	XINPUT_STATE rowInput;
public:
	XInput() : keys(),
		sticksInput(), sticksFrame(),
		triggersFrame(), angle(),
		rowInput()
	{}
};
struct DInput
{
	int		keys[ButtonDEnd];		// ボタンの入力状態
	int		keysPOV[JoypadPOVEnd];	// 十字キーの入力フレーム
	int		sticksInput[StickDEnd];	// スティックの傾きぐあい
	int		sticksFrame[StickDEnd];	// スティックの入力フレーム
	float	angle[LRStickDEnd];		// Degree
	DINPUT_JOYSTATE rowInput;
public:
	DInput() : keys(), keysPOV(),
		sticksInput(), sticksFrame(),
		angle(), rowInput()
	{}
};
struct Joypad
{
	// 使う方のポインタにだけ，実体が入る形になる

	std::unique_ptr<XInput> pXInput;
	std::unique_ptr<DInput> pDInput;
public:
	Joypad() : pXInput( nullptr ), pDInput( nullptr ) {}
};

static std::vector<Joypad> pad;

static int	stickBorderLine		= 350;	// 0 ~ 1000, 「傾いている」と判断する境界値
static int	triggerBorderLine	= 128;	// 0 ~ 255,  「トリガー入力している」と判断する境界値

/*
// XInput
static int	 keysX[ButtonXEnd];		// ボタンの入力状態
static int	 sticksInputX[StickXEnd];// スティックの傾きぐあい
static int	 sticksFrameX[StickXEnd];// スティックの入力フレーム
static int	 triggersFrameX[LRTriggerXEnd];// トリガーの入力フレーム
static float angleX[LRStickXEnd];	// スティックの角度
static XINPUT_STATE inputX[4];		// その他の詳細な状態

// DInput
static int	 keysD[ButtonDEnd];		// ボタンの入力状態
static int	 keysPOV[JoypadPOVEnd];	// 十字キーの入力フレーム
static int	 sticksInputD[StickDEnd];// スティックの傾きぐあい
static int	 sticksFrameD[StickDEnd];// スティックの入力フレーム
static float angleD[LRStickDEnd];	// スティックの角度
static DINPUT_JOYSTATE inputD[4];	// 生の入力状態
*/

void JoypadInit()
{
	ReSetupJoypad();
	std::vector<Joypad>().swap( pad );

	int num = GetJoypadNum();
	if ( !num )
	{
		return;
	}
	// else

	for ( int i = 0; num && i < scast<int>( INPUT_TYPES.size() ); i++ )
	{
		if ( CheckJoypadXInput( INPUT_TYPES[i] ) )
		{
			pad.push_back( Joypad() );
			pad.back().pXInput.reset( new XInput() );
			num -= 2;
		}
		else
		{
			pad.push_back( Joypad() );
			pad.back().pDInput.reset( new DInput() );
			num -= 1;
		}
	}
}
void JoypadUninit()
{
	std::vector<Joypad>().swap( pad );
}

void JoypadUpdate()
{
	for ( int padIndex = 0; padIndex < scast<int>( pad.size() ); padIndex++ )
	{
		// XInput
		if ( pad.at( padIndex ).pXInput )
		{
			auto &&ref = pad.at( padIndex ).pXInput;

			// ボタンの入力を見る			GetJoypadXInputState
			{
				GetJoypadXInputState( INPUT_TYPES[padIndex], &ref->rowInput );

				for ( int i = 0; i < ButtonXEnd; i++ )
				{
					ref->keys[i] = ( ref->rowInput.Buttons[i] ) ? ref->keys[i] + 1 : 0;
				}
				// トリガー入力
				{
					ref->triggersFrame[XT_LEFT] =
						( triggerBorderLine < ref->rowInput.LeftTrigger )
						? ref->triggersFrame[XT_LEFT] + 1
						: 0;

					ref->triggersFrame[XT_RIGHT] =
						( triggerBorderLine < ref->rowInput.RightTrigger )
						? ref->triggersFrame[XT_RIGHT] + 1
						: 0;
				}
			}
			// スティックの傾きを見る		GetJoypadAnalogInput
			{
				GetJoypadAnalogInput(		&ref->sticksInput[X_STICK_L_X], &ref->sticksInput[X_STICK_L_Y],  INPUT_TYPES[padIndex] );
				GetJoypadAnalogInputRight(	&ref->sticksInput[X_STICK_R_X], &ref->sticksInput[X_STICK_R_Y],  INPUT_TYPES[padIndex] );

				for ( int i = 0; i < StickXEnd; i++ )
				{
					if ( stickBorderLine <= ref->sticksInput[i] )
					{
						ref->sticksFrame[i] += 1;
					}
					else if ( ref->sticksInput[i] <= -stickBorderLine )
					{
						ref->sticksFrame[i] -= 1;
					}
					else
					{
						ref->sticksFrame[i] = 0;
					}
				}
			}
			// スティックの角度を見る		
			{
				// Lスティック
				if ( !ref->sticksInput[X_STICK_L_X] && !ref->sticksInput[X_STICK_L_Y] )
				{
					ref->angle[X_LEFT] = 0;
				}
				else
				{
					ref->angle[X_LEFT] = ToDegree( atan2f( scast<float>( ref->sticksInput[X_STICK_L_Y] ), scast<float>( ref->sticksInput[X_STICK_L_X] ) ) ) + 90.0f/* ０度を上にするため */;
					ref->angle[X_LEFT] = ToDegree( atan2f( scast<float>( ref->sticksInput[X_STICK_L_Y] ), scast<float>( ref->sticksInput[X_STICK_L_X] ) ) ) + 90.0f/* ０度を上にするため */;
					if ( ref->angle[X_LEFT] < 0 )
					{
						// -180 ~ 180 の数値を，0 ~ 360 にしている
						ref->angle[X_LEFT] += 360;
					}
				}
				// Rスティック
				if ( !ref->sticksInput[X_STICK_R_X] && !ref->sticksInput[X_STICK_R_Y] )
				{
					ref->angle[X_RIGHT] = 0;
				}
				else
				{
					ref->angle[X_RIGHT] = ToDegree( atan2f( scast<float>( ref->sticksInput[X_STICK_R_Y] ), scast<float>( ref->sticksInput[X_STICK_R_X] ) ) ) + 90.0f/* ０度を上にするため */;
					if ( ref->angle[X_RIGHT] < 0 )
					{
						// -180 ~ 180 の数値を，0 ~ 360 にしている
						ref->angle[X_RIGHT] += 360;
					}
				}
			}
			// DEBUG:すべての状態を出力する
			/*
			{
				clsDx();
				printfDx
				( "\nStickL   : ( %d, %d )\nStickR   : ( %d, %d )\nPushL    : %d\nPushR    : %d\nTriggerL : %d\nTriggerR : %d\nSTART   : %d\nSELECT  : %d\nＬ : %d, Ｒ : %d\nＡ : %d, Ｂ : %d, Ｘ : %d, Ｙ : %d\n↑ : %d, ↓ : %d, ← : %d, → : %d\nAnalog Input\nLX : %d, LY : %d\nRX : %d, RY : %d\nAnalog Frame\nLX : %d, LY : %d\nRX : %d, RY : %d\n\nAngleX : %f, AngleY : %f",
					ref->rowInput.ThumbLX, ref->rowInput.ThumbLY,
					ref->rowInput.ThumbRX, ref->rowInput.ThumbRY,
					ref->keys[XB_PUSH_L],
					ref->keys[XB_PUSH_R],
					ref->rowInput.LeftTrigger,
					ref->rowInput.RightTrigger,
					ref->keys[XB_START],
					ref->keys[XB_SELECT],
					ref->keys[XB_L],
					ref->keys[XB_R],
					ref->keys[XB_A],
					ref->keys[XB_B],
					ref->keys[XB_X],
					ref->keys[XB_Y],
					ref->keys[XB_UP],
					ref->keys[XB_DOWN],
					ref->keys[XB_LEFT],
					ref->keys[XB_RIGHT],
					ref->sticksInput[X_STICK_L_X],
					ref->sticksInput[X_STICK_L_Y],
					ref->sticksInput[X_STICK_R_X],
					ref->sticksInput[X_STICK_R_Y],
					ref->sticksFrame[X_STICK_L_X],
					ref->sticksFrame[X_STICK_L_Y],
					ref->sticksFrame[X_STICK_R_X],
					ref->sticksFrame[X_STICK_R_Y],
					ref->angle[X_LEFT],
					ref->angle[X_RIGHT]
				);
			}
			*/

			continue;
		}
		// else

		// DInput
		if ( pad.at( padIndex ).pDInput )
		{
			auto &&ref = pad.at( padIndex ).pDInput;

			// ボタンの入力を見る			GetJoypadDirectInputState
			{
				GetJoypadDirectInputState( INPUT_TYPES[padIndex], &ref->rowInput );
				for ( int i = 0; i < ButtonDEnd; i++ )
				{
					ref->keys[i] = ( ref->rowInput.Buttons[i] ) ? ref->keys[i] + 1 : 0;
				}
				// 十字キー
				for ( int i = 0; i < JoypadPOVEnd; i++ )
				{
					ref->keysPOV[i] = // 45度きざみの角度を，100倍したものが格納されている
						( ( i * 45 * 100 ) == ref->rowInput.POV[0] )
						? ref->keysPOV[i] + 1
						: 0;
				}
			}
			// スティックの傾きを見る		GetJoypadAnalogInput
			{
				GetJoypadAnalogInput(		&ref->sticksInput[D_STICK_L_X], &ref->sticksInput[D_STICK_L_Y], INPUT_TYPES[padIndex] );
				GetJoypadAnalogInputRight(	&ref->sticksInput[D_STICK_R_X], &ref->sticksInput[D_STICK_R_Y], INPUT_TYPES[padIndex] );

				for ( int i = 0; i < StickDEnd; i++ )
				{
					if ( stickBorderLine <= ref->sticksInput[i] )
					{
						ref->sticksFrame[i] += 1;
					}
					else if ( ref->sticksInput[i] <= -stickBorderLine )
					{
						ref->sticksFrame[i] -= 1;
					}
					else
					{
						ref->sticksFrame[i] = 0;
					}
				}
			}
			// スティックの角度を見る
			{
				// Lスティック
				if ( !ref->sticksInput[D_STICK_L_X] && !ref->sticksInput[D_STICK_L_Y] )
				{
					ref->angle[D_LEFT] = 0;
				}
				else
				{
					ref->angle[D_LEFT] = ToDegree( atan2f( scast<float>( ref->sticksInput[D_STICK_L_Y] ), scast<float>( ref->sticksInput[D_STICK_L_X] ) ) ) + 90.0f/* ０度を上にするため */;
					if ( ref->angle[D_LEFT] < 0 )
					{
						// -180 ~ 180 の数値を，0 ~ 360 にしている
						ref->angle[D_LEFT] += 360;
					}
				}
				// Rスティック
				if ( !ref->sticksInput[D_STICK_R_X] && !ref->sticksInput[D_STICK_R_Y] )
				{
					ref->angle[D_RIGHT] = 0;
				}
				else
				{
					ref->angle[D_RIGHT] = ToDegree( atan2f( scast<float>( ref->sticksInput[D_STICK_R_Y] ), scast<float>( ref->sticksInput[D_STICK_R_X] ) ) ) + 90.0f/* ０度を上にするため */;
					if ( ref->angle[D_RIGHT] < 0 )
					{
						// -180 ~ 180 の数値を，0 ~ 360 にしている
						ref->angle[D_RIGHT] += 360;
					}
				}
			}
			// DEBUG:すべての状態を出力する
			/*
			{
			clsDx();
			printfDx( "\nX : %d, Y : %d, Z : %d\nRZ : %d\n十字 : %d\nAngle\nL : %6f, R : %6f\n",
			inputD[0].X,
			inputD[0].Y,
			inputD[0].Z,
			inputD[0].Rz,
			inputD[0].POV[0],
			angleD[D_LEFT],
			angleD[D_RIGHT]
			);
			for ( int i = 0; i < ButtonDEnd; i++ )
			{
			printfDx( "Button[%d] : %d\n", i, keysD[i] );
			}

			printfDx( "関数\n" );
			for ( int i = 0; i < ButtonDEnd; i++ )
			{
			printfDx( "Button[%d] : %d\n", i, GetJoypadButtonD( scast<JoypadButtonD>( i ) ) );
			}
			}
			*/

			continue;
		}
		// else

		assert( !"Error : Joypad type is not expected type." );
		exit( EXIT_FAILURE );
		return;
	}
}

// XBox
int GetJoypadButtonX		( int padNum, JoypadButtonX button )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= button && button < ButtonXEnd );

	return pad.at( padNum ).pXInput->keys[button];
}

///<summary>
/// 「Xboxコントローラの，スティックの倒しぐあい」を返す。<para></para>
/// borderLine : 「傾いている」と判断する境界値<para></para>
/// 戻り値 : <para></para>
/// -1 : 上or左に倒している<para></para>
///  0 : 倒していない<para></para>
///  1 : 下or右に倒している
///</summary>
int GetJoypadStickInputX	( int padNum, JoypadStickX stick, int borderLine )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= stick  && stick < StickXEnd );

	return ( pad.at( padNum ).pXInput->sticksInput[stick] <= -borderLine )
			? -1
			:	(
					( borderLine <= pad.at( padNum ).pXInput->sticksInput[stick] )
					? 1
					: 0
				);
}

///<summary>
/// 「Xboxコントローラの，スティックを倒しているフレーム数」を返す。<para></para>
/// 戻り値 : <para></para>
/// ~-1 : 上or左に倒している<para></para>
///  0  : 倒していない<para></para>
///  1~ : 下or右に倒している
///</summary>
int GetJoypadStickFrameX	( int padNum, JoypadStickX stick )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= stick  && stick < StickXEnd );

	return pad.at( padNum ).pXInput->sticksFrame[stick];
}

///<summary>
/// 「Xboxコントローラの，トリガーを入力しているフレーム数」を返す。<para></para>
///</summary>
int GetJoypadTriggerFrameX	( int padNum, JoypadLRTriggerX trigger )
{
	assert( 0 <= padNum  && padNum < scast<int>( pad.size() ) );
	assert( 0 <= trigger && trigger < LRTriggerXEnd );

	return pad.at( padNum ).pXInput->triggersFrame[trigger];
}

///<summary>
/// Xboxコントローラの，ボタン以外の入力を返す。<para></para>
///</summary>
int GetJoypadStateX			( int padNum, JoypadStateX state )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= state  && state < StateXEnd );

	switch ( state )
	{
	case XS_STICK_X_L: return pad.at( padNum ).pXInput->rowInput.ThumbLX;		//break;
	case XS_STICK_Y_L: return pad.at( padNum ).pXInput->rowInput.ThumbLY;		//break;
	case XS_STICK_X_R: return pad.at( padNum ).pXInput->rowInput.ThumbRX;		//break;
	case XS_STICK_Y_R: return pad.at( padNum ).pXInput->rowInput.ThumbRY;		//break;
	case XS_TRIGGER_L: return pad.at( padNum ).pXInput->rowInput.LeftTrigger;	//break;
	case XS_TRIGGER_R: return pad.at( padNum ).pXInput->rowInput.RightTrigger;	//break;
	}

	assert( !"エラー：GetJoypadStateX");
	return NULL;
}

/// <summary>
/// 「スティックを傾けている角度」を返す。<para></para>
/// 上を０度とし，時計まわりに増えていく
/// </summary>
float GetJoypadStickAngleX	( int padNum, JoypadLRStickX side )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= side   && side < LRStickXEnd );

	return pad.at( padNum ).pXInput->angle[side];
}

/// <summary>
/// 「スティックを倒しているかどうか」を返す。
/// </summary>
bool IsTiltJoypadStickX		( int padNum, JoypadLRStickX side )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= side   && side < LRStickXEnd );

	if ( side == X_RIGHT )
	{
		if ( GetJoypadStickFrameX( padNum, X_STICK_R_X ) ) { return true; }
		if ( GetJoypadStickFrameX( padNum, X_STICK_R_Y ) ) { return true; }
		// else
		return false;
	}
	// else

	if ( GetJoypadStickFrameX( padNum, X_STICK_L_X ) ) { return true; }
	if ( GetJoypadStickFrameX( padNum, X_STICK_L_Y ) ) { return true; }
	// else
	return false;
}

// DirectInput
int GetJoypadButtonD		( int padNum, JoypadButtonD button )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= button && button < ButtonDEnd );

	return pad.at( padNum ).pDInput->keys[button];
}
int GetJoypadPOV_D			( int padNum, JoypadPOV_D button )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= button && button <= JoypadPOVEnd );

	return pad.at( padNum ).pDInput->keysPOV[button];
}

///<summary>
/// 「スティックの倒しぐあい」を返す。<para></para>
/// borderLine : 「傾いている」と判断する境界値<para></para>
/// 戻り値 : <para></para>
/// -1 : 上or左に倒している<para></para>
///  0 : 倒していない<para></para>
///  1 : 下or右に倒している
///</summary>
int GetJoypadStickInputD	( int padNum, JoypadStickD stick, int borderLine )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= stick  && stick < StickDEnd );

	return ( pad.at( padNum ).pDInput->sticksInput[stick] <= -borderLine )
		? -1
		:	(
				( borderLine <= pad.at( padNum ).pDInput->sticksInput[stick] )
				? 1
				: 0
			);
}

///<summary>
/// 「スティックを倒しているフレーム数」を返す。<para></para>
/// 戻り値 : <para></para>
/// ~-1 : 上or左に倒している<para></para>
///  0  : 倒していない<para></para>
///  1~ : 下or右に倒している
///</summary>
int GetJoypadStickFrameD	( int padNum, JoypadStickD stick )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= stick  && stick < StickDEnd );

	return pad.at( padNum ).pDInput->sticksFrame[stick];

}

/// <summary>
/// 「スティックを傾けている角度」を返す。<para></para>
/// 上を０度とし，時計まわりに増えていく
/// </summary>
float GetJoypadStickAngleD	( int padNum, JoypadLRStickD side )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= side   && side < LRStickDEnd );

	return pad.at( padNum ).pDInput->angle[side];
}

/// <summary>
/// 「スティックを倒しているかどうか」を返す。
/// </summary>
bool IsTiltJoypadStickD		( int padNum, JoypadLRStickD side )
{
	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );
	assert( 0 <= side   && side < LRStickDEnd );

	if ( side == D_RIGHT )
	{
		if ( GetJoypadStickFrameD( padNum, D_STICK_R_X ) ) { return true; }
		if ( GetJoypadStickFrameD( padNum, D_STICK_R_Y ) ) { return true; }
		// else
		return false;
	}
	// else

	if ( GetJoypadStickFrameD( padNum, D_STICK_L_X ) ) { return true; }
	if ( GetJoypadStickFrameD( padNum, D_STICK_L_Y ) ) { return true; }
	// else
	return false;
}


Input operator | ( Input L, Input R )
{
	return static_cast<Input>( static_cast<int>( L ) | static_cast<int>( R ) );
}
Input operator & ( Input L, Input R )
{
	return static_cast<Input>( static_cast<int>( L ) & static_cast<int>( R ) );
}

bool IsConnectJoypad()
{
	if ( pad.empty() )
	{
		return false;
	}
	// elee

	return true;
}
int  GetConnectedJoypadNum()
{
	return scast<int>( pad.size() );
}
int  GetJoypadButton( int padNum, Input input )
{
	if ( pad.empty() )
	{
		return 0;
	}
	// else

	assert( 0 <= padNum && padNum < scast<int>( pad.size() ) );

	// XInput
	if ( pad.at( padNum ).pXInput )
	{
		// Button
		{
			JoypadButtonX button = ButtonXEnd;
			switch ( input )
			{
			case Input::UP:			button = XB_UP;			break;
			case Input::DOWN:		button = XB_DOWN;		break;
			case Input::LEFT:		button = XB_LEFT;		break;
			case Input::RIGHT:		button = XB_RIGHT;		break;
			case Input::START:		button = XB_START;		break;
			case Input::SELECT:		button = XB_SELECT;		break;
			case Input::L1:			button = XB_L;			break;
			case Input::R1:			button = XB_R;			break;
			case Input::A:			button = XB_A;			break;
			case Input::B:			button = XB_B;			break;
			case Input::X:			button = XB_X;			break;
			case Input::Y:			button = XB_Y;			break;
			case Input::PRESS_L:	button = XB_PUSH_L;		break;
			case Input::PRESS_R:	button = XB_PUSH_R;		break;
			}
			if ( button != ButtonXEnd )
			{
				return GetJoypadButtonX( padNum, button );
			}
		}
		// else

		// Stick
		{
			JoypadStickX stick = StickXEnd;
			switch ( input )
			{
			case Input::STICK_LX: stick = X_STICK_L_X; break;
			case Input::STICK_LY: stick = X_STICK_L_Y; break;
			case Input::STICK_RX: stick = X_STICK_R_X; break;
			case Input::STICK_RY: stick = X_STICK_R_Y; break;
			}
			if ( stick != StickXEnd )
			{
				return GetJoypadStickFrameX( padNum, stick );
			}
		}
		// else

		// Shoulder
		{
			JoypadLRTriggerX trigger = LRTriggerXEnd;
			switch ( input )
			{
			case Input::L2: trigger = XT_LEFT;  break;
			case Input::R2: trigger = XT_RIGHT; break;
			}
			if ( trigger != LRTriggerXEnd )
			{
				return GetJoypadTriggerFrameX( padNum, trigger );
			}
		}

		assert( !"Error : input argument error." );
		return NULL;
	}
	// else

	// DInput
	if ( pad.at( padNum ).pDInput )
	{
		// Button
		{
			JoypadButtonD button = ButtonDEnd;
			switch ( input )
			{
			case Input::START:		button = DB_START;		break;
			case Input::SELECT:		button = DB_SELECT;		break;
			case Input::L1:			button = DB_L1;			break;
			case Input::R1:			button = DB_R1;			break;
			case Input::L2:			button = DB_L2;			break;
			case Input::R2:			button = DB_R2;			break;
			case Input::A:			button = DB_A;			break;
			case Input::B:			button = DB_B;			break;
			case Input::X:			button = DB_X;			break;
			case Input::Y:			button = DB_Y;			break;
			case Input::PRESS_L:	button = DB_PUSH_L;		break;
			case Input::PRESS_R:	button = DB_PUSH_R;		break;
			}
			if ( button != ButtonDEnd )
			{
				return GetJoypadButtonD( padNum, button );
			}
		}
		// else

		// POV
		{
			JoypadPOV_D pov = JoypadPOVEnd;
			switch ( input )
			{
			case Input::UP:		pov = POV_UP;		break;
			case Input::DOWN:	pov = POV_DOWN;		break;
			case Input::LEFT:	pov = POV_LEFT;		break;
			case Input::RIGHT:	pov = POV_RIGHT;	break;
			}
			if ( input == ( Input::UP	| Input::RIGHT	) ) { pov = POV_RIGHT_UP;	}
			if ( input == ( Input::DOWN	| Input::RIGHT	) ) { pov = POV_RIGHT_DOWN;	}
			if ( input == ( Input::UP	| Input::LEFT	) ) { pov = POV_LEFT_UP;	}
			if ( input == ( Input::DOWN	| Input::LEFT	) ) { pov = POV_LEFT_DOWN;	}

			if ( pov != JoypadPOVEnd )
			{
				return GetJoypadPOV_D( padNum, pov );
			}
		}
		// else

		// Stick
		{
			JoypadStickD stick = StickDEnd;
			switch ( input )
			{
			case Input::STICK_LX: stick = D_STICK_L_X; break;
			case Input::STICK_LY: stick = D_STICK_L_Y; break;
			case Input::STICK_RX: stick = D_STICK_R_X; break;
			case Input::STICK_RY: stick = D_STICK_R_Y; break;
			}
			if ( stick != StickDEnd )
			{
				return GetJoypadStickFrameD( padNum, stick );
			}
		}
		// else

		assert( !"Error : input argument error." );
		return NULL;
	}
	// else

	assert( !"Error : Joypad type is not expected type." );
	exit( EXIT_FAILURE );
	return NULL;
}
