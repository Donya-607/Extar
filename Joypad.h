#ifndef INCLUDED_JOYPAD_H_
#define INCLUDED_JOYPAD_H_

#include "DxLib.h"	// �{�^���萔�𗘗p���邽��

//-----------------------
//
//		Joypad.h
//
//-----------------------

// �{�^�����������u�Ԃ��ǂ���
#define TRG_J( padNum, input )		( 1 == GetJoypadButton( padNum, input ) )

// �{�^���������Ă��邩�ǂ���
#define PRESS_J( padNum, input )	( GetJoypadButton( padNum, input ) )

// �X�e�B�b�N��|�����u�Ԃ�
// Xbox
#define TRG_J_STICK_X_UP( padNum )			( X_S_UP	== GetJoypadButton( padNum, Input::STICK_LY ) )
#define TRG_J_STICK_X_DOWN( padNum )		( X_S_DOWN	== GetJoypadButton( padNum, Input::STICK_LY ) )
#define TRG_J_STICK_X_LEFT( padNum )		( X_S_LEFT	== GetJoypadButton( padNum, Input::STICK_LX ) )
#define TRG_J_STICK_X_RIGHT( padNum )		( X_S_RIGHT	== GetJoypadButton( padNum, Input::STICK_LX ) )
// DirectInput
#define TRG_J_STICK_D_UP( padNum )			( D_S_UP	== GetJoypadButton( padNum, Input::STICK_LY ) )
#define TRG_J_STICK_D_DOWN( padNum )		( D_S_DOWN	== GetJoypadButton( padNum, Input::STICK_LY ) )
#define TRG_J_STICK_D_LEFT( padNum )		( D_S_LEFT	== GetJoypadButton( padNum, Input::STICK_LX ) )
#define TRG_J_STICK_D_RIGHT( padNum )		( D_S_RIGHT	== GetJoypadButton( padNum, Input::STICK_LX ) )

// �X�e�B�b�N��|���Ă��邩
// Xbox
#define TILT_STICK_X_UP( padNum )			( X_S_UP	== GetJoypadButton( padNum, Input::STICK_LY ) )
#define TILT_STICK_X_DOWN( padNum )			( X_S_DOWN	== GetJoypadButton( padNum, Input::STICK_LY ) )
#define TILT_STICK_X_LEFT( padNum )			( X_S_LEFT	== GetJoypadButton( padNum, Input::STICK_LX ) )
#define TILT_STICK_X_RIGHT( padNum )		( X_S_RIGHT	== GetJoypadButton( padNum, Input::STICK_LX ) )
// DirectInput
#define TILT_STICK_D_UP( padNum )			( D_S_UP	== GetJoypadButton( padNum, Input::STICK_LY ) )
#define TILT_STICK_D_DOWN( padNum )			( D_S_DOWN	== GetJoypadButton( padNum, Input::STICK_LY ) )
#define TILT_STICK_D_LEFT( padNum )			( D_S_LEFT	== GetJoypadButton( padNum, Input::STICK_LX ) )
#define TILT_STICK_D_RIGHT( padNum )		( D_S_RIGHT	== GetJoypadButton( padNum, Input::STICK_LX ) )

// Xbox�R���g���[��
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

	// �֐��̖߂�l�Ɣ�r���邽��
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

	// �֐��̖߂�l�Ɣ�r���邽��
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

enum class Input
{
	UP			= 1 << 0,
	DOWN		= 1 << 1,
	LEFT		= 1 << 2,
	RIGHT		= 1 << 3,

	START		= 1 << 4,
	SELECT		= 1 << 5,

	L1			= 1 << 6,	// LB
	R1			= 1 << 7,	// RB
	L2			= 1 << 8,	// LT
	R2			= 1 << 9,	// RT

	A			= 1 << 10,
	B			= 1 << 11,
	X			= 1 << 12,
	Y			= 1 << 13,

	STICK_LX	= 1 << 14,	// �t���[������Ԃ�
	STICK_LY	= 1 << 15,	// �t���[������Ԃ�
	STICK_RX	= 1 << 16,	// �t���[������Ԃ�
	STICK_RY	= 1 << 17,	// �t���[������Ԃ�

	PRESS_L		= 1 << 18,
	PRESS_R		= 1 << 19,
};
Input operator | ( Input L, Input R );
Input operator & ( Input L, Input R );

bool IsConnectJoypad();
int  GetConnectedJoypadNum();
int  GetJoypadButton( int padNum, Input input );

/*
// XBox
int GetJoypadButtonX		( JoypadButtonX button );

///<summary>
/// �uXbox�R���g���[���́C�X�e�B�b�N�̓|���������v��Ԃ��B<para></para>
/// borderLine : �u�X���Ă���v�Ɣ��f���鋫�E�l<para></para>
/// �߂�l : <para></para>
/// -1 : ��or���ɓ|���Ă���<para></para>
///  0 : �|���Ă��Ȃ�<para></para>
///  1 : ��or�E�ɓ|���Ă���
///</summary>
int GetJoypadStickInputX	( JoypadStickX stick, int borderLine = stickBorderLine );

///<summary>
/// �uXbox�R���g���[���́C�X�e�B�b�N��|���Ă���t���[�����v��Ԃ��B<para></para>
/// �߂�l : <para></para>
/// ~-1 : ��or���ɓ|���Ă���<para></para>
///  0  : �|���Ă��Ȃ�<para></para>
///  1~ : ��or�E�ɓ|���Ă���
///</summary>
int GetJoypadStickFrameX	( JoypadStickX stick );

///<summary>
/// �uXbox�R���g���[���́C�g���K�[����͂��Ă���t���[�����v��Ԃ��B<para></para>
///</summary>
int GetJoypadTriggerFrameX	( JoypadLRTriggerX trigger );

///<summary>
/// Xbox�R���g���[���́C�{�^���ȊO�̓��͂�Ԃ��B<para></para>
/// padNum:�p�b�h�ԍ��C�O�Ԃ���R�ԂɑΉ�
///</summary>
int GetJoypadStateX			( JoypadStateX state, int padNum = 0 );

/// <summary>
/// �u�X�e�B�b�N���X���Ă���p�x�v��Ԃ��B<para></para>
/// ����O�x�Ƃ��C���v�܂��ɑ����Ă���
/// </summary>
float GetJoypadStickAngleX	( JoypadLRStickX side );

/// <summary>
/// �u�X�e�B�b�N��|���Ă��邩�ǂ����v��Ԃ��B
/// </summary>
bool IsTiltJoypadStickX		( JoypadLRStickX side );

// DirectInput
int GetJoypadButtonD		( JoypadButtonD button );
int GetJoypadPOV_D			( JoypadPOV_D button );

///<summary>
/// �u�X�e�B�b�N�̓|���������v��Ԃ��B<para></para>
/// borderLine : �u�X���Ă���v�Ɣ��f���鋫�E�l<para></para>
/// �߂�l : <para></para>
/// -1 : ��or���ɓ|���Ă���<para></para>
///  0 : �|���Ă��Ȃ�<para></para>
///  1 : ��or�E�ɓ|���Ă���
///</summary>
int GetJoypadStickInputD	( JoypadStickD stick, int borderLine = stickBorderLine );

///<summary>
/// �u�X�e�B�b�N��|���Ă���t���[�����v��Ԃ��B<para></para>
/// �߂�l : <para></para>
/// ~-1 : ��or���ɓ|���Ă���<para></para>
///  0  : �|���Ă��Ȃ�<para></para>
///  1~ : ��or�E�ɓ|���Ă���
///</summary>
int GetJoypadStickFrameD	( JoypadStickD stick );

/// <summary>
/// �u�X�e�B�b�N���X���Ă���p�x�v��Ԃ��B<para></para>
/// ����O�x�Ƃ��C���v�܂��ɑ����Ă���
/// </summary>
float GetJoypadStickAngleD	( JoypadLRStickD side );

/// <summary>
/// �u�X�e�B�b�N��|���Ă��邩�ǂ����v��Ԃ��B
/// </summary>
bool IsTiltJoypadStickD		( JoypadLRStickD side );

*/

#endif //INCLUDED_JOYPAD_H_
