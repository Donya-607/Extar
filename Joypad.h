#ifndef INCLUDED_JOYPAD_H_
#define INCLUDED_JOYPAD_H_

#include "DxLib.h"	// ボタン定数を利用するため

//-----------------------
//
//		Joypad.h
//
//-----------------------

// 「傾いている」と判断する境界値
extern const int stickBorderLine;
// 「トリガー入力している」と判断する境界値，0 ~ 255
extern const int triggerBorderLine;

// ボタンを押した瞬間かどうか
// Xbox
#define TRG_J_X( x )			( 1 == GetJoypadButtonX( x ) )
#define TRG_J_STICK_X( x )		( 1 == GetJoypadStickFrameX( x ) )
#define TRG_J_TRIGGER_X( x )	( 1 == GetJoypadTriggerFrameX( x ) )
// DirectInput
#define TRG_J_D( x )			( 1 == GetJoypadButtonD( x ) )
// HORI
#define TRG_J_H( x )			( 1 == GetJoypadHORI( x ) )

// ボタンを押しているかどうか
// Xbox
#define PRESS_J_X( x )			( GetJoypadButtonX( x ) )
#define PRESS_J_STICK_X( x )	( GetJoypadStickFrameX( x ) )
// HORI
#define PRESS_J_H( x )			( GetJoypadHORI( x ) )

// スティックを倒しているか
// Xbox
#define STICK_UP( x )		( X_S_UP	== GetJoypadStickInputX( x ) )
#define STICK_DOWN( x )		( X_S_DOWN	== GetJoypadStickInputX( x ) )
#define STICK_LEFT( x )		( X_S_LEFT	== GetJoypadStickInputX( x ) )
#define STICK_RIGHT( x )	( X_S_RIGHT	== GetJoypadStickInputX( x ) )
// DirectInput
#define STICK_D_UP( x )		( D_S_UP	== GetJoypadStickInputD( x ) )
#define STICK_D_DOWN( x )	( D_S_DOWN	== GetJoypadStickInputD( x ) )
#define STICK_D_LEFT( x )	( D_S_LEFT	== GetJoypadStickInputD( x ) )
#define STICK_D_RIGHT( x )	( D_S_RIGHT	== GetJoypadStickInputD( x ) )

// 使用するボタンのマクロ
#define ACCEL_TRIGGER_X		(XT_RIGHT)
#define BUTTON_ACCEL_X		(XB_B)
#define BUTTON_ATTACK_X		(XB_Y)

// DirectInput(仮)
#define BUTTON_ACCEL_D		(D_B_A)
#define BUTTON_ATTACK_D		(D_B_X)

// コントローラの種類
enum JoypadType
{
	T_Nothing = -1, 
	T_XInput, 
	T_DInput, 
	JoypadTypeEnd
};
// Xboxコントローラ
enum JoypadButtonX
{
	XB_UP		=	XINPUT_BUTTON_DPAD_UP,
	XB_DOWN		=	XINPUT_BUTTON_DPAD_DOWN,
	XB_LEFT		=	XINPUT_BUTTON_DPAD_LEFT,
	XB_RIGHT	=	XINPUT_BUTTON_DPAD_RIGHT,
	XB_START	=	XINPUT_BUTTON_START,
	XB_SELECT	=	XINPUT_BUTTON_BACK, 
	XB_PUSH_L	=	XINPUT_BUTTON_LEFT_THUMB, 
	XB_PUSH_R	=	XINPUT_BUTTON_RIGHT_THUMB,
	XB_L		=	XINPUT_BUTTON_LEFT_SHOULDER, 
	XB_R		=	XINPUT_BUTTON_RIGHT_SHOULDER, 
	XB_A		=	XINPUT_BUTTON_A,
	XB_B		=	XINPUT_BUTTON_B, 
	XB_X		=	XINPUT_BUTTON_X,
	XB_Y		=	XINPUT_BUTTON_Y,
	ButtonXEnd
};
enum JoypadStickX
{
	X_STICK_L_X = 0,
	X_STICK_L_Y,
	X_STICK_R_X,
	X_STICK_R_Y,
	StickXEnd, 

	// 関数の戻り値と比較するため
	X_S_UP		= -1, 
	X_S_DOWN	= 1, 
	X_S_LEFT	= -1, 
	X_S_RIGHT	= 1
};
enum JoypadStateX
{
	XS_STICK_X_L = 0, 
	XS_STICK_Y_L, 
	XS_STICK_X_R,
	XS_STICK_Y_R,
	XS_TRIGGER_L, 
	XS_TRIGGER_R,
	StateXEnd
};
enum JoypadLRStickX
{
	X_LEFT = 0, 
	X_RIGHT, 
	LRStickXEnd
};
enum JoypadLRTriggerX
{
	XT_LEFT = 0,
	XT_RIGHT,
	LRTriggerXEnd
};

