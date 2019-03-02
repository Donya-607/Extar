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

// ボタンを押しているかどうか
// Xbox
#define PRESS_J_X( x )			( GetJoypadButtonX( x ) )
// DirectInput
#define PRESS_J_D( x )			( GetJoypadButtonD( x ) )

// スティックを倒した瞬間か
// Xbox
#define TRG_J_STICK_X_UP		( X_S_UP	== GetJoypadStickFrameX( X_STICK_L_Y ) )
#define TRG_J_STICK_X_DOWN		( X_S_DOWN	== GetJoypadStickFrameX( X_STICK_L_Y ) )
#define TRG_J_STICK_X_LEFT		( X_S_LEFT	== GetJoypadStickFrameX( X_STICK_L_X ) )
#define TRG_J_STICK_X_RIGHT		( X_S_RIGHT	== GetJoypadStickFrameX( X_STICK_L_X ) )
// DirectInput
#define TRG_J_STICK_D_UP		( D_S_UP	== GetJoypadStickFrameD( D_STICK_L_Y ) )
#define TRG_J_STICK_D_DOWN		( D_S_DOWN	== GetJoypadStickFrameD( D_STICK_L_Y ) )
#define TRG_J_STICK_D_LEFT		( D_S_LEFT	== GetJoypadStickFrameD( D_STICK_L_X ) )
#define TRG_J_STICK_D_RIGHT		( D_S_RIGHT	== GetJoypadStickFrameD( D_STICK_L_X ) )

// スティックを倒しているか
// Xbox
#define TILT_STICK_X_UP			( X_S_UP	== GetJoypadStickInputX( X_STICK_L_Y ) )
#define TILT_STICK_X_DOWN		( X_S_DOWN	== GetJoypadStickInputX( X_STICK_L_Y ) )
#define TILT_STICK_X_LEFT		( X_S_LEFT	== GetJoypadStickInputX( X_STICK_L_X ) )
#define TILT_STICK_X_RIGHT		( X_S_RIGHT	== GetJoypadStickInputX( X_STICK_L_X ) )
// DirectInput
#define TILT_STICK_D_UP			( D_S_UP	== GetJoypadStickInputD( D_STICK_L_Y ) )
#define TILT_STICK_D_DOWN		( D_S_DOWN	== GetJoypadStickInputD( D_STICK_L_Y ) )
#define TILT_STICK_D_LEFT		( D_S_LEFT	== GetJoypadStickInputD( D_STICK_L_X ) )
#define TILT_STICK_D_RIGHT		( D_S_RIGHT	== GetJoypadStickInputD( D_STICK_L_X ) )

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
	DB_Y = 0,
	DB_B,
	DB_A,
	DB_X,
	DB_L1,
	DB_R1,
	DB_L2,
	DB_R2,
	DB_SELECT,
	DB_START,
	DB_PUSH_L,
	DB_PUSH_R,

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

void JoypadInit();
void JoypadUninit();

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
/// ~-1 : 上or左に倒している<para></para>
///  0  : 倒していない<para></para>
///  1~ : 下or右に倒している
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
/// ~-1 : 上or左に倒している<para></para>
///  0  : 倒していない<para></para>
///  1~ : 下or右に倒している
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

#endif //INCLUDED_JOYPAD_H_
