#include <array>
#include <vector>
#include <memory>
#include <math.h>

#include "DxLib.h"
#include "Common.h"	// assert.hの読み込み, ToDegree定数, scast定数の使用
#include "Joypad.h"

const int	stickBorderLine		= 350;	// 0 ~ 1000, 「傾いている」と判断する境界値
const int	triggerBorderLine	= 128;	// 0 ~ 255,  「トリガー入力している」と判断する境界値

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

	for (
			int i = 0;
			num && i < scast<int>( INPUT_TYPES.size() );
			i++
		)
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
	/* * * * * * * * * * * * * * * * *

	いちおう
	複数の接続に対応できるよう
	inputX を配列としているが，
	まだ対応するようには組んでいない。

	もし対応するのなら，
	keysX, sticksInputX, sticksFrameX
	も配列（２次元）にし，
	現在の処理をfor文で囲むとよいだろう。
	また，その際
	取得関数も忘れずに変更すること。

	* * * * * * * * * * * * * * * * */

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

	// TODO:DirectInputがとれたっぽいので，あとはラッパー関数と，古いものを削除するのと，などする
}

// XBox
int GetJoypadButtonX		( JoypadButtonX button )
{
	assert( 0 <= button && button < ButtonXEnd );

	return pad.at( 0 ).pXInput->keys[button];
}
int GetJoypadStickInputX	( JoypadStickX stick, int borderLine )
{
	assert( 0 <= stick && stick < StickXEnd );

	return ( pad.at( 0 ).pXInput->sticksInput[stick] <= -borderLine )
		? -1
		:	(
				( borderLine <= pad.at( 0 ).pXInput->sticksInput[stick] )
				? 1
				: 0
			);
}
int GetJoypadStickFrameX	( JoypadStickX stick )
{
	assert( 0 <= stick && stick < StickXEnd );

	return pad.at( 0 ).pXInput->sticksFrame[stick];
}
int GetJoypadTriggerFrameX	( JoypadLRTriggerX trigger )
{
	assert( 0 <= trigger && trigger < LRTriggerXEnd );

	return pad.at( 0 ).pXInput->triggersFrame[trigger];
}
int GetJoypadStateX			( JoypadStateX state, int padNum )
{
	assert( 0 <= state && state < StateXEnd );
	assert( 0 <= padNum && padNum < 4 );

	switch ( state )
	{
	case XS_STICK_X_L: return pad.at( 0 ).pXInput->rowInput.ThumbLX;		//break;
	case XS_STICK_Y_L: return pad.at( 0 ).pXInput->rowInput.ThumbLY;		//break;
	case XS_STICK_X_R: return pad.at( 0 ).pXInput->rowInput.ThumbRX;		//break;
	case XS_STICK_Y_R: return pad.at( 0 ).pXInput->rowInput.ThumbRY;		//break;
	case XS_TRIGGER_L: return pad.at( 0 ).pXInput->rowInput.LeftTrigger;	//break;
	case XS_TRIGGER_R: return pad.at( 0 ).pXInput->rowInput.RightTrigger;	//break;
	}

	assert( !"エラー：GetJoypadStateX");
	return NULL;
}
float GetJoypadStickAngleX	( JoypadLRStickX side )
{
	assert( 0 <= side || side < LRStickXEnd );

	return pad.at( 0 ).pXInput->angle[side];
}
bool IsTiltJoypadStickX		( JoypadLRStickX side )
{
	assert( side != LRStickXEnd );

	if ( side == X_RIGHT )
	{
		if ( GetJoypadStickFrameX( X_STICK_R_X ) ) { return true; }
		if ( GetJoypadStickFrameX( X_STICK_R_Y ) ) { return true; }
		// else
		return false;
	}
	// else

	if ( GetJoypadStickFrameX( X_STICK_L_X ) ) { return true; }
	if ( GetJoypadStickFrameX( X_STICK_L_Y ) ) { return true; }
	// else
	return false;
}

// DirectInput
int GetJoypadButtonD		( JoypadButtonD button )
{
	assert( 0 <= button && button < ButtonDEnd );

	return pad.at( 0 ).pDInput->keys[button];
}
int GetJoypadPOV_D			( JoypadPOV_D button )
{
	assert( 0 <= button || button <= JoypadPOVEnd );

	return pad.at( 0 ).pDInput->keysPOV[button];
}
int GetJoypadStickInputD	( JoypadStickD stick, int borderLine )
{
	assert( 0 <= stick && stick < StickDEnd );

	return ( pad.at( 0 ).pDInput->sticksInput[stick] <= -borderLine )
		? -1
		:	(
				( borderLine <= pad.at( 0 ).pDInput->sticksInput[stick] )
				? 1
				: 0
			);
}
int GetJoypadStickFrameD	( JoypadStickD stick )
{
	assert( 0 <= stick && stick < StickDEnd );

	return pad.at( 0 ).pDInput->sticksFrame[stick];

}
float GetJoypadStickAngleD	( JoypadLRStickD side )
{
	assert( 0 <= side || side < LRStickDEnd );

	return pad.at( 0 ).pDInput->angle[side];
}
bool IsTiltJoypadStickD		( JoypadLRStickD side )
{
	assert( side != LRStickDEnd );

	if ( side == D_RIGHT )
	{
		if ( GetJoypadStickFrameD( D_STICK_R_X ) ) { return true; }
		if ( GetJoypadStickFrameD( D_STICK_R_Y ) ) { return true; }
		// else
		return false;
	}
	// else

	if ( GetJoypadStickFrameD( D_STICK_L_X ) ) { return true; }
	if ( GetJoypadStickFrameD( D_STICK_L_Y ) ) { return true; }
	// else
	return false;
}