// DirectInput
enum JoypadButtonD
{
	D_B_Y = 0,
	D_B_B,
	D_B_A,
	D_B_X,
	D_B_L1,
	D_B_R1,
	D_B_L2,
	D_B_R2,
	D_B_SELECT,
	D_B_START,
	D_B_PUSH_L,
	D_B_PUSH_R,

	ButtonDEnd
};
enum JoypadPOV_D
{
	POV_UP = 0, 
	POV_RIGHT_UP, 
	POV_RIGHT, 
	POV_RIGHT_DOWN, 
	POV_DOWN,
	POV_LEFT_DOWN,
	POV_LEFT,
	POV_LEFT_UP,
	JoypadPOVEnd 
};
enum JoypadStickD
{
	D_STICK_L_X = 0,
	D_STICK_L_Y,
	D_STICK_R_X,
	D_STICK_R_Y,
	StickDEnd,

	// 関数の戻り値と比較するため
	D_S_UP		= -1,
	D_S_DOWN	= 1,
	D_S_LEFT	= -1,
	D_S_RIGHT	= 1
};
enum JoypadLRStickD
{
	D_LEFT = 0,
	D_RIGHT,
	LRStickDEnd
};

// HORIコントローラ
enum JOYPAD_BUTTON_HORI
{
	H_DOWN,
	H_LEFT,
	H_RIGHT,
	H_UP,
	H_RECT,
	H_CROSS,
	H_CIRCLE,
	H_TRIANGLE,
	H_L,
	H_R,
	H_L2, 
	H_R2, 
	H_SELECT,
	H_START,
	H_LSTICK,
	H_RSTICK,
	HORIButtonEnd
};

// ループ用関数
void JoipadInit();
void JoypadUpdate();

// 分岐用関数
int GetJoypadButton();

// XBox
int GetJoypadButtonX		( JoypadButtonX button );

///<summary>
/// 「Xboxコントローラの，スティックの倒しぐあい」を返す。<para></para>
/// borderLine : 「傾いている」と判断する境界値<para></para>
/// 戻り値 : <para></para>
/// -1 : 上or左に倒している<para></para>
///  0 : 倒していない<para></para>
///  1 : 下or右に倒している
///</summary>
int GetJoypadStickInputX	( JoypadStickX stick, int borderLine = stickBorderLine );

///<summary>
/// 「Xboxコントローラの，スティックを倒しているフレーム数」を返す。<para></para>
/// 戻り値 : <para></para>
/// -1 : 上or左に倒している<para></para>
///  0 : 倒していない<para></para>
///  1 : 下or右に倒している
///</summary>
int GetJoypadStickFrameX	( JoypadStickX stick );

///<summary>
/// 「Xboxコントローラの，トリガーを入力しているフレーム数」を返す。<para></para>
///</summary>
int GetJoypadTriggerFrameX	( JoypadLRTriggerX trigger );

///<summary>
/// Xboxコントローラの，ボタン以外の入力を返す。<para></para>
/// padNum:パッド番号，０番から３番に対応
///</summary>
int GetJoypadStateX			( JoypadStateX state, int padNum = 0 );

/// <summary>
/// 「スティックを傾けている角度」を返す。<para></para>
/// 上を０度とし，時計まわりに増えていく
/// </summary>
float GetJoypadStickAngleX	( JoypadLRStickX side );

/// <summary>
/// 「スティックを倒しているかどうか」を返す。
/// </summary>
bool IsTiltJoypadStickX		( JoypadLRStickX side );

// DirectInput
int GetJoypadButtonD		( JoypadButtonD button );
int GetJoypadPOV_D			( JoypadPOV_D button );

///<summary>
/// 「スティックの倒しぐあい」を返す。<para></para>
/// borderLine : 「傾いている」と判断する境界値<para></para>
/// 戻り値 : <para></para>
/// -1 : 上or左に倒している<para></para>
///  0 : 倒していない<para></para>
///  1 : 下or右に倒している
///</summary>
int GetJoypadStickInputD	( JoypadStickD stick, int borderLine = stickBorderLine );

///<summary>
/// 「スティックを倒しているフレーム数」を返す。<para></para>
/// 戻り値 : <para></para>
/// -1 : 上or左に倒している<para></para>
///  0 : 倒していない<para></para>
///  1 : 下or右に倒している
///</summary>
int GetJoypadStickFrameD	( JoypadStickD stick );

/// <summary>
/// 「スティックを傾けている角度」を返す。<para></para>
/// 上を０度とし，時計まわりに増えていく
/// </summary>
float GetJoypadStickAngleD	( JoypadLRStickD side );

/// <summary>
/// 「スティックを倒しているかどうか」を返す。
/// </summary>
bool IsTiltJoypadStickD		( JoypadLRStickD side );

// Debris
int GetJoypadHORI			( JOYPAD_BUTTON_HORI button );

#endif //INCLUDED_JOYPAD_H_
