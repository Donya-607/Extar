#ifndef INCLUDED_JOYPAD_H_
#define INCLUDED_JOYPAD_H_

#include "DxLib.h"	// �{�^���萔�𗘗p���邽��

//-----------------------
//
//		Joypad.h
//
//-----------------------

// �u�X���Ă���v�Ɣ��f���鋫�E�l
extern const int stickBorderLine;
// �u�g���K�[���͂��Ă���v�Ɣ��f���鋫�E�l�C0 ~ 255
extern const int triggerBorderLine;

// �{�^�����������u�Ԃ��ǂ���
// Xbox
#define TRG_J_X( x )			( 1 == GetJoypadButtonX( x ) )
#define TRG_J_STICK_X( x )		( 1 == GetJoypadStickFrameX( x ) )
#define TRG_J_TRIGGER_X( x )	( 1 == GetJoypadTriggerFrameX( x ) )
// DirectInput
#define TRG_J_D( x )			( 1 == GetJoypadButtonD( x ) )

// �{�^���������Ă��邩�ǂ���
// Xbox
#define PRESS_J_X( x )			( GetJoypadButtonX( x ) )
// DirectInput
#define PRESS_J_D( x )			( GetJoypadButtonD( x ) )

// �X�e�B�b�N��|�����u�Ԃ�
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

// �X�e�B�b�N��|���Ă��邩
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

// ����p�֐�
int GetJoypadButton();

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

#endif //INCLUDED_JOYPAD_H_
